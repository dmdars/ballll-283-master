
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
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
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 * inisialisasi penendang
 */
void init_penendang(){
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*
 * gerakan handle masuk kearah robot
 */


//inisiasi sensor time of flight



void handleRotateIn(){
	//kiri
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);

	//right
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

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
	//kiri
	GPIO_SetBits(GPIOB,GPIO_Pin_4);
	GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);

	//right
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
}

/*
 * handle berhenti
 */
void handleOff(){
	//kiri
	GPIO_ResetBits(GPIOB,GPIO_Pin_4);
	GPIO_SetBits(GPIOB,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);

	//right
	GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	GPIO_SetBits(GPIOB,GPIO_Pin_8);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
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
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	}
	else{
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
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
