
#include "odometry.h"

/**
 * Variables
 */
extern int TM_INT_RE_Count;
extern int secondCount;
extern int thirdCount;
extern int fourthCount;
extern float compassHeading;
extern uint8_t acceleroX;
extern uint8_t acceleroY;

// odometers
int Lavg, Ravg, Favg, Bavg;			//average ticks
int Lticks, Rticks, Fticks, Bticks;	//ticks difference
int Lprev = 0, Rprev = 0;			//previous average ticks
int Fprev = 0, Bprev = 0;
float Lmeters, Rmeters;				//ticks to meters
float Fmeters, Bmeters;
float LRmeters, FBmeters;
float theta;                    	//heading in rad
float odomHeading;					//heading in degree
float X_pos, Y_pos;             	//position in meters
float X_poscmps, Y_poscmps;     	//position in meters with compass
float sudut;

// accelerometers
int Xaccel, Yaccel;					//acceleration
int Xvelo, Yvelo;					//velocity
int XveloPrev = 0, YveloPrev = 0;	//previous velocity
int Xdis, Ydis;						//distance
int Xpos, Ypos;						//position

/*
 * menghitung perputaran roda sebagai acuan posisi robot
 */
void odometers()
{
	// average left encoders and right encoders
			Lavg = -(TM_INT_RE_Count + secondCount)/2;
			Ravg = (thirdCount + fourthCount)/2;
			Favg = -(secondCount + thirdCount)/2;
			Bavg = (TM_INT_RE_Count + fourthCount)/2;

	// substract ticks with previous ticks
			Lticks = Lavg - Lprev;
			Rticks = Ravg - Rprev;
			Fticks = Favg - Fprev;
			Bticks = Bavg - Bprev;

	// save previous ticks
			Lprev = Lavg;
			Rprev = Ravg;
			Fprev = Favg;
			Bprev = Bavg;

	// convert ticks to meters
			Lmeters = (float)Lticks/LEFT_CLICKS_PER_METER;
			Rmeters = (float)Rticks/RIGHT_CLICKS_PER_METER;
			Fmeters = (float)Fticks/FRONT_CLICKS_PER_METER;
			Bmeters = (float)Bticks/BACK_CLICKS_PER_METER;

	// average left meters and right meters
			LRmeters = (Lmeters + Rmeters) / 2.0;

	// average front meters and back meters
			FBmeters = (Fmeters + Bmeters) / 2.0;

	// calculate heading
			theta += (Lmeters - Rmeters) / (WHEEL_BASE21 * 4);
//			theta += (Lmeters - Rmeters) / (WHEEL_BASE12 * 4);
//			theta += (Lmeters - Rmeters) / (WHEEL_BASE1 * 4);

	/* and clip the rotation to plus or minus 360 degrees */
//			theta -= (float)((int)(theta/TWOPI))*TWOPI;
			if(theta > 6.28318)
			{
				theta -= 6.28318;
			}
			else if(theta < 0)
			{
				theta += 6.28318;
			}

			sudut = theta * 180 / PI;

	// displacement to position with encoders heading
//			X_pos += (LRmeters * sin(theta) + FBmeters * (-cos(theta))) / 0.3258888276;
//			Y_pos += (LRmeters * cos(theta) + FBmeters * sin(theta)) / 0.3258888276;

			X_pos += (LRmeters * sin(theta) + FBmeters * cos(theta)) / 0.320451712; //0.3258888276
			Y_pos += (LRmeters * cos(theta) + FBmeters * (-sin(theta))) / 0.320451712;

	// displacement to position with compass heading
			X_poscmps += (LRmeters * sin((compassHeading*PI/180)) + FBmeters * cos(compassHeading*PI/180)) / 0.3258888276;
			Y_poscmps += (LRmeters * cos((compassHeading*PI/180)) + FBmeters * (-sin(compassHeading*PI/180))) / 0.3258888276;
}

/* ----------------------------------------------------------------------- */

/*
 * menghitung akselerasi sebagai acuan posisi robot
 */
void accelerometers()
{
	// get data from uart_camera
	Xaccel = acceleroX;
	Yaccel = acceleroY;

	// Acceleration to velocity
	Xvelo += Xaccel * 0.0042;
	Yvelo += Yaccel * 0.0042;

	// Displacement
	Xdis = (XveloPrev * 0.0042) + (0.5 * Xaccel * 0.00001764);
	Xdis = (XveloPrev * 0.0042) + (0.5 * Xaccel * 0.00001764);

	// save velocity form previous calculation
	XveloPrev = Xvelo;
	YveloPrev = Yvelo;

	// Displacement to position
	Xpos += (Xdis/XACCEL_POS_PER_METER) * sin((compassHeading*PI/180));
	Ypos += (Ydis/YACCEL_POS_PER_METER) * cos((compassHeading*PI/180));

}
