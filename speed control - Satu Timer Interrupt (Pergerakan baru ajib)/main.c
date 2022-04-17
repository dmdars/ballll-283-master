#include "lcd_16x2.h"
#include "pwm_motor.h"
#include "stdio.h"
#include "speed_control.h"
#include "uart_node.h"
#include "uart_camera.h"
#include "localization.h"
#include "mekanisme_penendang.h"
#include <math.h>


/* Luas Lapangan :
 * Real : 7,2 x 4,8
 * Virtual : 200 x 133
 * */

/*
 * Variables
 */
volatile uint8_t is_tra_done_changeflag;
extern volatile uint8_t is_tra_speed_x_done;
extern volatile uint8_t is_tra_speed_y_done;
extern volatile uint8_t is_tra_w_done;
extern uint8_t ballYCoor;
extern float Y_pos, Y_poscmps;
extern float X_pos, X_poscmps;
extern float theta;
extern float sudut;
int counterSrf = 0;
int w, x, y, z;
extern int move_type;
int type = 0;
double inputx = 50.0;
double inputy = 0.0;
double inputw = 0.0;
double sudutw = 0.707106781;

extern uint8_t robotSelect;
extern uint8_t nomorRobot;
extern uint8_t controlMode;


int main(void)
{
	SystemInit();

	TM_DELAY_Init();

	init_node();
	init_camera();
	init_handle();
//	init_proximity();
	init_penendang();
	init_speed_control();

//	controlMode = 1;
//	stop();
	controlMode = 0;
//	init_mode();
//	init_srf();

//	Delayms(5000);

//	fungsiautosudut();

//	resetInit();
//	gerakCobaSabtu(150,150);
//	controlMode = 0;
//	motorSpeed(0,0,0,0);

//	Delayms(1000);

//	if(GPIO_ReadInputDataBit(GPIOE,GPIO_PIN_8) == 1){
////		while(1)
////		{
////			penalty();
////		}
//	}
//	else{
//		maju(150);
//		Delayms(3000);
//	}

//	for(int i = 0; i < 5; i++)
//	{
//		traInit(0, 100, 0, 0);
//		Delayms(3000);
//		traInit(0, 0, 0, 0);
//		Delayms(3000);
//	}

//	traInit(-100, -100, 0, 0 , 0.100);//ngatur PID di speed_control.h
//	Delayms(1000);
	traYInit(1000,10,0,3);
//	traXInit(1000,10,0,3);
//	traYInit(0,0,0);
//	traXInit(0,0,0);
//	calcTimingY(9000.0,2600.0,1600.0,3200.0);
//	Delayms(3000);
//	resetInit();
//	Delayms(1000);
//	traInit(0,0,0,0,0);


//	Delayms(1000);
//	resetInit();
//	traInit(0, 0, 0, 0, 0);
	//nendang di pin 1c
//	traInit(0, 0, 180, 0);
//	Delayms(3000);

//	traInit(0, 0, 315, 0);
//	Delayms(2000);
//	traInit(0, 200, 315, 0);

//	controlMode = 1;

//	stop();

//	handleRotateIn();
//	motorDC(2,100);
    while(1)
    {

    	if (is_tra_done_changeflag == 0)
    	{
    		if(is_tra_speed_y_done == 1 ){
    			type = 1;
//    			traInit(0,0,0,0,0,3);
//    			resetInit();
//    			Delayms(100);
    			traXInit(-1000,10,0,3);
    			is_tra_w_done = 0;
    		}
//    		if(is_tra_speed_x_done == 1){
////    			resetInit();
//    			break;
//    		}
    	}
//			else if (move_type == 1)
//				{
//					traXInit(1000,10,0);
//					is_tra_done_changeflag = 0;
//					move_type=0;
//				}
//    		}


//    	else if (i)
//    	{
//
//    	}
//    	getProxy();
//    	GPIO_SetBits(GPIOC,GPIO_Pin_1);

//    	handleRotateIn();
//    	Delayms(1000);
//    	handleOff();
//    	Delayms(1000);
//    	traInit(0, -100, 0,0);
//    	ballGet();
//    	traInit(0,100,0,0);
//    	Delayms(3000);
//    	stop();
//    	Delayms(3000);

//		maju(15000);
//
//    	if(ballGet())
//    	{
//    		handleOff(); cx
//    		Delayms(500);
//    		handleRotateOut();
//    		Delayms(100);
//    	}
//    	else {
//    		handleRotateIn();
//		}


//    	moveInput();
//    	kickBall();
//    	maju(100);
//    	Delayms(1000);
    }
}

