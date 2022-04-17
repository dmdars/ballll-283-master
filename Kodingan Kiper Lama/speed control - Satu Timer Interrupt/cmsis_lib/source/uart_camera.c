#include "uart_camera.h"

/**
 * Variables
 */
uint8_t buffer;
float autosudut = 0;

/*
 * inisialisasi uart3 untuk data kamera, kompas dan akselerometer
 */
void init_camera(void)
{
	// give the struct a name
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// activate a clock for GPIOB and USART3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	/* Configure USART3 Tx (PB.10) Rx (PB.11) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Map USART3 to B.10
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11 , GPIO_AF_USART3);

	// Initialize USART
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* Configure USART */
	USART_Init(USART3, &USART_InitStructure);

	/* Enable the USART */
	USART_Cmd(USART3, ENABLE);

	 /* Here the USART3 receive interrupt is enabled
	  * and the interrupt controller is configured
	  * to jump to the USART3_IRQHandler() function
	  * if the USART3 receive interrupt occurs
	  */
	 USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the USART3 receive interrupt

	 NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;             // we want to configure the USART3 interrupts
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USAR31 interrupts
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART3 interrupts are globally enabled
	 NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	 // finally this2, E enables the complete USART3 peripheral
	 USART_Cmd(USART3,ENABLE);
}

/*
 * interrupt handler untuk uart3
 */
void USART3_IRQHandler(void)
{
	// fetch data from arduino nano
	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		buffer = USART_ReceiveData(USART3);
	}
	else{}
	// select X coordinate of camera servo
	if(flag=='X')
	{
		ballXCoor = buffer;
		flag = 0;
	}
	// select Y coordinate of camera servo
	else if(flag=='Y')
	{
		ballYCoor = buffer;
		flag = 0;
	}
	// select compass data
	else if (flag =='C')
	{
		compassDerajat1 = buffer;

		flag = 0;
	}
	// select X direction of accelerometer
	else if(flag=='O')
	{
		compassDerajat2 = buffer;
		compassHeading = (float)(compassDerajat1*100+compassDerajat2)/10;
		compassHeading = compassHeading - SUDUTGAWANG - autosudut;

		if(compassHeading < 0)
		{
			compassHeading = compassHeading + 360;
		}
		if(compassHeading > 360)
		{
			compassHeading = compassHeading - 360;
		}
		flag = 0;
	}
	// select Y direction of accelerometer
	else if(flag=='L')
	{
		acceleroY = buffer;
		flag = 0;
	}
	// select ball detection flag
	else if(flag == 'B')
	{
		ballFound = buffer;
		flag = 0;
	}
	// get flag for select data
	else
	{
		flag = buffer;
	}
}

void fungsiautosudut()
{
	autosudut = compassHeading;
}
