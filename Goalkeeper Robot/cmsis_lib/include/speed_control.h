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
#include "uart_node.h"
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
void TIM5_IRQHandler();
void init_speed_control(void);
void motorSpeed(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed);
void maju(int32_t pwm);
void mundur(int32_t pwm);
void kanan(int32_t pwm);
void kiri(int32_t pwm);
void timurLaut(int32_t pwm);
void tenggara(int32_t pwm);
void baratLaut(int32_t pwm);
void baratDaya(int32_t pwm);
void rotateClockWise(int32_t pwm);
void rotateAntiClockWise(int32_t pwm);
void stop();
void tesMotor(int rpm);

/**
 * Defines
 */
#define PI 3.14159265
#define KP 5
#define KD 10
#define KI 0

#endif //SPEED_CONTROL_H_
