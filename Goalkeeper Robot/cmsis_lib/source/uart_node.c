
#include "uart_node.h"
uint8_t buffer;
/*
 * Variables
 */
char gameState;
extern float movY;
extern float movX;
int nodex1,nodey1,nodex2,nodey2,prevx1,prevy1;
extern int gotoxyFlag;
int strategy;
char flagNode;
char bufferNode;
float autosudut1 = 0;

//extern volatile int32_t rpm;

void init_USART(void)
{
		NVIC_InitTypeDef NVIC_InitStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		// sort out clocks
		RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOA, &GPIO_InitStructure);

		GPIO_PinAFConfig(GPIOA, GPIO_PinSource3 , GPIO_AF_USART2);
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource2 , GPIO_AF_USART2);
		// Initialize USART
	    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	     USART_DeInit(USART2);
	     USART_InitStructure.USART_BaudRate = 9600;
	     USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	     USART_InitStructure.USART_StopBits = USART_StopBits_1;
	     USART_InitStructure.USART_Parity = USART_Parity_No;
	     USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	     USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	     USART_Init(USART2,&USART_InitStructure);

	     USART_Cmd(USART2, ENABLE);

	     /* Here the USART2 receive interrupt is enabled
	      * and the interrupt controller is configured
	      * to jump to the USART2_IRQHandler() function
	      * if the USART2 receive interrupt occurs
	      */
	     USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART2 receive interrupt

	     NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;             // we want to configure the USART1 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;         // this sets the priority group of the USART1 interrupts
	     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // this sets the subpriority inside the group
	     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;           // the USART2 interrupts are globally enabled
	     NVIC_Init(&NVIC_InitStructure);                       // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	     // finally this2, E enables the complete USART2 peripheral
	     USART_Cmd(USART2,ENABLE);

}
char buff[5];
//char flag1 = 'a';
void USART2_IRQHandler(void){
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
	{
//		bufferNode = USART_ReceiveData(USART2);
		USART_SendData(USART2, buffer);
	}
//		if(flagNode=='x'){
//			nodex1 = bufferNode;
//			flagNode = 0;
//		}
//		else if(flagNode=='y'){
//			nodey1 = bufferNode;
//			flagNode = 0;
//		}
//		else if(flagNode=='X'){
//			nodex2 = bufferNode;
//			flagNode = 0;
//		}
//		else if(flagNode=='Y'){
//			nodey2 = bufferNode;
//			flagNode = 0;
//		}
//		else if(flagNode=='g'){
//			gameState = bufferNode;
//			flagNode = 0;
//		}
//		else if(flagNode=='s'){
//			strategy = bufferNode;
//			flagNode = 0;
//		}
//		else {
//			flagNode = bufferNode;
//		}
//		if(prevx1 != nodex1 || prevy1 != nodey1){
//			movX = nodex1;
//			movY = nodey1;
//			gotoxyFlag = 1;
//		}
//		prevx1 = nodex1;
//		prevy1 = nodey1;
//
//	}
		if(flag=='X')
			{
				ballXCoor = buffer;
				flag = 0;
			}

		//	else if(flag=='M')
		//		{
		//			fragX = buffer * 10;
		//			flagX++;
		//			flag = 0;
		//		}
		//
		//	else if(flag=='N')
		//		{
		//			fragX += buffer;
		//			flagX++;
		//			flag = 0;
		//		}
		//
		//	else if(flagX == 2)
		//		{
		//			ballXCoor = fragX;
		//			flagX = 0;
		//			flag = 0;
		//		}


			// select Y coordinate of camera servo
			else if(flag=='Y')
			{
				ballYCoor = buffer;
				flag = 0;
			}

		//	else if(flag=='V')
		//	{
		//		fragY = buffer * 10;
		//		flagY++;
		//		flag = 0;
		//	}
		//
		//	else if(flag=='F')
		//	{
		//		fragY += buffer;
		//		flagY++;
		//		flag = 0;
		//	}
		//
		//	else if(flagY == 2)
		//	{
		//		ballYCoor = fragY;
		//		flagY = 0;
		//		flag = 0;
		//	}

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
				compassHeading = compassHeading - SUDUTGAWANG - autosudut1;

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
		//	else if(flag=='L')
		//	{
		//		acceleroY = buffer;
		//		flag = 0;
		//	}
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


void getGamestate(void){
	   while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	   USART_SendData(USART2, 1);
}

void init_node(void)
{
	init_USART();
}
