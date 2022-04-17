#ifndef LOCALIZATION_H_
#define LOCALIZATION_H_

/*
 * Update Note 8 Februari 2017
 * - heading merupakan arah robot menghadap yang di acukan saat robot baru dinyalakan dalam satuan derajat(0-360)
 * - perhitungan heading dimulai dengan menandakan mulainya perhitungan(startHeading =1) jika robot bergerak rotasi
 * yang dituliskan pada fungsi rotateClockWise dan rotateAntiClockWise di library pwm.h
 * - pergerakan selain rotate akan mematikan perhitungan(startHeading =0)
 * - perhitungan heading terdapat pada fungsi getHeading yang akan menghitung perbedaan count
 * yang dicatat pada fungsi TIM2_IRQHandler di library speedcontrol.
 *
 */

/* Luas Lapangan :
 * Real : 7,2 x 4,8
 * Virtual : 200 x 133
 * */

/**
 * Includes
 */
#include "speed_control.h"
#include "mekanisme_penendang.h"
#include "math.h"
#include "uart_node.h"
#include "uart_camera.h"
#include "tm_stm32f4_delay.h"
#include "lcd_16x2.h"
#include "stdbool.h"
#include "stdio.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"
#include "defines.h"
#include  "odometry.h"
#include "tm_stm32f4_adc.h"

/**
 * Functions
 */
void startOff();
void startMain(int refree, int arahGawang);
void outBall1();
void outBall2();
void keeper();
void penalty();
void penalty1();
void kickOff1();
void kickOff2();
void printData();
void init_mode();
void init_proximity();
void init_proximity2();
int getProxy();
void ballGet(void);
void getBallNew();
void getBall(void);
int PID(int param, int desire, int vMaks);
bool getGoalPos(void);
bool gotoHeadZero(void);
void getCoor(int headingProcess,int encoderLeft, int encoderRight);
void gotoXY(int coorX, int coorY, int head);
void gotoxyCoach();
void tesRotateMap(void);
void LCD_InitializeTimer();
void LCD_EnableTimerInterrupt();
void TIM5_IRQHandler();
void init_lcdTimer();
int hindar();
void readProxy();
int getProxy1();
void proxyExtender();
void readGP();
/**
 * Defines
 */
#define servoMid 112  // 142
#define goalX  -160
#define goalY  -3
#define IHead 0
#define DHead 0
#define PHead 1
#define ON 1
#define OFF 0
#define PI 3.14159265
#define TICK_PER_MM_LEFT  0.7857
#define TICK_PER_MM_RIGHT  0.7857
#define DIAMETER  200.0 // Distance between the 2 wheels (in millimeter)
#define TWOPI 6.2831853070
//Cepet
#define VXMAKS 120
#define VYJAUH 90
#define VYDEKAT 60
#define VYMAKS 90

//Normal
//#define VXMAKS 100
//#define VYJAUH 75
//#define VYDEKAT 50
//#define VYMAKS 75

////Pelan
//#define VXMAKS 60
//#define VYJAUH 45
//#define VYDEKAT 30
//#define VYMAKS 45

#define CYAN 10
#define MAGENTA 20

//gotoKickField
#define MINXKICK	0
#define MAKSXKICK	0
#define MINYKICK	0
#define MAKSYKICK	0

#endif

