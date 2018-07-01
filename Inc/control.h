
#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "communication.h"


typedef struct
{
  uint32_t CellCanId; /*�������Ԫ���CANid*/
  uint16_t CelAddr;   /*����Ԫ��ı��*/
  RxDataTypedef Rx;   /**/
}Master_TypeDef;

enum{
  CELL_NONE           = 0x00,
  CELL_REQUEST_ENABLE = 0x01, /*ʹ������ID*/
  CELL_REQUEST_ID     = 0x02, /*�������ID*/
  CELL_GET_ID         = 0x03, /*�յ�ID*/
  CELL_DELIVER        = 0x04, /*��������*/
  CELL_DELIVER_DONE   = 0x05, /*��Ԫ��������*/
};

void Master_init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint16_t addr, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif