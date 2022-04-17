
#include "localization.h"

/*
 * Variables
 */
char lcd[16];
int team=CYAN;
extern int nodex1,nodey1,nodex2,nodey2,prevx1,prevy1;
int gotoHead;
char prevGameState;
float KaP = 1;
float KaD = 2;
float KaI = 0;
float ball,errorBall,derivErrorBall,intErrorBall,prevErrorBall;
float ballMaju,errorBallMaju,derivErrorBallMaju,intErrorBallMaju,prevErrorBallMaju;
float heading;
float head,errorHead,derivErrorHead,intErrorHead,prevErrorHead;
int headGoal;
extern int strategy;
float theta = 0;
float left_mm;
float right_mm;
int headCount;
float heading, mov,movPrev, movX, movY;
int movCompass;
extern float compassHeading;
extern char gameState;
extern uint8_t ballFound;
extern float movY;
extern float movX;
extern int movCompass;
extern int x;
extern int y;
extern int a;
extern int b;
extern float X_pos, Y_pos;
int counterDelay;
int rotateCount;
int icount=1;
int flagKickoff=0;
int prevCompass;
int sudutKanan = 315;
int sudutKiri = 45;
int flagmundur = 0;
int flagStrategy = 0;
int rpmKanan;
int rpmKiri;
int handle = 0;
int strategyCount;
int keeperFlag = 0;
int keeperCount = 0;
int wasit = 1;
int gotoxyFlag;
int hindarState = 0;

int proxyKiri1 = 1;
int proxyKiri2 = 1;
int proxyKiri3 = 1;

int proxyKanan1 = 1;
int proxyKanan2 = 1;
int proxyKanan3 = 1;

int proxyAtas1 = 1;
int proxyAtas2 = 1;
int proxyAtas3 = 1;
int proxySampingKiri =1;
int proxySampingKanan = 1;
int proxyTengah = 1;

int proxyCount = 0;
int proxyCount1 = 0;
int pflag1 = 0;
int pflag2 = 0;


double GPDistance = 0;
/**
 * Start main merupakan fungsi utama program robot
 * fungsi ini akan mencari bola, dan gawang
 * @param refree untuk menentukan akan menggunakan refree atau tidak
 * @param mode untuk mengarahkan ke area musuh saja atau mengarahkan ke gawang musuh
 *
 */

void startOff(){
	if(ballYCoor>30){
		getBallNew();
		if(ballYCoor<=45){
			handleRotateIn();
			handle = 1;
		}
		else{
			handleOff();
		}
	}
	else{
		if (gotoHeadZero())
		{
			if (handle == 1)
			{
				maju(40);
				kickBall();
			}
		}
//			}
	}
}

void startMain(int refree, int arahGawang){

	if(refree == ON ){
		if (gameState == 's') {
			if(team == CYAN){
				if(prevGameState == 'k'|| prevGameState ==  'f'||prevGameState ==  'g'||
						prevGameState == 't'|| prevGameState == 'c'||prevGameState == 'p'
								||prevGameState =='a'||prevGameState =='o'||prevGameState =='r'||prevGameState =='y')
				{
					Delayms(5000);
				}
			}
			else{
				if(prevGameState == 'K'|| prevGameState ==  'F'||prevGameState ==  'G'||
						prevGameState == 'T'|| prevGameState == 'C'||prevGameState == 'P'
								||prevGameState =='A'||prevGameState =='O'||prevGameState =='R'||prevGameState =='Y')

				{
					Delayms(5000);
				}
			}
			prevGameState = 's';
			if(ballYCoor>35){
				getBallNew();
				if(ballYCoor<=45){
					handleRotateIn();
					handle = 1;
				}
				else{
					handleOff();
				}
			}
			else{
				if(arahGawang == ON){
					if (getGoalPos())
					{
						motorSpeed(0,0,0,0);
						Delayms(100);
						handleOff();
						maju(100);
						Delayms(1000);
						kickBall();
					}
				}
				else{
					if (gotoHeadZero())
					{
						motorSpeed(0,0,0,0);
						Delayms(100);
						handleOff();
						maju(100);
						Delayms(1000);
						kickBall();
					}
				}
			}
		}
		else{
			if(prevGameState == 's'){
				movX = nodex2;
				movY = nodey2;
			}
			else{
				gotoxyCoach();
			}
			if(gameState != 's'){
				prevGameState = gameState;
			}
		}
	}
	// refree off
	else{
//		wasit = 0;
		if(ballYCoor>35){
			getBallNew();
			if(ballYCoor<=45){
				handleRotateIn();
				handle = 1;
			}
			else{
				handleOff();
			}
		}
		else{
//			if (gotoHeadZero())
//			{
//				motorSpeed(0,0,0,0);
//				Delayms(300);
				handleOff();
//				maju(100);
//				Delayms(1000);
				kickBall();
//			}
		}
	}
}

/*
 * outBall1 merupakan fungsi untuk mengambil posisi sebelum kita throw in
 * fungsi ini akan menggerakkan robot mendekati bola dengan jarak
 */
void outBall1()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 0 && compassHeading <= 180)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,40));
				kanan(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else if(compassHeading > 180 && compassHeading < 360)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,40));
				kiri(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if((compassHeading > 42 && compassHeading < 48) || (compassHeading > 312 && compassHeading < 318))
			{
				int speedKiri = PID(ballXCoor,servoMid,30);
				if(speedKiri <= 10 && speedKiri > 0)
				{
					speedKiri = 10;
				}
				else if(speedKiri > -10 && speedKiri < 0)
				{
					speedKiri = -10;
				}
				kiri(speedKiri);
				if((ballXCoor-servoMid) > -2 && (ballXCoor - servoMid) < 2)
				{
					motorSpeed(0,0,0,0);
				}
			}
			else {
				if(prevCompass >= 0 && prevCompass <= 180)
				{
					int rotateSpeed = PID(compassHeading,sudutKanan,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
				else if(prevCompass > 180 && prevCompass < 360)
				{
					int rotateSpeed = PID(compassHeading,sudutKiri,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
}

/*
 * outBall2 merupakan fungsi untuk mengambil posisi sebelum musuh throw in
 * fungsi ini akan menggerakkan robot mendekati bola dengan jarak 3 meter
 */
void outBall2()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 0 && compassHeading <= 90)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 360)
			{
				prevCompass = compassHeading;
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(1000);
				maju(40);
				Delayms(1000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(40);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if((compassHeading > 42 && compassHeading < 48) || (compassHeading > 312 && compassHeading < 318))
			{
				if (flagmundur == 0)
				{
					int speedKiri = PID(ballXCoor,servoMid,30);
					if(speedKiri <= 10 && speedKiri > 0)
					{
						speedKiri = 10;
					}
					else if(speedKiri > -10 && speedKiri < 0)
					{
						speedKiri = -10;
					}
					kiri(speedKiri);
					if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
					{
						flagmundur = 1;
					}
				}
				else if (flagmundur == 1)
				{
					mundur(40);
					if (ballYCoor > 110)
					{
						motorSpeed(0,0,0,0);
					}
				}
			}
			else {
				if(prevCompass >= 0 && prevCompass <= 180)
				{
					int rotateSpeed = PID(compassHeading,sudutKanan,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
				else if(prevCompass > 180 && prevCompass < 360)
				{
					int rotateSpeed = PID(compassHeading,sudutKiri,30);
					if(rotateSpeed <= 10 && rotateSpeed > 0)
					{
						rotateSpeed = 10;
					}
					else if(rotateSpeed > -10 && rotateSpeed < 0)
					{
						rotateSpeed = -10;
					}
					motorSpeed(rotateSpeed,0,0,rotateSpeed);
				}
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(40);
		}
		else
		{
			if (flagmundur == 0)
			{
				getBallNew();
			}
		}
	}
}

void keeper()
{
	if(getProxy() == 1)
	{
		keeperFlag = 1;
		keeperCount++;
		Delayms(10);
		if(keeperCount == 20){
			keeperFlag = 0;
		}
	}
	else if(getProxy() == 2)
	{
		keeperFlag = 2;
		keeperCount++;
		Delayms(10);
		if(keeperCount == 20){
			keeperFlag = 0;
		}
	}
	else{}
	if(keeperFlag == 1)
	{
		if(ballFound == 1)
		{
			if(ballXCoor <= 138){
				keeperFlag = 0;
			}
			motorSpeed(0,0,0,0);

		}
		else
		{
			kiri(50);
		}
	}
	else if(keeperFlag == 2)
	{
		if(ballFound == 1)
		{
			if(ballXCoor >= 146){
				keeperFlag = 0;
			}
			motorSpeed(0,0,0,0);
		}
		else
		{
			kanan(50);
		}
	}
	else if(keeperFlag == 0)
	{
		keeperCount = 0;
		if(ballXCoor <= 138 && ballFound == 1)
		{

			if(ballXCoor >= servoMid - 10 && ballXCoor <= servoMid + 10)
			{
				motorSpeed(0,0,0,0);
			}
			else{
				kiri(100);
			}
		}
		else if(ballXCoor >= 146 && ballFound == 1)
		{

			if(ballXCoor >= servoMid - 10 && ballXCoor <= servoMid + 10)
			{
				motorSpeed(0,0,0,0);
			}
			else{
				kanan(100);
			}
		}
		if(ballFound == 0)
		{
			if(flagStrategy == 0)
			{
				if(compassHeading >= 320 && compassHeading < 360)
				{
					rotateClockWise(10);
					if(compassHeading <= 320 && compassHeading > 300)
					{
						flagStrategy = 1;
					}
				}
				else if(compassHeading < 300 && compassHeading >= 0)
				{
					rotateAntiClockWise(10);
					if(compassHeading <= 320 && compassHeading > 300)
					{
						flagStrategy = 1;
					}
				}
			}
			if(flagStrategy == 1)
			{
				if (GPDistance == 60)
				{
					motorSpeed(0,0,0,0);
					flagStrategy = 2;
				}
				else if (GPDistance > 60)
				{
					mundur(50);
				}
				else if (GPDistance < 60)
				{
					maju(50);
				}
			}
			if(flagStrategy == 2 )
			{
				if (proxyTengah == 1)
				{
					kanan(50);
					Delayms(500);
				}
				else if (proxyTengah == 0)
				{
					kiri(20);
					Delayms(500);
					motorSpeed(0,0,0,0);
				}
			}
		}
	}
}



/*
 * penalty merupakan fungsi untuk mengambil posisi sebelum penalty
 * fungsi ini akan menggerakkan robot mendekati bola
 */
void penalty(){
	if(ballYCoor >= 40){
		maju(30);
		//handleOn();
	}
	else{
			motorSpeed(0,0,0,0);
			Delayms(500);
			kickBall();
	}
}

void penalty1()
{
	if(ballYCoor >= 40){
		maju(120);
		//handleOn();
	}
	else{
			motorSpeed(0,0,0,0);
			Delayms(500);
	}
}

void kickOff1()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 315 && compassHeading < 360)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 315)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else if(compassHeading > 0 && compassHeading <= 45)
			{
				rotateClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading > 45 && compassHeading <= 90)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 187 && compassHeading < 193)
			{
				int speedKiri = PID(ballXCoor,servoMid,30);
				if(speedKiri <= 10 && speedKiri > 0)
				{
					speedKiri = 10;
				}
				else if(speedKiri > -10 && speedKiri < 0)
				{
					speedKiri = -10;
				}
				kiri(speedKiri);
				if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
				{
					motorSpeed(0,0,0,0);
				}
			}
			else {
				int rotateSpeed = PID(compassHeading,190,30);
				if(rotateSpeed <= 10 && rotateSpeed > 0)
				{
					rotateSpeed = 10;
				}
				else if(rotateSpeed > -10 && rotateSpeed < 0)
				{
					rotateSpeed = -10;
				}
				motorSpeed(rotateSpeed,0,0,rotateSpeed);
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
}

void kickOff2()
{
	if(flagKickoff == 0){
		if(ballYCoor >= 60 && ballYCoor <= 65 && ballFound == 1){
			if(compassHeading >= 315 && compassHeading < 360)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading >= 270 && compassHeading < 315)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kiri(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else if(compassHeading > 0 && compassHeading <= 45)
			{
				rotateClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(3500);
				maju(40);
				Delayms(5000);
				flagKickoff = 1;
			}
			else if(compassHeading > 45 && compassHeading <= 90)
			{
				rotateAntiClockWise(PID(compassHeading,0,50));
				kanan(40);
				Delayms(2500);
				maju(40);
				Delayms(4000);
				flagKickoff = 1;
			}
			else{
				flagKickoff = 1;
			}
		}
		else if(ballYCoor < 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			getBallNew();
		}
	}
	else if (flagKickoff == 1) {
		if(ballYCoor > 55 && ballYCoor < 70 && ballFound == 1){
			if(compassHeading > 187 && compassHeading < 193)
			{
				if (flagmundur == 0)
				{
					int speedKiri = PID(ballXCoor,servoMid,30);
					if(speedKiri <= 10 && speedKiri > 0)
					{
						speedKiri = 10;
					}
					else if(speedKiri > -10 && speedKiri < 0)
					{
						speedKiri = -10;
					}
					kiri(speedKiri);
					if((ballXCoor-servoMid) > -5 && (ballXCoor - servoMid) < 5)
					{
						flagmundur = 1;
					}
				}
				else if (flagmundur == 1)
				{
					mundur(40);
					if (ballYCoor > 95)
					{
						motorSpeed(0,0,0,0);
					}
				}
			}
			else {
				int rotateSpeed = PID(compassHeading,190,30);
				if(rotateSpeed <= 10 && rotateSpeed > 0)
				{
					rotateSpeed = 10;
				}
				else if(rotateSpeed > -10 && rotateSpeed < 0)
				{
					rotateSpeed = -10;
				}
				motorSpeed(rotateSpeed,0,0,rotateSpeed);
			}
		}
		else if(ballYCoor <= 55 && ballFound == 1)
		{
			mundur(30);
		}
		else
		{
			if (flagmundur == 0)
			{
				getBallNew();
			}
		}
	}
}

void printData(){
	lcd_clear();
//	int xCoor,yCoor;
//	xCoor = movX;
//	yCoor = movY;
	sprintf(lcd,"H:%f Y:%d X:%d ",compassHeading,ballYCoor,ballXCoor);
	lcd_putsf(0,0,lcd);
	sprintf(lcd, "G:%c y:%d x:%d",gameState,ballYCoor,ballXCoor);
	lcd_putsf(0,1,lcd);
}


void init_mode(){
	GPIO_InitTypeDef gpio_init;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	gpio_init.GPIO_Pin  = GPIO_PIN_8;
	gpio_init.GPIO_Mode = GPIO_Mode_OUT;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);
}

/*
 *
 */
void init_proximity()
{
	GPIO_InitTypeDef gpio_init;
//	EXTI_InitTypeDef EXTI_InitStruct;
//	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	gpio_init.GPIO_Pin  = GPIO_PIN_1;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_2;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_3;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_4;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_5;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_6;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_7;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_8;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_9;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_10;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);

	gpio_init.GPIO_Pin  = GPIO_PIN_11;
	gpio_init.GPIO_Mode = GPIO_Mode_IN;
	gpio_init.GPIO_Speed = GPIO_Speed_100MHz;
	gpio_init.GPIO_OType = GPIO_OType_PP;
	gpio_init.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOC, &gpio_init);
//
//	/* Connect pin to interrupt */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
//
//
//	/* Configure external interrupt */
//	EXTI_InitStruct.EXTI_Line = EXTI_Line13;
//	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
//	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
//	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_Init(&EXTI_InitStruct);
//
//	/* Add interrupt to NVIC */
//	NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x03;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
//
}



