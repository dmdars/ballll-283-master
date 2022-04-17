#ifndef UART_CAMERA_H_INCLUDED
#define	UART_CAMERA_H_INCLUDED

/**
 * Includes
 */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"

/**
 * Functions
 */
void init_camera();
void USART3_IRQHandler(void);
void fungsiautosudut();

/**
 * Defines
 */
/*
 * sudut untuk menjadikan compassHeading bernilai 0
 */
//Cyan
//#define SUDUTGAWANG 142
//Magenta
//#define SUDUTGAWANG -38
//Default
#define SUDUTGAWANG 77

/**
 * Variables
 */
uint8_t ballXCoor;
uint8_t ballYCoor;
uint8_t acceleroX;
uint8_t acceleroY;
uint8_t compassDerajat1;
uint8_t compassDerajat2;
uint8_t ballFound;
float compassHeading;
char flag;

#endif
