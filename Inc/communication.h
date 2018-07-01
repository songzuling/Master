

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include "stm32f1xx_hal.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdarg.h"

void CommunicationInit(void);
void UART_WriteBytes(UART_HandleTypeDef *huart,uint8_t *data,int len);

typedef struct 
{    
    uint8_t  IsGetNewData;
    unsigned int ParaLen;
    unsigned long int RxTotalBytes;
    unsigned long int RxTotalFrameCnt;
    unsigned long int RxCorrectFrameCnt;
    unsigned long int RxErrorFrameCnt ;    
    void (*OnCorrectDataReceive)();
    void (*OnErrorDataReceive)();
}RxDataTypedef;

void ParseDataFromUART(RxDataTypedef *RxData);

#endif

