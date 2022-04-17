#ifndef PWM_MOTOR_H_
#define PWM_MOTOR_H_

/**
 * Includes
 */
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stdint.h"
#include "stdbool.h"
#include <math.h>
#include "speed_control.h"

/**
 * Functions
 */
void TM_TIMER_Init();
void init_pwm();
void init_motor();
void motorDC(uint8_t channel, int32_t pwm);

/**
 * Defines
 */

#endif
