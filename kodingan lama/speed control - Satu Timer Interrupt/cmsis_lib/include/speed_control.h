#ifndef SPEED_CONTROL_H_
#define SPEED_CONTROL_H_

/**
 * Includes
 */
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "tm_stm32f4_rotary_encoder.h"
#include "pwm_motor.h"
#include "stdint.h"
#include <math.h>
//#include <cmath>
#include "uart_node.h"
#include "uart_camera.h"
#include "localization.h"
#include "tm_stm32f4_hcsr04.h"
#include "tm_stm32f4_gpio.h"
#include "odometry.h"

/**
 * Functions
 */
void InitializeTimer();
void EnableTimerInterrupt();
void TIM2_IRQHandler();
//void TIM5_IRQHandler();
void init_speed_control(void);

void odoInit();
void traSinInit();
void traSinOff();
void traInit(double x, double y, double w, double time);
void traXInit(double x, double time_x);
void traYInit(double y, double time_y);
void traWInit(double w, double time_w);
void pidInit();

void calculatePosition();
void calculateTrj();
void calculatePID();

void motorSpeed(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed);
void maju(int32_t speed);
void mundur(int32_t speed);
void kanan(int32_t speed);
void kiri(int32_t speed);
void timurLaut(int32_t pwm);
void tenggara(int32_t pwm);
void baratLaut(int32_t pwm);
void baratDaya(int32_t pwm);
void rotateClockWise(int32_t pwm);
void rotateAntiClockWise(int32_t pwm);
void stop();
void tesMotor(int rpm);

void moveInput();

/**
 * Defines
 */
#define PI 3.14159265
//5 10 0    tuning yang dulu
//1.5 0 1   tuning gagal
//100 60 10 pg45
//10  90 10 pg36
#define KP0 15 //15
#define KD0 20 //30
#define KI0 10 //15


#define KP_W 10
#define KI_W 0
#define KD_W 5

#define KP1 1.5
#define KD1 0
#define KI1 1

#define KPBall 2000;
#define KDBall 0;
#define KIBall 0;

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

#endif //SPEED_CONTROL_H_

extern int gotoBall;
