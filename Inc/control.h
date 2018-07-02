
#ifndef __CONTROL_H__
#define __CONTROL_H__


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "string.h"
#include "communication.h"


typedef struct
{
  uint32_t CellCanId;   /*�������Ԫ���CANid*/
  uint8_t  CellRow;     /*����Ԫ����б��*/
  uint16_t CellColumn;  /*����Ԫ����б��*/
  RxDataTypedef Rx;     /**/
}Master_TypeDef;

enum{
  CELL_NONE = 0U,
  CELL_RESET,                   /*��λ��Ԫ��*/
  CELL_RESET_SYSTEM,            /*��λ��Ԫ��ϵͳ*/
  CELL_REQUEST_ENABLE,          /*ʹ������ID*/
  CELL_REQUEST_ID,              /*�������ID*/
  CELL_ALLOCATE_ONE_LAYER_DONE, /*����һ�����*/
  CELL_GET_ID,                  /*�յ�ID*/
  CELL_DELIVER,                 /*��������*/
  CELL_DELIVER_DONE,            /*��Ԫ��������*/
};

void Master_init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint8_t row, uint16_t col, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif