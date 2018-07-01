
#include "control.h"

extern CAN_HandleTypeDef hcan;

//static void Delay(uint32_t ms);


void Master_init(Master_TypeDef *m)
{
  /*单元体的地址从1开始分配*/
  m->CellColumn = 1;
  m->CellRow = 1;
  /*单元体的ID从10开始分配*/  
  m->CellCanId = 10;
  /*注意一定要初始化接收到数据的函数*/
  CommunicationInit();
  m->Rx.OnCorrectDataReceive = OnDataReceive;
  m->Rx.OnErrorDataReceive = DoNothing;
} 

// 给单元体分配CAN总线ID和单元体地址Addr
void AllocatingIdAndAddr(Master_TypeDef *m)
{
  /*同步信号0x03,告知所有单元体准备申请ID*/
  hcan.pTxMsg->Data[0] = CELL_REQUEST_ENABLE;
  HAL_CAN_Transmit_IT(&hcan);    
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);   
  
  uint8_t loop = 1;
  uint8_t timeoutcnt = 0;
  while(loop)
  {
    while(loop)
    {      
      /*等待单元体的请求信号*/    
      uint32_t tickstart = HAL_GetTick();
      while( hcan.pRxMsg->Data[0] != CELL_REQUEST_ID )
      {        
        if( HAL_GetTick() - tickstart > 100 )
        {
          /*等待超时，一层单元层分配完成*/
          timeoutcnt++;
          hcan.pTxMsg->Data[0] = CELL_ALLOCATE_ONE_LAYER_DONE;
          HAL_CAN_Transmit_IT(&hcan);   
          break;
        }
      }
      /*未超时，计数清零*/ 
      timeoutcnt = 0;
      
      /*分配ID*/
      hcan.pTxMsg->Data[0] = CELL_GET_ID;      
      hcan.pTxMsg->Data[1] = (uint8_t) (0xFF & (m->CellCanId>>24) );
      hcan.pTxMsg->Data[2] = (uint8_t) (0xFF & (m->CellCanId>>16) );
      hcan.pTxMsg->Data[3] = (uint8_t) (0xFF & (m->CellCanId>>8) );
      hcan.pTxMsg->Data[4] = (uint8_t) (0xFF & (m->CellCanId>>0) );
      hcan.pTxMsg->Data[5] = m->CellRow;
      hcan.pTxMsg->Data[6] = (uint8_t) (0xFF & (m->CellColumn>>8) );  
      hcan.pTxMsg->Data[7] = (uint8_t) (0xFF & (m->CellColumn>>0) );    
      HAL_CAN_Transmit_IT(&hcan);    
      
      /*按顺序分配ID*/
      m->CellColumn++;
      m->CellCanId++;
      
      memset(hcan.pRxMsg->Data, 0, 8);
    }// end while
    
    /*分配完一层，列复位，行加一*/
    m->CellColumn = 1;
    m->CellRow++;
    
    /*连续超时2次以上，所有单元层分配完成*/
    if(timeoutcnt >= 2)
    {      
      break;
    }            
  }// end while
    
  /*拉低控制脚*/
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);     
  
  return;
}

// addr: 要出货的单元体编号
// n   ：要出货的数量
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

