
#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "communication.h"


typedef struct
{
  uint32_t CellCanId;   /*分配给单元体的CANid*/
  uint8_t  CellRow;     /*给单元体的行编号*/
  uint16_t CellColumn;  /*给单元体的列编号*/
  RxDataTypedef Rx;     /**/
}Master_TypeDef;

enum{
  CMD_NONE = 0U,
  CMD_MASTER_RESET_CELL,              /*主机复位单元体*/
  CMD_MASTER_RESET_CELL_SYSTEM,       /*主机复位单元体系统*/
  CMD_MASTER_REQUEST_ENABLE,          /*主机使能请求ID*/
  CMD_CELL_REQUEST_ID,                /*单元体请求分配ID*/
  CMD_MASTER_ALLOCATE_ONE_LAYER_DONE, /*主机分配一层完成*/
  CMD_MASTER_SEND_ID,                 /*主机发送ID*/
  CMD_MASTER_DELIVER,                 /*主机出货命令*/
  CMD_CELL_DELIVER_DONE,              /*单元体出货完成*/
};

void Master_Init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint8_t row, uint16_t col, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif