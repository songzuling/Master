
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
  CMD_NONE = 0U,
  CMD_MASTER_RESET_CELL,              /*������λ��Ԫ��*/
  CMD_MASTER_RESET_CELL_SYSTEM,       /*������λ��Ԫ��ϵͳ*/
  CMD_MASTER_REQUEST_ENABLE,          /*����ʹ������ID*/
  CMD_CELL_REQUEST_ID,                /*��Ԫ���������ID*/
  CMD_MASTER_ALLOCATE_ONE_LAYER_DONE, /*��������һ�����*/
  CMD_MASTER_SEND_ID,                 /*��������ID*/
  CMD_MASTER_DELIVER,                 /*������������*/
  CMD_CELL_DELIVER_DONE,              /*��Ԫ��������*/
};

void Master_Init(Master_TypeDef *m);
void AllocatingIdAndAddr(Master_TypeDef *m);
void Deliver(uint8_t row, uint16_t col, uint8_t n);
void DoNothing(void);
void OnDataReceive(void);
#endif