
#include "control.h"

extern CAN_HandleTypeDef hcan;

//static void Delay(uint32_t ms);


void Master_init(Master_TypeDef *m)
{
  /*��Ԫ��ĵ�ַ��1��ʼ����*/
  m->CellColumn = 1;
  m->CellRow = 1;
  /*��Ԫ���ID��10��ʼ����*/  
  m->CellCanId = 10;
  /*ע��һ��Ҫ��ʼ�����յ����ݵĺ���*/
  CommunicationInit();
  m->Rx.OnCorrectDataReceive = OnDataReceive;
  m->Rx.OnErrorDataReceive = DoNothing;
} 

// ����Ԫ�����CAN����ID�͵�Ԫ���ַAddr
void AllocatingIdAndAddr(Master_TypeDef *m)
{
  /*ͬ���ź�0x03,��֪���е�Ԫ��׼������ID*/
  hcan.pTxMsg->Data[0] = CELL_REQUEST_ENABLE;
  HAL_CAN_Transmit_IT(&hcan);    
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   
  
  uint8_t loop = 1;
  uint8_t timeoutcnt = 0;
  while(loop)
  {
    while(loop)
    {      
      /*�ȴ���Ԫ��������ź�*/    
      uint32_t tickstart = HAL_GetTick();
      while( hcan.pRxMsg->Data[0] != CELL_REQUEST_ID )
      {        
        if( HAL_GetTick() - tickstart > 100 )
        {
          /*�ȴ���ʱ��һ�㵥Ԫ��������*/
          timeoutcnt++;
          hcan.pTxMsg->Data[0] = CELL_ALLOCATE_ONE_LAYER_DONE;
          HAL_CAN_Transmit_IT(&hcan);   
          break;
        }
      }
      /*δ��ʱ����������*/ 
      timeoutcnt = 0;
      
      /*����ID*/
      hcan.pTxMsg->Data[0] = CELL_GET_ID;      
      hcan.pTxMsg->Data[1] = (uint8_t) (0xFF & (m->CellCanId>>24) );
      hcan.pTxMsg->Data[2] = (uint8_t) (0xFF & (m->CellCanId>>16) );
      hcan.pTxMsg->Data[3] = (uint8_t) (0xFF & (m->CellCanId>>8) );
      hcan.pTxMsg->Data[4] = (uint8_t) (0xFF & (m->CellCanId>>0) );
      hcan.pTxMsg->Data[5] = m->CellRow;
      hcan.pTxMsg->Data[6] = (uint8_t) (0xFF & (m->CellColumn>>8) );  
      hcan.pTxMsg->Data[7] = (uint8_t) (0xFF & (m->CellColumn>>0) );    
      HAL_CAN_Transmit_IT(&hcan);    
      
      /*��˳�����ID*/
      m->CellColumn++;
      m->CellCanId++;
      
      memset(hcan.pRxMsg->Data, 0, 8);
    }// end while
    
    /*������һ�㣬�и�λ���м�һ*/
    m->CellColumn = 1;
    m->CellRow++;
    
    /*������ʱ2�����ϣ����е�Ԫ��������*/
    if(timeoutcnt >= 2)
    {      
      break;
    }            
  }// end while
    
  /*���Ϳ��ƽ�*/
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);     
  
  return;
}

// addr: Ҫ�����ĵ�Ԫ����
// n   ��Ҫ����������
void Deliver(uint8_t row, uint16_t col, uint8_t n)
{  
  hcan.pTxMsg->Data[0] = CELL_DELIVER;
  hcan.pTxMsg->Data[1] = 0;
  hcan.pTxMsg->Data[2] = 0;
  hcan.pTxMsg->Data[3] = 0;
  hcan.pTxMsg->Data[4] = n;
  hcan.pTxMsg->Data[5] = row;
  hcan.pTxMsg->Data[6] = (uint8_t) (0xFF & (col>>8));  
  hcan.pTxMsg->Data[7] = (uint8_t) (0xFF & (col>>0));    
  HAL_CAN_Transmit_IT(&hcan);    
}

//static void Delay(uint32_t ms)
//{
//  uint32_t tickstart = HAL_GetTick();
//  
//  while( HAL_GetTick() - tickstart < ms ){};
//  
//  return;
//}

void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan)
{  
  HAL_CAN_Receive_IT(hcan, CAN_FIFO0);
}


void DoNothing(void)
{
  return;
}

void OnDataReceive(void)
{
  return;
}

