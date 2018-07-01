
#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "communication.h"


typedef struct
{
  uint32_t CellCanId; /*分配给单元体的CANid*/
  uint16_t CelAddr;   /*给单元体的编号*/
  RxDataTypedef Rx;   /**/
}Master_TypeDef;

enum{
  CELL_NONE           = 0x00,
  CELL_REQUEST_ENABLE = 0x01, /*使能请求ID*/
  CELL_REQUEST_ID     = 0x02, /*请求分配ID*/
  CELL_GET_ID         = 0x03, /*收到ID*/
  CELL_DELIVER        = 0x04, /*出货命令*/
  CELL_DELIVER_DONE   = 0x05, /*单元体出货完成*/
};

void Master_init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint16_t addr, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif