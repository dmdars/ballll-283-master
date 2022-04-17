#ifndef UART_NODE_H_INCLUDED
#define UART_NODE_H_INCLUDED


/**
 * Includes
 */
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_usart.h"
#include "misc.h"
#include <stdint.h>
#include "localization.h"

/**
 * Functions
 */
void init_node(void);
void USART2_IRQHandler(void);
void getGamestate(void);

/**
 * Defines
 */

#endif // UART_NODE_H_INCLUDED
