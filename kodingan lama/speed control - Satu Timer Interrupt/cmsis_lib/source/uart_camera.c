#include "uart_camera.h"

/**
 * Variables
 */
uint8_t buffer;
float autosudut = 0;
float compassDegree = 0;
float compassBase = 0;
int rx_index = 0;
int counter = 0;
uint8_t uartRxBuff[6];


extern volatile int32_t rpm;
extern double current_y;

void fungsiautosudut()
{
	autosudut = compassBase;
}

/*
 * inisialisasi uart3 untuk data kamera, kompas dan akselerometer
 */
void init_camera(void)
{
	robotSelect = 0;
	nomorRobot = 0;
	controlMode = 0;

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

//	// fetch data from arduino nano
//	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
//	{
//		buffer = USART_ReceiveData(USART3);
////		USART_SendData(USART3, buffer);
//	}

	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		buffer = USART_ReceiveData(USART3);

		uartRxBuff[counter] = buffer;

		if(uartRxBuff[0] == 0x58 || uartRxBuff[0] == 0x59) //0x58 = X   0x59 = Y
		{
			rx_index = 4;
			counter++;
		}
		else if(uartRxBuff[0] == 0x43) //0x43 = C
		{
			rx_index = 5;
			counter++;
		}
//		else if(uartRxBuff[0] == 0x54)
//		{
//			rx_index = 2;
//			counter++;
//		}
		else if(uartRxBuff[0] == 0x50 || uartRxBuff[0] == 0x53)	// P = play		S = stop
		{
			rx_index = 1;
			counter++;
		}
		if(counter == rx_index)
		{
			if(uartRxBuff[0] == 0x58)
			{
				xCoor = 100 * (uartRxBuff[1] - 0x30) + 10 * (uartRxBuff[2] - 0x30) + (uartRxBuff[3] - 0x30);

			}
			else if(uartRxBuff[0] == 0x59)
			{
				yCoor = 100 * (uartRxBuff[1] - 0x30) + 10 * (uartRxBuff[2] - 0x30) + (uartRxBuff[3] - 0x30);
			}
//			else if(uartRxBuff[0] == 0x54)
//			{
//				obtain_ball = uartRxBuff[1];
//			}
			else if(uartRxBuff[0] == 0x43)
			{
				compassHeading = (float) 100 * (uartRxBuff[1] - 0x30) + 10 * (uartRxBuff[2] - 0x30) + 1 * (uartRxBuff[3] - 0x30) + (0.1 * (uartRxBuff[4] - 0x30));
				compassBase = compassHeading;
				compassHeading = compassHeading - SUDUTGAWANG - autosudut;

				if(compassHeading < 0)
				{
					compassHeading = compassHeading + 360;
				}
				else if(compassHeading > 360)
				{
					compassHeading = compassHeading - 360;
				}
			}
			else if (uartRxBuff[0] == 0x50 || uartRxBuff[0] == 0x53)
			{
				gameState = uartRxBuff[0];
			}

			counter = 0;

		}
		if((xCoor < 290 && xCoor > 265) && (yCoor < 280 && yCoor > 250))
		{
			obtain_ball = 1;
		}
		else {
			obtain_ball = 0;
		}
	}



//	// select compass data
//	if (flag =='C')
//	{
//		compassDerajat1 = buffer;
//
//		flag = 0;
//	}
//	// select X direction of accelerometer
//	else if(flag=='O')
//	{
//		compassDerajat2 = buffer;
//		compassHeading = (float)(compassDerajat1*100+compassDerajat2)/10;
//		compassHeading = compassHeading - SUDUTGAWANG - autosudut;
//
//		if(compassHeading < 0)
//		{
//			compassHeading = compassHeading + 360;
//		}
//		else if(compassHeading > 360)
//		{
//			compassHeading = compassHeading - 360;
//		}
//		flag = 0;
//	}
//
//
//
//
//	//camera
//	else if(flag=="X")
//	{
//
//	}
//	else if(flag=="Y")
//	{
//
//	}
////	else if(flag=='w')
////	{
////		xCoor = (fragX * 10)+ buffer;
////		flagX++;
////		flag = 0;
////	}
////
////	else if(flag=='q')
////	{
////		fragX = buffer;
////		flagX++;
////		flag = 0;
////	}
////	else if(flag=='r')
////	{
////		yCoor = (fragY * 10) + buffer;
////		flagY++;
////		flag = 0;
////	}
////	else if(flag=='e')
////	{
//////		fragY += buffer;
////		fragY = buffer;
////		flagY++;
////		flag = 0;
////	}
//
//
//
//
//
//
//
//	//kontrol manual lewat ros
//	else if(flag == '1' || flag == '2')
//	{
//		robotSelect = flag;
//		flag = 0;
//
//		//tanda mode kontrol manual
//		if(robotSelect == nomorRobot)
//			controlMode = 1;
//	}
//	else if(flag == 'w' && robotSelect == nomorRobot)
//	{
//		maju(10000);
//		flag = 0;
//	}
//	else if(flag == 'a' && robotSelect == nomorRobot)
//	{
//		kiri(10000);
//		flag = 0;
//	}
//	else if(flag == 's' && robotSelect == nomorRobot)
//	{
//		kanan(10000);
//		flag = 0;
//	}
//	else if(flag == 'd' && robotSelect == nomorRobot)
//	{
//		mundur(10000);
//		flag = 0;
//	}
//	else if(flag == 'q' && robotSelect == nomorRobot)
//	{
//		stop();
//		flag = 0;
//	}
//
//
//
//
//
//	else if(flag == '3' && robotSelect == nomorRobot) //tanda reset posisi encoder
//	{
//		resetInit();
//		current_y = 450 - 25;
//		flag = 0;
//	}
//	else if(flag == '4' && robotSelect == nomorRobot)
//	{
//		resetInit();
//		current_y = 112.5 + 25;
//		flag = 0;
//	}
//	else if(flag == '5' && robotSelect == nomorRobot)
//	{
//		resetInit();
//		current_y = 37.5 + 25;
//		flag = 0;
//	}
//	else // get flag for select data
//	{
//		flag = buffer;
//	}

}
