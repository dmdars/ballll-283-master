#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include "uart_node.h"
#include <stdint.h>
uint16_t compass;


void init_compas(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		// sort out clocks
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0 , GPIO_AF_UART4);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource1 , GPIO_AF_UART4);
		// Initialize USART
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

	     USART_DeInit(UART4);
	     USART_InitStructure.USART_BaudRate = 9600;
	     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	     USART_InitStructure.USART_StopBits = USART_StopBits_1;
	     USART_InitStructure.USART_Parity = USART_Parity_No;
	     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	     USART_Init(UART4,&USART_InitStructure);

	     USART_Cmd(UART4, ENABLE);

	     /* Here the USART4 receive interrupt is enabled
	      * and the interrupt controller is configured
	      * to jump to the USART4_IRQHandler() function
	      * if the USART4 receive interrupt occurs
	      */
	     USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); // enable the USART4 receive interrupt

	     NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;             // we want to configure the USART4 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART4 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART4 interrupts are globally enabled
	     NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	     // finally this2, E enables the complete USART4 peripheral
	     USART_Cmd(UART4,ENABLE);

}

void UART4_IRQHandler(void){
	if( USART_GetITStatus(UART4, USART_IT_RXNE) ){
		compass = USART_ReceiveData(UART4);
		compass = compass*360/255;
	}
}
