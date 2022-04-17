#include "lcd_16x2.h"
#include "pwm_motor.h"
#include "stdio.h"
#include "speed_control.h"
#include "uart_node.h"
#include "uart_camera.h"
#include "localization.h"
#include "mekanisme_penendang.h"



/* Luas Lapangan :
 * Real : 7,2 x 4,8
 * Virtual : 200 x 133
 * */

/*
 * Variables
 */
extern uint8_t ballYCoor;
extern float Y_pos, Y_poscmps;
extern float X_pos, X_poscmps;
extern float theta;
extern float sudut;
int counterSrf = 0;
int w, x, y, z;
int flagJumat = 0;
double inputx = 50.0;
double inputy = 0.0;
double inputw = 0.0;
double sudutw = 0.707106781;

int main(void)
{
	SystemInit();

//	lcd_init();
	TM_DELAY_Init();
//	init_lcdTimer();
//	lcd_clear();
//	init_mode();
	init_camera();
	init_proximity();
	init_speed_control();
//	init_extender();
//	init_node();
//	init_USART();
//	init_handle();
//	init_penendang();
//	init_srf();
	motorSpeed(0,0,0,0);
	Delayms(5000);

//	kiri(60);
//	Delayms(1500);
//	kanan(60);
//	Delayms(1500);
//	kiri(60);
//	Delayms(1500);
//	kanan(60);
//	flagJumat = 0;
//	Delayms(500);

//	stop();
    while(1)
    {
//    	motorSpeed(0,0,0,0);
//    	flagJumat = 1;
//    	Delayms(500);
//    	keeper();

//    	readProxy();
//    	keeperProxy();
//    	tesMotor(50);
//    	moveInput();
    	tesMotor(50);

//    	motorSpeed(0,0,0,0);

//    	if(Y_pos < 2000.0)
//    		maju(30);
//    	else
//    		stop();
//    	stop();


//    	if(compassHeading >= 179 && compassHeading <= 181)
//    	{
//    		f = 1;
//    	}
//    	if(f == 1)
//    	{
//    		mundur(PID(Y_poscmps, -2000.0, 150));
//			kiri(PID(X_poscmps, 1000, 150));
//    	}
//    	else
//		{
//			rotateAntiClockWise(50);
//		}

//    	maju(30);
//    	maju(PID(Y_poscmps, -2000.0, 50));

//    	maju(30);

//    	w = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
//    	x = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
//    	y = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);
//    	if (z==0)
//    	{
//        	GPIO_ResetBits(GPIOA,GPIO_Pin_1);// tangan kiri
//        	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
//        	for(int i=0; i<100 ;i++)
//
//        	GPIO_SetBits(GPIOA,GPIO_Pin_1);// tangan kiri
//        	GPIO_SetBits(GPIOA,GPIO_Pin_0);
//    	}

//    	        //    	startMain(OFF,OFF);

//    	motor = 0;
//    	maju(PID(Y_poscmps, 200, 10));

//    	kiri(10);
//    	Delay(100);
//    	kiri(30);
//    	Delay(100);
//    	kiri(50);
//    	Delay(100);
//
//    	Delay(1000);

//    	if(y == 1)
//    		kanan(150);
//    	else
//    		stop();
//

//    	tesSRF();

//    	if(y == 0)
//    		stop();
//    	else
//    		kanan(150);

//    	maju(10);
//		Delay(200);
//		mundur(10);
//		Delay(200);

//    	Delay(20000);

//    	kiri(150);

//    	handleRotateIn();

//    	keeperProx(150);

//    	if(ballFound == 1)
//    		maju(30);
//    	else {
//			stop();
//		}

//    	kiri(150);

    }
}
