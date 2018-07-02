
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
  CELL_NONE = 0U,
  CELL_RESET,                   /*复位单元体*/
  CELL_RESET_SYSTEM,            /*复位单元体系统*/
  CELL_REQUEST_ENABLE,          /*使能请求ID*/
  CELL_REQUEST_ID,              /*请求分配ID*/
  CELL_ALLOCATE_ONE_LAYER_DONE, /*分配一层完成*/
  CELL_GET_ID,                  /*收到ID*/
  CELL_DELIVER,                 /*出货命令*/
  CELL_DELIVER_DONE,            /*单元体出货完成*/
};

void Master_init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint8_t row, uint16_t col, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif