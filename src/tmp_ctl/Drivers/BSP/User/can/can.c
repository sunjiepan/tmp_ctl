#include "can.h" 

extern   CanTxMsg TxMessage;
extern   CanRxMsg RxMessage;
extern vu16 After_filter[8];
extern vu8  After_Value[16];
extern vu8 date_out[8];
extern vu8 date_out_1[8];

extern vu8 command_level;
extern vu8 number_board ;
extern vu8 Can1_Recv_Buf[8];
vu32 mask =0;
vu16 std_id =0x000;  
vu32 ext_id =0x1314;  
/*
 * 函数名：CAN_GPIO_Config
 * 描述  ：CAN的GPIO 配置,PB8上拉输入，PB9推挽输出
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_GPIO_Config(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
   	
  	/*外设时钟设置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

  	/*IO设置*/
	//GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);
	
	/* Configure CAN pin: RX PA11*/									          
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
    
	/* Configure CAN pin: TX PA12 */									               
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}

/*
 * 函数名：CAN_NVIC_Config
 * 描述  ：CAN的NVIC 配置,第1优先级组，0，0优先级
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_NVIC_Config(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure;
		/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	 	/*中断设置*/
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	   //CAN1 RX0中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		   //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			   //子优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * 函数名：CAN_Mode_Config
 * 描述  ：CAN的模式 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
