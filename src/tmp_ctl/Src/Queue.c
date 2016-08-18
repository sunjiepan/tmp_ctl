/*
********************************************************************************
*
*                                 Queue.c
*
* File          : Queue.c
* Version       : V1.0
* Author        : whq
* Mode          : Thumb2
* Toolchain     : 
* Description   : ¶ÓÁĞ²Ù×÷
*                
* History       : 2014.11.29   ĞŞÕı´æÈë¶ÓÁĞÊ±£¬¿ÉÄÜÔì³É¶Á¶ÓÁĞÊı¾İ³¤¶ÈÒì³£
				
* Date          : 2013.07.22
*******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "Queue.h"
/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketCreate(QUEUE8_t *pQ8, uint8_t *pBuf, uint32_t lenSize)
* Description   : ¶ÓÁĞ½¨Á¢
* Input         :   pQ8:    ¶ÓÁĞ
                    pBuf:   ¶ÓÁĞ»º³åÇøµØÖ·
                    bufSize:¶ÓÁĞ»º³åÇø´óĞ¡
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
uint32_t QUEUE_PacketCreate(QUEUE8_t *pQ8, uint8_t *pBuf, uint32_t bufSize)
{
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);

    pQ8->bufSize    = bufSize;
    pQ8->pBuf       = pBuf;
    pQ8->pStart     = pBuf;
    pQ8->pEnd       = pBuf;

    return 0;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketIn(QUEUE8_t *pQ8, uint8_t *pData, uint32_t len)
* Description   : Êı¾İÔØÈë¶ÓÁĞ
* Input         :   pQ8:    ¶ÓÁĞ
                    pData:  Òª½ø¶ÓÁĞµÄÊı¾İ
                    len:    Êı¾İ³¤¶È
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
uint32_t QUEUE_PacketIn(QUEUE8_t *pQ8, uint8_t *pData, uint32_t len)
{
    volatile uint8_t    *pEnd   = NULL;
    
    uint32_t            index   = 0;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pEnd = pQ8->pEnd;
    
    for (index = 0; index < len; index++)
    {
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize)
        {
            pEnd = pQ8->pBuf;
        }
        if (pEnd == pQ8->pStart) 
        {
            break;
        }
        
        *pQ8->pEnd = *pData++;
        
        pQ8->pEnd = pEnd;
    }
    
    return index;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketOut(QUEUE8_t *pQ8, uint8_t *pData, uint32_t dataLen)
* Description   : ¶ÓÁĞÖĞÈ¡Êı¾İ
* Input         :   pQ8:    ¶ÓÁĞ
                    pData:  »º³åÇø
                    dataLen:»º³åÇø´óĞ¡
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
uint32_t QUEUE_PacketOut(QUEUE8_t *pQ8, uint8_t *pData, uint32_t dataLen)
{
    uint32_t index = 0;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    while ((pQ8->pStart != pQ8->pEnd) && (index < dataLen) && (index < pQ8->bufSize))
    {
        pData[index++] = *pQ8->pStart++;
        if (pQ8->pStart >= pQ8->pBuf + pQ8->bufSize) 
        {
            pQ8->pStart = pQ8->pBuf;
        }
    }

    return index;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketLengthGet(QUEUE8_TYPE *pQ8)
* Description   : ¶ÓÁĞÖĞµÄÊı¾İ³¤¶È
* Input         :   pQ8:    ¶ÓÁĞ
                    pData:  »º³åÇø
                    dataLen:»º³åÇø´óĞ¡
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
uint32_t QUEUE_PacketLengthGet(QUEUE8_t *pQ8)
{
    
    volatile uint8_t    *pStart     = NULL;
    uint32_t            index       = 0;
    
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart = pQ8->pStart;

    while ((pStart != pQ8->pEnd) && (index < pQ8->bufSize))
    {
        index++;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) 
        {
            pStart = pQ8->pBuf;
        }
    }

    return index;
}


/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketStartEndDifferentCharSplit(QUEUE8_t *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen)
* Description   : ÒÔÆğÊ¼·ûºÍ½áÊø·ûÈ¡¶ÓÁĞÖĞµÄÊı¾İ (È¡³öµÄÊı¾İ °üÀ¨ÆğÊ¼·û ºÍ·Ö¸ô·û)
* Input         :   pQ8:        ¶ÓÁĞ
                    startChar:  ÆğÊ¼·û
                    endChar:    ½áÊø·û
                    pData:      »º³åÇø
                    dataLen:    »º³åÇø´óĞ¡
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
uint32_t QUEUE_PacketStartEndDifferentCharSplit(QUEUE8_t *pQ8, uint8_t startChar, uint8_t endChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    
    while ((pStart != pQ8->pEnd) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        if (startChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //Î´ÕÒµ½ÆğÊ¼·û
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--)          //²éÕÒ½áÊø×Ö·û
    {
        if (endChar == *pEnd) break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    }
    
    if (pEnd == pQ8->pEnd) return 0;                //Î´ÕÒ½áÊø·û
    if (count == -1) return 0;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    count   = pQ8->bufSize - count;   
    index   = 0;
    //»ñÈ¡´ÓÆğÊ¼×Ö·ûµ½½áÊø×Ö·ûµÄÊı¾İ
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
    {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    pQ8->pStart = pEnd;
    return index;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketStartEndCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
* Description   : ÌáÈ¡Ê×Î²·Ö¸ô·ûÄÚµÄÊı¾İ(°üÀ¨·Ö¸ô·û)
* Input         :   pQ8:        ¶ÓÁĞ
                    startChar:  ÆğÊ¼·û
                    endChar:    ½áÊø·û
                    pData:      »º³åÇø
                    dataLen:    »º³åÇø´óĞ¡
* Output        : 
* Other         : 
* Date          : 2013.08.30
*******************************************************************************/
uint32_t QUEUE_PacketStartEndCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    
    while ((pStart != pQ8->pEnd) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        if (splitChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) 
		{
			pQ8->pStart = pQ8->pBuf;
			pQ8->pEnd = pQ8->pBuf;
			return 0;              //Î´ÕÒµ½ÆğÊ¼·
		}
    if (count == -1)
		{
			pQ8->pStart = pQ8->pBuf;
			pQ8->pEnd = pQ8->pBuf;
			return 0;              //Î´ÕÒµ½ÆğÊ¼·
		}
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    while ((pEnd != pQ8->pEnd) && count--)          //²éÕÒ½áÊø×Ö·û
    {
        if (splitChar == *pEnd) break;
        if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    }
    
    if (pEnd == pQ8->pEnd) 
		{
			pQ8->pStart = pQ8->pBuf;
			pQ8->pEnd = pQ8->pBuf;
			return 0;              //Î´ÕÒµ½½áÊø·û
		}
    if (count == -1)
		{
			pQ8->pStart = pQ8->pBuf;
			pQ8->pEnd = pQ8->pBuf;
			return 0;              //Î´ÕÒµ½½áÊø·û
		}
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    count   = pQ8->bufSize - count;   
    index   = 0;
    //»ñÈ¡´ÓÆğÊ¼×Ö·ûµ½½áÊø×Ö·ûµÄÊı¾İ
    while ((pStart != pEnd) && (index < dataLen) && (index < pQ8->bufSize) && count--)
    {
        pData[index++] = *pStart++;
        if (pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    //Èç¹ûÈ¡³öµÄÊı¾İÖ»°üÀ¨·Ö¸ô·û£¬ÓĞ¿ÉÄÜÊÇÉÏ´Î½áÊø·ûºÍÏÂ´ÎÆğÊ¼·û£¬Òò´Ë·ÅÆúÉÏ´Î½áÊø·û¡£
    if (index <= 2)                                  
    {
        index = 0;
        if (--pStart < pQ8->pBuf) pStart = pQ8->pBuf + pQ8->bufSize - 1;
    }

    pQ8->pStart = pStart;
    return index;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
* Description   : ÌáÈ¡µ¥½áÊø·Ö¸ô·ûµÄÊı¾İ (°üÀ¨·Ö¸ô·û)
* Input         : 
* Output        : 
* Other         : 
* Date          : 2013.10.20
*******************************************************************************/
uint32_t QUEUE_PacketCharSplit(QUEUE8_t *pQ8, uint8_t splitChar, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    
    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        if (splitChar == *pStart) break;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //Î´ÕÒµ½ÆğÊ¼·û
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;
    
    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    index       = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }
    
    pQ8->pStart = pStart;
    return index;
}

/*******************************************************************************
* Function Name :QUEUE_PacketDoubleEndCharSplit
* Description   :ÌáÈ¡Ë«½áÊø·Ö¸ô·ûµÄÊı¾İ (°üÀ¨·Ö¸ô·û)
* Input         :QUEUE8_t * pQ8
* Input         :uint8_t splitChar1
* Input         :uint8_t splitChar2
* Input         :uint8_t * pData
* Input         :uint32_t dataLen
* Output        :uint32_t
* Other         :
* Date          :2014/03/27
*******************************************************************************/
uint32_t QUEUE_PacketDoubleEndCharSplit(QUEUE8_t *pQ8, uint8_t splitChar1, uint8_t splitChar2, uint8_t *pData, uint32_t dataLen)
{
    int32_t count;
    int32_t index;
    volatile uint8_t *pStart;
    volatile uint8_t *pEnd;
    uint8_t lastChar = 0;

    ASSERT_PARAM(pData);
    ASSERT_PARAM(pQ8);
    ASSERT_PARAM(pQ8->pBuf);
    ASSERT_PARAM(pQ8->pStart);
    ASSERT_PARAM(pQ8->pEnd);

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;

    while ((pStart != pQ8->pEnd) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        if ((splitChar1 == lastChar) && (splitChar2 == *pStart)) break;

        lastChar = *pStart;

        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    if (pStart == pQ8->pEnd) return 0;              //Î´ÕÒµ½ÆğÊ¼·û
    if (count == -1) return 0;
    pEnd = pStart;
    if (++pEnd >= pQ8->pBuf + pQ8->bufSize) pEnd = pQ8->pBuf;

    pStart      = pQ8->pStart;
    count       = pQ8->bufSize;
    index       = 0;
    while ((pStart != pEnd) && (index < dataLen) && count--)        //²éÕÒÆğÊ¼×Ö·û
    {
        pData[index++] = *pStart;
        if (++pStart >= pQ8->pBuf + pQ8->bufSize) pStart = pQ8->pBuf;
    }

    pQ8->pStart = pStart;
    return index;
}





/*******************************************************************************
* Function Name : uint32_t QUEUE_PacketCreate(QUEUE_STRUCT_t *pQueue, uint8_t *pBuf, uint32_t bufSize)
* Description   : ½á¹¹Ìå¶ÓÁĞ
* Input         :   pQueue:     ¶ÓÁĞÃû
                    pBuf:       ¶ÓÁĞ»º³åÇø
                    bufSize:    »»³åÇø´óĞ¡(×Ö½Ú)
                    blkSize:    µ¥½á¹¹Ìå´óĞ¡(×Ö½Ú)
* Output        : 0: ³É¹¦
* Other         : 
* Date          : 2014.08.13
*******************************************************************************/
uint32_t QUEUE_StructCreate(QUEUE_STRUCT_t *pQueue, void *pBuf, uint32_t bufSize, uint16_t blkSize)
{
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pBuf);
    ASSERT_PARAM(bufSize);
    ASSERT_PARAM(blkSize);

    pQueue->elemSize    = blkSize;
    pQueue->sumCount    = bufSize / blkSize;
    pQueue->pBuf        = pBuf;
    pQueue->start       = 0;
    pQueue->end         = 0;
    return 0;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_StructIn(QUEUE_STRUCT_t *pQueue, void *pData, uint16_t blkCount)
* Description   : ½á¹¹Ìå¶ÓÁĞÈëÕ» »º³åÇøÖĞÈç¹ûÂúÔò²»ÔØÈë
* Input         :   pQueue:     ¶ÓÁĞÃû
                    pData:      ×¼±¸ÈëÕ»µÄÊı¾İ
                    blkCount:   ×¼±¸ÈëÕ»µÄ½á¹¹Ìå¸öÊı(µ¥Î»:½á¹¹Ìå¸öÊı)
* Output        : ³É¹¦ÈëÕ»½á¹¹ÌåµÄ¸öÊı
* Other         : 
* Date          : 2014.08.14
*******************************************************************************/
uint32_t QUEUE_StructIn(QUEUE_STRUCT_t *pQueue, void *pData, uint32_t blkCount)
{
    uint32_t i = blkCount;
    uint32_t end = 0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    end = pQueue->end;
    for (i = 0; i < blkCount; i++)
    {
        //ÔÙ×°Ò»×éÊı¾İºó£¬Ö¸ÕëÊÇ·ñÖ¸ÏòÕ»Î²
        if (++end >= pQueue->sumCount)      
        {
            end = 0;
        }
        
        //»º³åÇøÌîÂú Ö±½ÓÍË³ö
        if (end == pQueue->start)   
        {
            break; 
        }
        
        memcpy((uint8_t *)pQueue->pBuf + pQueue->end * pQueue->elemSize, pData, pQueue->elemSize);

        pData = (uint8_t *)pData + pQueue->elemSize;
        pQueue->end = end;
    }
    
    return i;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_StructOut(QUEUE_STRUCT_t *pQueue, void *pData, uint16_t blkCount)
* Description   : ½á¹¹Ìå¶ÓÁĞ³öÕ»
* Input         :   pQueue:     ¶ÓÁĞÃû
                    pData:      ×¼±¸³öÕ»µÄÊı¾İ»º³åÇø
                    blkCount:   ´æ·Å³öÕ»½á¹¹ÌåµÄ×î´ó¸öÊı(µ¥Î»:½á¹¹Ìå¸öÊı)
* Output        : ³É¹¦³öÕ»½á¹¹ÌåµÄ¸öÊı
* Other         : 
* Date          : 2014.08.14
*******************************************************************************/
uint32_t QUEUE_StructOut(QUEUE_STRUCT_t *pQueue, void *pData, uint32_t blkCount)
{
    uint32_t index = 0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);
    ASSERT_PARAM(pData);

    while ((pQueue->start != pQueue->end) && (index < pQueue->sumCount) && (index < blkCount))
    {
        memcpy(pData, (uint8_t *)pQueue->pBuf + pQueue->start * pQueue->elemSize, pQueue->elemSize);

        pData = (uint8_t *)pData + pQueue->elemSize;
        index++;
        if (++pQueue->start >= pQueue->sumCount) 
        {
            pQueue->start = 0;
        }
    }

    return index;
}

/*******************************************************************************
* Function Name : uint32_t QUEUE_StructCountGet(QUEUE_STRUCT_t *pQueue)
* Description   : »ñÈ¡½á¹¹Ìå¶ÓÁĞÖĞµÄ¸öÊı(½á¹¹Ìå¸öÊı)
* Input         : 
* Output        : 
* Other         : 
* Date          : 2014.08.14
*******************************************************************************/
uint32_t QUEUE_StructCountGet(QUEUE_STRUCT_t *pQueue)
{
    uint32_t index = 0;
    uint32_t start =0;
    
    ASSERT_PARAM(pQueue);
    ASSERT_PARAM(pQueue->pBuf);

    start = pQueue->start;
    while ((start != pQueue->end) && (index < pQueue->sumCount))
    {
        index++;
        if (++start >= pQueue->sumCount) 
        {
            start = 0;
        }
    }

    return index;
}


#if     DEBUG_FULL_ASSERT

/*******************************************************************************
* Function Name : void ASSERT_FAILED(uint8_t* file, uint32_t line)
* Description   : Òì³£
* Input         : 
* Output        : 
* Other         : 
* Date          : 2013.08.29
*******************************************************************************/
void ASSERT_FAILED(uint8_t* file, uint32_t line)
{
    uint8_t flg = 1;

    printf("wrong information ÎÄ¼ş:%s µÚ%dĞĞ\r\n", file, line);
    while (flg);
}

#endif