//#ifdef ACTIVATEPROXY1
//void EXTI15_10_IRQHandler(void) {
//	if (EXTI_GetITStatus(EXTI_Line13) != RESET) {
//		x = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13);
//		y = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_14);
//		/* Clear interrupt bit */
//		EXTI_ClearITPendingBit(EXTI_Line1);
//	}
//}
//
//#endif


/*
 * getProxy merupakan fungsi untuk menghasilkan nilai 0 atau 1
 * dari inputan proximity
 */
int getProxy1()
{
	if (/*proxyKiri1 == 0 ||*/ proxyKiri2 == 0 || proxyKiri3 == 0)
	{
		return 1;
	}
	else if (proxyAtas1 == 0 || proxyAtas2 == 0 || proxyAtas3 == 0)
	{
		return 2;
	}
	else if (proxyKanan1 == 0 || proxyKanan2 == 0 || proxyKanan3 == 0)
	{
		return 3;
	}
	return 0;
}
int getProxy()
{
	if (proxySampingKanan == 0)
	{
		return 1;
	}
	else if (proxySampingKiri == 0)
	{
		return 2;
	}
	else if (keeperFlag == 0)
	{
		return 3;
	}
	return 0;
}
void readProxy()
{
//	proxyKiri1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1);
	proxyKiri2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2);
	proxyKiri3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3);

	proxyAtas1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4);
	proxyAtas2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
	proxyAtas3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6);

	proxyKanan1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);
	proxyKanan2 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	proxyKanan3 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);

	proxySampingKanan = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10);
	proxySampingKiri = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11);

	readGP();


//	gpValue = TM_ADC_Read(ADC1, ADC_Channel_10);

}
uint8_t gpread = 0;
int flaggp = 0;
void readGP()
{
	gpread = TM_ADC_Read(ADC1, TM_ADC_Channel_4);
	if(gpread != 0)
		flaggp = 1;
	else
		flaggp = 0;

	if(gpread >= 2500 && gpread<=2700)
	{
		GPDistance=60;
	}
	else if (gpread <=2500)
	{
		GPDistance=100;
	}
	else if (gpread >=2700)
		{
			GPDistance=0;
		}

//	if(gpread >=1437 && gpread<= 1511)
//		GPDistance = 50;
//	GPDistance = 10650.08 * pow(gpread, -0.935) - 10;
//	GPDistance = 9462 / (gpread - 16.92);
//	GPDistance = 5461 / (gpread - 17) - 2 ;
//	GPDistance = 12343.85 * pow(gpread, -1.15);

//	double distance;
//	for(int gpiter = 0; gpiter < 10; gpiter++)
//	{
//		distance = (1834.9953/gpread) - 6.2304;
//		GPDistance += distance;
//	}
//
//	GPDistance = GPDistance / 10;

	Delayms(100);
}
void proxyExtender()
{

	if(getProxy1()==1|| pflag1 ==1)
	{
		pflag1 = 1;
		if (proxyCount < 10)
		{
			leftExtender();
			Delayms(10);
			proxyCount++;
		}
		if (proxyCount==10)
		{
			if(proxyCount1 < 10)
			{
				offExtender();
				Delayms(10);
				proxyCount1++;
				if(proxyCount1 == 10)
				{
					proxyCount1 = 0;
					proxyCount = 0;
					pflag1 = 0;
				}
			}
		}
	}
	else if(getProxy1()==2 || pflag1 ==2)
			{
			pflag1 = 2;
				if (proxyCount < 10)
				{
					upExtender();
					Delayms(10);
					proxyCount++;
				}
				if (proxyCount==10)
				{
					if(proxyCount1 < 10)
					{
						offExtender();
						Delayms(10);
						proxyCount1++;
						if(proxyCount1 == 10)
						{
							proxyCount1 = 0;
							proxyCount = 0;
							pflag1 = 0;
						}
					}
				}
			}
	else if(getProxy1()==3 || pflag1 == 3)
		{
			pflag1 = 3;
			if (proxyCount < 10)
			{
				rightExtender();
				Delayms(10);
				proxyCount++;
			}
			if (proxyCount==10)
			{
				if(proxyCount1 < 10)
				{
					offExtender();
					Delayms(10);
					proxyCount1++;
					if(proxyCount1 == 10)
					{
						proxyCount1 = 0;
						proxyCount = 0;
						pflag1 = 0;
					}
				}
			}

		}
	else
	{
		offExtender();
		proxyCount = 0;
//		if (pflag1 == 1)
//		{
//			offExtender();
//			proxyCount = 0;
//			pflag1 = 0;
//		}
	}
}
/*
 * ballGet merupakan fungsi untuk mengetahui apakah
 * robot telah mendapatkan bola atau belum dengan
 * menggunakan proximity
 * fungsi ini akan memutar hadle jika telah mendapatkan bola
 * dan mencari apabila belum
 */
void ballGet()
{
	if (getProxy() == 3)
	{
		handleRotateIn();
//		if(compassHeading>180)
//		{
//			rotateAntiClockWise(20);
//		}
//		else if (compassHeading<180)
//		{
//			rotateClockWise(20);
//		}
	}
	else
	{
		handleOff();
		getBall();
	}
}

void getBallNew()
{
	int kedepan;
	if(ballYCoor < 40)
	{
		kedepan = VYDEKAT;
	}
	else if(ballYCoor < 50)
	{
		kedepan = VYJAUH;
	}
	else{
		kedepan = VYMAKS;
	}
	int tengah = PID(ballXCoor, servoMid, VXMAKS);
	if(ballFound == 1){
		motorSpeed(tengah - kedepan, tengah - kedepan, tengah + kedepan, tengah + kedepan);
	}
	else
	{
		//rotateClockWise(50);
		if(flagStrategy == 0)
		{
			prevCompass = compassHeading + 20;
			flagStrategy = 1;
		}
		if(flagStrategy == 1)
		{
			rotateClockWise(50);
			if(compassHeading >= prevCompass - 5 && compassHeading <= prevCompass + 5)
			{
				flagStrategy = 2;
			}
		}
		if(flagStrategy == 2)
		{
			rotateClockWise(50);
			if(compassHeading >= 177 && compassHeading <= 183)
			{
				maju(50);
				for(strategyCount = 0; strategyCount < 30; strategyCount++)
				{
					Delayms(100);
					if(ballFound == 1)
					{
						break;
					}
				}
				flagStrategy = 0;
			}
		}
	}
}

////		if (wasit == 0)
////		{
////			maju(VXMAKS);
////			for(strategyCount = 0; strategyCount < NYARIDELAY; strategyCount++)
////			{
////				Delayms(100);
////				if(ballFound == 1)
////				{
////					break;
////				}
////			}
////			wasit++;
////		}
//		if(flagStrategy == 0)
//		{
//			prevCompass = compassHeading + 20;
//			flagStrategy = 1;
//		}
//		if(flagStrategy == 1)
//		{
//			rotateClockWise(50);
//			if(compassHeading >= prevCompass - 5 && compassHeading <= prevCompass + 5)
//			{
//				flagStrategy = 2;
//			}
//		}
//		if(flagStrategy == 2)
//		{
//			rotateClockWise(50);
//			if(compassHeading >= 177 && compassHeading <= 183)
//			{
//				maju(50);
//				for(strategyCount = 0; strategyCount < 30; strategyCount++)
//				{
//					Delayms(100);
//					if(ballFound == 1)
//					{
//						break;
//					}
//				}
//				flagStrategy = 0;
//			}
//		}
//	}
//}


//{
//	int tengah = PID(ballXCoor, servoMid, 100);
//	int kedepan = 75;
//	rpmKiri = tengah-kedepan;
//	rpmKanan = kedepan+tengah;
//	if(ballFound == 1){
//		if(jarakDepan < 25){
//			if(rpmKanan > 30){
//				int offsetKanan = rpmKanan - 30;
//				int perbandinganKanan = rpmKiri/rpmKanan;
//				rpmKanan = 30;
//				rpmKiri = rpmKiri + offsetKanan * perbandinganKanan;
//			}
//			if(rpmKiri < -30){
//				int offsetKiri = rpmKiri + 30;
//				int perbandinganKiri = rpmKanan/rpmKiri;
//				rpmKiri = -30;
//				rpmKanan = rpmKanan - offsetKiri * perbandinganKiri;
//			}
//		motorSpeed(rpmKiri, rpmKiri, rpmKanan, rpmKanan);
//		if(ballYCoor < 30)
//		{
//			motorSpeed(0,0,0,0);
//		}
//		}
//	}
//	else
//	{
////		prevCompass = compassHeading;
//		rotateAntiClockWise(50);
////		if (compassHeading == prevCompass)
////		{
////			int cariBolaCount;
////			getBallNew();
////			if (ballFound == 0 && compassHeading == 0)
////			{
////				maju(50);
////				for (cariBolaCount = 0; cariBolaCount < 70; cariBolaCount++)
////				{
////					Delayms(100);
////					if (ballFound == 1)
////					{
////						break;
////					}
////				}
////			}
////		}
//	}
//}

void getBall(void){
	if(ballFound == 1){
		if((ballXCoor-servoMid) > -15 && (ballXCoor - servoMid) < 15){
			errorBallMaju =  ballYCoor ;
			derivErrorBallMaju = errorBallMaju - prevErrorBallMaju;
			intErrorBallMaju += errorBallMaju;
			ballMaju = (float)KaP * errorBallMaju + (float)KaD * derivErrorBallMaju + (float)KaI * intErrorBallMaju;
			if(ballMaju>75){
				ballMaju = 75;
			}
			else if(ballMaju<20){
				ballMaju = 20;
			}
			if(jarakDepan < 70)
			{
				maju(PID(jarakDepan,40,30));
			}
			maju(ballMaju);
			prevErrorBallMaju=errorBallMaju;
		}
		else{
			errorBall = servoMid - ballXCoor ;
			derivErrorBall = errorBall - prevErrorBall;
			intErrorBall += errorBall;
			ball = (float)KaP * errorBall + (float)KaD * derivErrorBall + (float)KaI * intErrorBall;
			if(ball>50){
				ball = 50;
			}
			else if(ball<15 && ball >0){
				ball = 10;
			}
			else if(ball>-15 && ball<0){
				ball = -10;
			}
			else if (ball <-50){
				ball = -50;
			}
			rotateAntiClockWise(ball);
			prevErrorBall=errorBall;
		}
	}
	else{
		rotateAntiClockWise(20);
	}


}

int PID(int param, int desire, int vMaks){
	int temp;
	errorBall = desire - param ;
	derivErrorBall = errorBall - prevErrorBall;
	intErrorBall += errorBall;
	temp = (float)KaP * errorBall + (float)KaD * derivErrorBall + (float)KaI * intErrorBall;
	if(temp>vMaks){
		temp = vMaks;
	}
	else if(temp<5 && temp >0){
		temp = 5;
	}
	else if(temp>-5 && temp<0){
		temp = -5;
	}
	else if (temp <-vMaks){
		temp = -vMaks;
	}
	prevErrorBall=errorBall;
	return temp;
}

bool getGoalPos(void){


	if(strategy == 2){
		if(compassHeading <55 && compassHeading>35){
			 return true;
		 }
		 else{
			rotateAntiClockWise(PID(compassHeading,45,30));
			handleRotateIn();
			return false;
		 }
	}
	else if(strategy == 3){
		if(compassHeading <325 && compassHeading>305){
			 return true;
		 }
		 else{
			rotateAntiClockWise(PID(compassHeading,315,30));
			handleRotateIn();
			return false;
		 }
	}
	else{
		if(compassHeading <10|| compassHeading>350){
			 return true;
		 }
		 else{
			rotateAntiClockWise(PID(compassHeading,0,30));
			handleRotateIn();
			return false;
		 }

	}
}

bool gotoHeadZero(void){
	 if(compassHeading == 0)
	 {
		return true;
	 }
	 else{
		rotateAntiClockWise(PID(compassHeading,0,30));
		handleRotateIn();
		return false;
	 }
}

void getCoor(int headingProcess,int encoderLeft, int encoderRight){
    	if(headingProcess == 1){
//    		if(headCount > 586) headCount -= 586;
//    		if(headCount < 0) headCount += 586;
//        	heading = (float)headCount*360/586;
    	}
    	else{
    		left_mm = encoderLeft / TICK_PER_MM_LEFT;
    		right_mm = encoderRight / TICK_PER_MM_RIGHT;
    		theta += (right_mm - left_mm) / DIAMETER ;
    		if (theta > PI)
    		theta -= TWOPI;
    		if (theta < (-PI))
    		theta += TWOPI;
    		mov = (left_mm + right_mm) / 200; ;
    		double val = PI/180;
    		movX = movX + mov * cos (compassHeading*val);
    		movY = movY + mov * sin (compassHeading*val);
    		movPrev = mov;
    	}
}

void gotoXY(int coorX, int coorY, int head)
{
	int kedepan = PID(Y_pos, coorY, 100);
	int teta = atan(((coorX - X_pos)/(coorY - Y_pos)));
	int tengah = PID(compassHeading, teta, 100);
	motorSpeed(tengah - kedepan, tengah - kedepan, tengah + kedepan, tengah + kedepan);
	if(Y_pos == coorY && X_pos == coorX)
	{
		rotateAntiClockWise(PID(compassHeading, head, 50));
	}
}

