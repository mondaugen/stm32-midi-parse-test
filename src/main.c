/* Includes ------------------------------------------------------------------*/
#include <stdlib.h> 
#include "stm32f4xx.h" 
#include "leds.h" 

#define MIDI_BAUD_RATE 31250 
#define UART_DMA_BUF_SIZE ((uint32_t)24)

char uartDMAData[UART_DMA_BUF_SIZE];

void Delay(__IO uint32_t nCount);
void UART5_Enable_Rx(void);

int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */

    /* Enable LEDs so we can toggle them */
    LEDs_Init();
    
    /* Enable USART For Tx */
    UART5_Enable_Rx();

    /* we have to do this */

  while (1);
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

void UART5_Enable_Rx(void)
{
    GPIO_InitTypeDef  GPIO_USART_InitStruct;
    USART_InitTypeDef USART_InitStruct;
    DMA_InitTypeDef   DMA_InitStruct;

    /* Enable Clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

    /* Set up GPIO for alternate function */
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource2,GPIO_AF_UART5);

    /* Configure GPIO to transmit */
    GPIO_USART_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_USART_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_USART_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_USART_InitStruct.GPIO_Pin = GPIO_Pin_2;
    GPIO_Init(GPIOD, &GPIO_USART_InitStruct);

    /* Configure USART */
    USART_InitStruct.USART_BaudRate = MIDI_BAUD_RATE;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_Mode = USART_Mode_Rx;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(UART5, &USART_InitStruct);

    /* Enable UART */
    USART_Cmd(UART5, ENABLE);

    /* configure DMA interrupts */
    DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
    NVIC_EnableIRQ(DMA1_Stream0_IRQn);


    DMA_StructInit(&DMA_InitStruct);
    DMA_InitStruct.DMA_Channel = DMA_Channel_4;
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&(UART5->DR));
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)uartDMAData;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_BufferSize = UART_DMA_BUF_SIZE;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
    /* DMA_FIFOThreshold ... N/A */
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_Init(DMA1_Stream0, &DMA_InitStruct);


    DMA_Cmd(DMA1_Stream0, ENABLE);

    /* Connect UART to DMA */
    USART_DMACmd(UART5, USART_DMAReq_Rx, ENABLE);

    /* wait for DMA to be enabled */
    while (DMA_GetCmdStatus(DMA1_Stream0) != ENABLE);

    LEDs_redSet(); /* DMA Enabled */

}





#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
