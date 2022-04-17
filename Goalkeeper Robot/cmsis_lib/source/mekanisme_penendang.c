
#include "mekanisme_penendang.h"

/*
 * Variables
 */

/*
 * inisialisasi handle
 */
void init_handle(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*
 * inisialisasi penendang
 */
void init_penendang(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*
 * gerakan handle masuk kearah robot
 */
void handleRotateIn(){
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}
int bolaketemu=0;
int posisibola;


void init_extender()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);// tangan kiri
	GPIO_SetBits(GPIOD,GPIO_Pin_9);// tangan kanan
	GPIO_SetBits(GPIOD,GPIO_Pin_10);// tangan atas
}
void rightExtender()
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
}

void leftExtender()
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);// tangan kiri
}
void upExtender()
{
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);// tangan kiri
}
void offExtender()
{
	GPIO_SetBits(GPIOD,GPIO_Pin_8);// tangan kiri
	GPIO_SetBits(GPIOD,GPIO_Pin_9);// tangan kiri
	GPIO_SetBits(GPIOD,GPIO_Pin_10);// tangan kiri
}


//void bergerak()
//{
//	while(1)
//	{
//		if (posisibola<=50 && posisibola>0)
//		{
//			traInit(posisibola, 0, 180, 0);
//			tangankiri();
//		}
//		else if (posisibola>=51 && posisibola<=100)
//		{
//			traInit(posisibola, 0, 0, 0);
//			tangankanan();
//		}
//		else
//			{
//			tanganmati();
//			}
//	}
//}
/*
 * gerakan handle kiri masuk
 */
void leftHandleRotateIn()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

/*
 * gerakan handle kanan masuk
 */
void rightHandleRotateIn()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
}

/*
 * gerakan handle keluar robot
 */
void handleRotateOut(){
	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
}

/*
 * handle berhenti
 */
void handleOff(){
//	GPIO_ResetBits(GPIOB,GPIO_Pin_0);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
//	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

/*
 * gerakan solenoid tendang bola
 */
void kickBall(){
	kick(ON);
	Delayms(1000);
//	kick(OFF);
//	Delayms(50);
}

/*
 * fungsi tendang
 */
void kick(bool mode){
	if(mode == true){
		GPIO_SetBits(GPIOE,GPIO_Pin_11);
	}
	else{
		GPIO_ResetBits(GPIOE,GPIO_Pin_11);
	}
}

/*
 * tes semua gerakan handle
 */
void tesHandle(){
	handleRotateIn();
	Delayms(2000);
	handleRotateOut();
	Delayms(2000);
	leftHandleRotateIn();
	Delayms(2000);
	rightHandleRotateIn();
	Delayms(2000);
	handleOff();
	Delayms(2000);
}
