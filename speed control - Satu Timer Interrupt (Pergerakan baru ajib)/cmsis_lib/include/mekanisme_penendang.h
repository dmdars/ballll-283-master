#ifndef MEKANSME_PENENDANG_H_
#define MEKANISME_PENENDANG_H_

/**
 * Includes
 */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "tm_stm32f4_delay.h"
#include "stdbool.h"

/**
 * Functions
 */
void init_handle();
void init_penendang();
void handleRotateIn();
void leftHandleRotateIn();
void rightHandleRotateIn();
void handleRotateOut();
void handleOff();
void kickBall();
void chargeKick();
void kick(bool mode);
void tesKicker();
void tesHandle();

/**
 * Defines
 */
#define ON 1
#define OFF 0

#endif