static void CAN_Mode_Config(void)
{
   	CAN_InitTypeDef        CAN_InitStructure;
	 	/************************CAN通信参数设置**********************************/
	/*CAN寄存器初始化*/
	CAN_DeInit(CAN1);
	CAN_StructInit(&CAN_InitStructure);
	/*CAN单元初始化*/
	CAN_InitStructure.CAN_TTCM=DISABLE;			   //MCR-TTCM  时间触发通信模式使能
    CAN_InitStructure.CAN_ABOM=DISABLE;			   //MCR-ABOM  自动离线管理 
    CAN_InitStructure.CAN_AWUM=DISABLE;			   //MCR-AWUM  自动唤醒模式
    CAN_InitStructure.CAN_NART=DISABLE;			   //MCR-NART  禁止报文自动重传	  DISABLE-自动重传
    CAN_InitStructure.CAN_RFLM=DISABLE;			   //MCR-RFLM  接收FIFO 锁定模式  DISABLE-溢出时新报文会覆盖原有报文  
    CAN_InitStructure.CAN_TXFP=DISABLE;			   //MCR-TXFP  发送FIFO优先级 DISABLE-优先级取决于报文标示符 
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;  //正常发送模式
    CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;		   //BTR-SJW 重新同步跳跃宽度 2个时间单元
    CAN_InitStructure.CAN_BS1=CAN_BS1_13tq;		   //BTR-TS1 时间段1 占用了6个时间单元
    CAN_InitStructure.CAN_BS2=CAN_BS2_4tq;		   //BTR-TS1 时间段2 占用了3个时间单元
    CAN_InitStructure.CAN_Prescaler =16;		   ////BTR-BRP 波特率分频器  定义了时间单元的时间长度 36/(1+13+4)/16=0.125Mbps
	CAN_Init(CAN1, &CAN_InitStructure);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */


  
/*接收多个ID：0x7e9,0x1800f001,前面为标准ID，后面为扩展ID，要同时能接收这两个ID*/
static void CAN_Filter_Config_1(void)
{
		CAN_FilterInitTypeDef  CAN_FilterInitStructure;
		CAN_FilterInit(&CAN_FilterInitStructure);     //初始化CAN_FilterInitStructrue结构体变量  
		CAN_FilterInitStructure.CAN_FilterNumber=0;      //设置过滤器组0，范围为0~13  
		CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;    //设置过滤器组0为屏蔽模式  
		CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;   //设置过滤器组0位宽为32位  
			
		//标识位寄存器的设置  
		//ext_id<<3对齐，见上图9，再>>16取高16位  
		CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)ext_id<<3)&0xFFFF0000)>>16;;  //设置标识符寄存器高字节。  
		CAN_FilterInitStructure.CAN_FilterIdLow=(((u32)ext_id<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //要过滤的ID低位 
		//这里也可以这样设置  
		//CAN_FilterInitStructure.CAN_FilterIdHigh=std_id<<5;  //设置标识符寄存器高字节.这里为什么是左移5位呢？从上图可以看出，CAN_FilterIdHigh包含的是STD[0~10]和EXID[13~17],标准CAN ID本身是不包含扩展ID数据，因此为了要将标准CAN ID放入此寄存器，标准CAN ID首先应左移5位后才能对齐.  
		//CAN_FilterInitStructure.CAN_FilterIdLow=0|CAN_ID_EXT;   //设置标识符寄存器低字节,这里也可以设置为CAN_ID_STD  
			
		//屏蔽寄存器的设置  
		//这里的思路是先将标准CAN ID和扩展CAN ID对应的ID值先异或后取反，为什么？异或是为了找出两个CAN ID有哪些位是相同的，是相同的位则说明需要关心，需要关心的位对应的屏蔽码位应该设置为1，因此需要取反一下。最后再整体左移3位。  
		mask =(std_id<<18);//这里为什么左移18位？因为从ISO11898中可以看出，标准CAN ID占ID18~ID28，为了与CAN_FilterIdHigh对齐，应左移2位，接着为了与扩展CAN对应，还应该再左移16位，因此，总共应左移2+16＝18位。也可以用另一个方式来理解：直接看Mapping的内容，发现STDID相对EXID[0]偏移了18位,因此左移18位.  
		mask ^=ext_id;//将对齐后的标准CAN与扩展CAN异或后取反  
		mask =~mask;  
		mask <<=3;//再整体左移3位  
		mask |=0x02; //只接收数据帧，不接收远程帧  
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh=(mask>>16)&0x0007; //设置屏蔽寄存器高字节  
		CAN_FilterInitStructure.CAN_FilterMaskIdLow=mask&0xffff;   //设置屏蔽寄存器低字节  
			
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;  //此过滤器组关联到接收FIFO0  
		CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; //激活此过滤器组  
		CAN_FilterInit(&CAN_FilterInitStructure); //设置过滤器 
			/*CAN通信中断使能*/
		CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}




//static void CAN_Filter_Config(void)
//{
//   CAN_FilterInitTypeDef  CAN_FilterInitStructure;

//	/*CAN过滤器初始化*/
//	CAN_FilterInitStructure.CAN_FilterNumber=0;						//过滤器组0
//    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识符屏蔽位模式
//	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
//	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

//    CAN_FilterInitStructure.CAN_FilterIdHigh=(((u32)ext_id<<3)&0xFFFF0000)>>16;				//要过滤的ID高位 
//    CAN_FilterInitStructure.CAN_FilterIdLow= (((u32)ext_id<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF; //要过滤的ID低位 
//    CAN_FilterInitStructure.CAN_FilterMaskIdHigh= 0x0007;			//过滤器高16位每位必须匹配
//    CAN_FilterInitStructure.CAN_FilterMaskIdLow= 0xFFFF;			//过滤器低16位每位必须匹配
//	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;//CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
//	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
//	CAN_FilterInit(&CAN_FilterInitStructure);
//	/*CAN通信中断使能*/
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
//}


/*
 * 函数名：CAN_Config
 * 描述  ：完整配置CAN的功能
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */
void CAN_Config(void)
{
  CAN_GPIO_Config();
  CAN_NVIC_Config();
  CAN_Mode_Config();
  //CAN_Filter_Config(); 
	CAN_Filter_Config_1();
}


/*
 * 函数名：CAN_SetMsg
 * 描述  ：CAN通信报文内容设置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */	 
void CAN_SetMsg(void)
{	  
  //TxMessage.StdId=0x453;					 
  TxMessage.ExtId=0x1453;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为8字节
	TxMessage.Data[0]=Can1_Recv_Buf[0];
	TxMessage.Data[1]=Can1_Recv_Buf[1];
	TxMessage.Data[2]=Can1_Recv_Buf[2];
  TxMessage.Data[3]=Can1_Recv_Buf[3];
	TxMessage.Data[4]=Can1_Recv_Buf[4];
  TxMessage.Data[5]=Can1_Recv_Buf[5];
	TxMessage.Data[6]=Can1_Recv_Buf[6];
  TxMessage.Data[7]=Can1_Recv_Buf[7];
//	TxMessage.Data[0]=After_Value[0];
//  TxMessage.Data[1]=After_Value[1];
//	TxMessage.Data[2]=After_Value[2];
//  TxMessage.Data[3]=After_Value[3];
//	TxMessage.Data[4]=After_Value[4];
//  TxMessage.Data[5]=After_Value[5];
//	TxMessage.Data[6]=After_Value[6];
//  TxMessage.Data[7]=After_Value[7];
}
void CAN_SetMsg_return(void)
{	  
  //TxMessage.StdId=command_level;						 
  TxMessage.ExtId=0x1314;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为8字节
	TxMessage.Data[0]=date_out[0];
  TxMessage.Data[1]=date_out[1];
	TxMessage.Data[2]=date_out[2];
  TxMessage.Data[3]=date_out[3];
	TxMessage.Data[4]=date_out[4];
  TxMessage.Data[5]=date_out[5];
	TxMessage.Data[6]=date_out[6];
  TxMessage.Data[7]=date_out[7];
}

void CAN_SetMsg_1(void)
{	  
  //TxMessage.StdId=0x453;					 
  TxMessage.ExtId=0x1314;					 //使用的扩展ID
  TxMessage.IDE=CAN_ID_EXT;					 //扩展模式
  TxMessage.RTR=CAN_RTR_DATA;				 //发送的是数据
  TxMessage.DLC=8;							 //数据长度为8字节
	TxMessage.Data[0]=date_out_1[0];
  TxMessage.Data[1]=date_out_1[1];
	TxMessage.Data[2]=date_out_1[2];
  TxMessage.Data[3]=date_out_1[3];
	TxMessage.Data[4]=date_out_1[4];
  TxMessage.Data[5]=date_out_1[5];
	TxMessage.Data[6]=date_out_1[6];
  TxMessage.Data[7]=date_out_1[7];
}
/*************************END OF FILE******************************/

