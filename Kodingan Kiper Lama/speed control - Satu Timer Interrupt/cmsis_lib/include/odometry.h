#ifndef ODOMETRY_H_
#define ODOMETRY_H_


/**
 * Includes
 */
#include "speed_control.h"
#include "tm_stm32f4_rotary_encoder.h"
#include "uart_camera.h"
#include "math.h"

/**
 * Functions
 */
void odometers();
void accelerometers();

/**
 * Defines
 */
#define WHEEL_BASE21 49.8			//jarak roda kanan dengan roda kiri
#define LEFT_CLICKS_PER_METER 1		//banyak counter roda kiri
#define RIGHT_CLICKS_PER_METER 1	//banyak counter roda kanan
#define FRONT_CLICKS_PER_METER 1	//banyak counter roda kiri
#define BACK_CLICKS_PER_METER 1 	//banyak counter roda kanan
#define TWOPI 6.2831853070
#define XACCEL_POS_PER_METER 10
#define YACCEL_POS_PER_METER 10



#endif
