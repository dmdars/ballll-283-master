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
int counterSrf = 0;
int x, y;
int a, b;

int main(void)
{
	SystemInit();

//	lcd_init();
	TM_DELAY_Init();
//	init_lcdTimer();
//	lcd_clear();
//	init_mode();
	init_proximity();
	init_speed_control();
//	init_node();
	init_camera();
	init_extender();
//	init_handle();
//	init_penendang();
	TM_ADC_Init(ADC1, TM_ADC_Channel_4);
//	init_srf();
	Delayms(5000);

	fungsiautosudut();


    while(1)
    {
//    	keeper function
    	keeper();

    	readProxy();

//    	mechanical hydraulic
    	proxyExtender();
    }
}