void gotoxyCoach(){

		float x,y;
		double val = 180/PI;
		x = nodex2-nodex1;
		y = nodey1-nodey2;
		gotoHead = atan(y/x)*val;
		if(x>0 && y>0){
			gotoHead = gotoHead;
		}
		else if(x<0 && y> 0){
			gotoHead = gotoHead + 180;
		}
		else if(x<0 && y<0){
			gotoHead = gotoHead + 180;
		}
		else if(x>0 && y<0){
			gotoHead = gotoHead+360;
		}
		else{
			gotoHead = compassHeading;
		}
		if((gotoHead-compassHeading>-20 && gotoHead-compassHeading<0) ||
				(gotoHead-compassHeading<20 && gotoHead-compassHeading>=0)){
			if(abs(nodex2-nodex1) > abs(nodey2-nodey1)){
				if(nodex2-movX<=5 && nodex2-movX>=-5){
					motorSpeed(0,0,0,0);
				}
				else if(nodex2>nodex1 && nodex2-movX>=5){
					maju(70);
				}
				else if(nodex2>nodex1 && nodex2-movX<-5){
					mundur(30);
				}
				else if(nodex2<nodex1 && nodex2-movX<=-5){
					maju(70);
				}
				else if(nodex2<nodex1 && nodex2-movX>5){
					mundur(30);
				}
			}
			else{
				if(nodey2-movY<=5 && nodey2-movY>=-5){
					motorSpeed(0,0,0,0);
				}
				else if(nodey2>nodey1 && nodey2-movY>=5){
					maju(70);
				}
				else if(nodey2>nodey1 && nodey2-movY<-5){
					mundur(30);
				}
				else if(nodey2<nodey1 && nodey2-movY<=-5){
					maju(70);
				}
				else if(nodey2<nodey1 && nodey2-movY>5){
					mundur(30);
				}
			}
		}
		else {
			rotateAntiClockWise(PID(compassHeading,gotoHead,40));

		}
}

int gotoKickField()
{
	int flagKickField = 0;
	if((X_pos <= MINXKICK || X_pos >= MAKSXKICK) || (Y_pos <= MINYKICK || Y_pos >= MAKSYKICK))
	{
		if(flagKickField == 0)
		{
			rotateAntiClockWise(PID(compassHeading, 180, 100));
			if(compassHeading == 180)
			{
				flagKickField = 1;
			}
		}
		else if(flagKickField == 1)
		{
			if(jarakBelakang > 3000 && jarakKananBelakang > 3000 && jarakKiriBelakang > 3000)
			{
				mundur(200);
			}
			else if(jarakBelakang <= 3000)
			{
				if(jarakKananBelakang <= 3000)
				{
					tenggara(200);
				}
				else if(jarakKiriBelakang <= 3000)
				{
					baratDaya(200);
				}
			}
			else if(jarakKananBelakang <= 3000)
			{
				kiri(200);
			}
			else if(jarakKiriBelakang <= 3000)
			{
				kanan(200);
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

void tesRotateMap(void){
	int icount=1;
	while(compassHeading > (90 * icount)+2){
		rotateAntiClockWise(PID(compassHeading,(90 * icount)+2,50));
	}
	while(1){
		if(icount == 1){
				while(compassHeading > (90 * icount)+2){
					rotateAntiClockWise(PID(compassHeading,(90 * icount)+2,50));
				}
		}

		while(compassHeading < (90 * icount)){
			rotateAntiClockWise(PID(compassHeading,(90 * icount),50 ));
		}
		maju(20);
		Delayms(2000);
		rotateAntiClockWise(0);
		Delayms(2000);
		icount+=2;
		if (icount > 3){
			icount = 1;
		}
	}
}


void LCD_InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	// TIM clock = 84 MHz,
	//Update event time = 1 / [(84 * 10^6) / (TIM_Prescaler * TIM_Period)]
    //kalo mau sedetik: pre = 42000-1, per = 2000-1
    TIM_TimeBaseInitTypeDef timerInitStructure;

    timerInitStructure.TIM_Prescaler = 42000-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 1000-1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM5, &timerInitStructure);
    TIM_Cmd(TIM5, ENABLE);
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

}
void LCD_EnableTimerInterrupt()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM5_IRQHandler(){
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	    {
	        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	        printData();

	    }
}

void init_lcdTimer(){
	LCD_InitializeTimer();
	LCD_EnableTimerInterrupt();
}

int hindar()
{
	if((x == 1 || y == 1) && hindarState == 0)
	{
		hindarState = 1;
	}
	else if(hindarState == 1)
	{
		if(compassHeading > 175 && compassHeading < 185)
		{
			rotateClockWise(70);
		}
		else
		{
			hindarState = 2;
		}
	}
	else if(hindarState == 2)
	{
		if(a == 1)
		{
			kiri(20);
		}
		else if(b == 1)
		{
			kanan(20);
		}
		else
		{
			hindarState = 3;
		}
	}
	else if(hindarState ==3)
	{
		if(compassHeading < 5 || compassHeading > 355)
		{
			hindarState = 0;
		}
		else
		{
			rotateClockWise(70);
		}
	}

	if(hindarState == 0)
		return 0;
	else
		return 1;
}

