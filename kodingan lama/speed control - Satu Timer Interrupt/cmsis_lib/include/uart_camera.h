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

/*
 * sudut untuk menjadikan compassHeading bernilai 0
 */
#define SUDUTGAWANG 0

/**
 * Variables
 */
uint8_t ballXCoor;
uint8_t ballYCoor;
uint16_t xCoor;
uint16_t yCoor;
uint8_t acceleroX;
uint8_t acceleroY;
uint8_t compassDerajat1;
uint8_t compassDerajat2;
uint8_t ballFound;
uint8_t fragX;
uint8_t fragY;
uint8_t flagX;
uint8_t flagY;

int obtain_ball;
char gameState;
uint8_t robotSelect;
uint8_t nomorRobot;
uint8_t controlMode;


float compassHeading;
char flag;
//int fragX, fragY, flagX = 0, flagY = 0;

#endif
