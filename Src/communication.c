

#include "communication.h"
#include "math.h"


extern UART_HandleTypeDef huart1;


#define RX_BUFFER_SIZE               1024
#define MoveReadPointer(pStart,n)    do{ pRead = (pStart) + (n);if(pRead >= (RxBuffer+RX_BUFFER_SIZE)){pRead -= RX_BUFFER_SIZE;}}while(0) //
#define ReadByte(index)              ( *(RxBuffer + (((pRead+index)- RxBuffer)%RX_BUFFER_SIZE)) )// Read one byte from RxBuffer


static unsigned char RxBuffer[RX_BUFFER_SIZE];
static unsigned char *pRead;
static unsigned char *pWrite;


extern DMA_HandleTypeDef hdma_usart1_rx;

static void UpdateWritePointer(void)
{
	volatile uint32_t tmp_cndtr = hdma_usart1_rx.Instance->CNDTR;
    
	pWrite = RxBuffer + RX_BUFFER_SIZE - tmp_cndtr;
    
	return;
}

static uint32_t UpdateData(void)
{
	UpdateWritePointer();
	if( (pWrite - pRead) > 0)
	{
		return pWrite - pRead;
	}	
	else if( (pWrite - pRead) < 0 )
	{	
		return pWrite + RX_BUFFER_SIZE - 1 - pRead ;
	}	
	else
	{
		return 0;
	}
}


void ParseDataFromUART(RxDataTypedef *RxData)
{
    unsigned int newDataLen = 0;
    unsigned char *pHead = 0;
    unsigned char *pEnd = 0;
    
    newDataLen = UpdateData();    
    if(newDataLen > 0)
    {// data exist
        while(newDataLen > 0)
        {
            pHead = 0;
            pEnd = 0;
            int pHeadCnt = 0;
            unsigned char *pHeadTemp[20];           
            memset(pHeadTemp,0,20);
            for(unsigned int i=0; i<newDataLen; i++)
            {
                unsigned int pl = 0;
                if( ReadByte(i) == 0xBB && (pHead==0))
                {// Header found
                    pHeadTemp[pHeadCnt++] = &(ReadByte(i));
                    if(pHeadCnt >= 20)
                    {
                        pHeadCnt = 20;
                    }
                }
                if( ReadByte(i) == 0x7E)
                {// Tail found9
                    pEnd = &(ReadByte(i));
                    for(int j=0; j<pHeadCnt; j++)
                    {
                        pl = (unsigned int)((*(pHeadTemp[j]+1)<<8)|(*(pHeadTemp[j]+2)));
                        if(5+pl == (pEnd-pHeadTemp[j])+1)
                        {
                            pHead = pHeadTemp[j];
                            break;
                        }
                        else
                        {
                            pHead = 0;
                        }
                    }// end for(int j=0; j<pHeadCnt; j++)
                }
                if((pEnd!=0) && (pHead!=0))
                {// probable find one frame
                    RxData->RxTotalFrameCnt ++;
                    unsigned char checkSum = 0;                                        
                    unsigned int len= pEnd-pHead; //5+pl==len+1
                    for(unsigned int j=1; j<len-1; j++)
                    {// cal check sum
                        checkSum += *(pHead+j);
                    }
                    
                    if( checkSum == *(pEnd-1) )
                    {// data correct
                        RxData->RxCorrectFrameCnt++;
                        //ReadData(pHead,RxData);
                        RxData->OnCorrectDataReceive();
                        RxData->IsGetNewData = 1;                        
                    }
                    else
                    {// check sum error
                        RxData->RxErrorFrameCnt++;
                        RxData->OnErrorDataReceive();
                    }
                    int l = pEnd - pRead + 1;
                    MoveReadPointer(pRead, l);
                    newDataLen -= l;
                    break;
                }// end if((pEnd!=0) && (pHead!=0))
            }// end for
            if((pHead==0) || (pEnd==0))
            {// can find header or tail,but not need to update pRead
                newDataLen = 0;
            }
        }// end while
    }// end if
}

void CommunicationInit(void)
{
	pRead = RxBuffer;
	pWrite = RxBuffer;
    
	HAL_UART_Receive_DMA(&huart1,RxBuffer,RX_BUFFER_SIZE);
}



















