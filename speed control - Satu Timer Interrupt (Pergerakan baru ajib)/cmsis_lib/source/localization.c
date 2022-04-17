
#include "localization.h"




extern double current_x;
extern double current_y;
extern double current_w;
extern double current_w2;





/*
 * Variables
 */
char lcd[16];
int team=CYAN;
extern int nodex1,nodey1,nodex2,nodey2,prevx1,prevy1;
int gotoHead;
char prevGameState;
//1 2 0
//0.9 5 0
//0.11 0 0

//float KaP = 0.11;
//double KaD = 0 / 0.0000084;
//double KaI = 0.0000084 * 0;

float KaP = 1;
double KaD = 2;
double KaI = 0;

//float KaP = 0.9;
//double KaD = 5 / 0.0000084;
//double KaI = 0.0000084 * 0;

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
extern int w;
extern int z;
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
extern float Y_pos;
extern float Y_poscmps;
extern float X_pos;
extern float X_poscmps;
extern float SRFSerongKanan;
extern float SRFSerongKiri;

int tengahflag = 0;
float tengahy;
float tengahx;
float panjanggawang;
float lebargawang;


int keeperFlagHor = 0;
int keeperFlagHor1 = 0;
int keeperFlagHor2 = 0;
int keeperFlagVer = 0;
int keeperFlagVer1 = 0;
int keeperFlagVer2 = 0;
int keeperFlag2 = 0;
int countHor = 0;
int countVer = 0;
int pwmMotor = 0;
int get_tof = 0;

//extern int32_t output = 0;
//extern int32_t outputDua = 0;
//extern int32_t outputTiga = 0;
//extern int32_t outputEmpat = 0;


/**
 * Start main merupakan fungsi utama program robot
 * fungsi ini akan mencari bola, dan gawang
 * @param refree untuk menentukan akan menggunakan refree atau tidak
 * @param mode untuk mengarahkan ke area musuh saja atau mengarahkan ke gawang musuh
 *
 */
void init_tof(){
//	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	TM_ADC_Init(ADC1, ADC_Channel_10);
}

void tof()
{
	get_tof = TM_ADC_Read(ADC1, ADC_Channel_10);
}

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
int dapetbola = 0;
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
			dapetbola = 0;
			if(ballYCoor<=50){
				handleRotateIn();
			}
			else{
				handleOff();
			}
		}
		else{
//			if (gotoHeadZero())
//			{
			if(dapetbola == 0)
			{
				if(compassHeading >= 358 || compassHeading <= 2)
				{
					dapetbola = 1;
				}
				else
				{
					rotateAntiClockWise(30);
				}
			}
			else if(dapetbola == 1)
			{
				stop();
				handleOff();
			}
//				motorSpeed(0,0,0,0);
//				Delayms(300);
//				handleOff();
//				maju(100);
//				Delayms(1000);
//				kickBall();
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


	//Proximity

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

	//Limit switch

	GPIO_InitTypeDef GPIO_InitDef;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitDef.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	//Initialize pins
	GPIO_Init(GPIOD, &GPIO_InitDef);


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

/*
 * getProxy merupakan fungsi untuk menghasilkan nilai 0 atau 1
 * dari inputan proximity
 */
//void getProxy()
//{
//	proxyKanan = GPIO_ReadInputDataBit(GPIOC,GPIO_PIN_5);
//	proxyKiri = GPIO_ReadInputDataBit(GPIOC,GPIO_PIN_2);
//	proxyDepanKanan = GPIO_ReadInputDataBit(GPIOC,GPIO_PIN_4);
//	proxyDepanKiri = GPIO_ReadInputDataBit(GPIOC,GPIO_PIN_3);
//	proxyBelakang = GPIO_ReadInputDataBit(GPIOC,GPIO_PIN_6);
//}

/*
 * ballGet merupakan fungsi untuk mengetahui apakah
 * robot telah mendapatkan bola atau belum dengan
 * menggunakan proximity
 * fungsi ini akan memutar hadle jika telah mendapatkan bola
 * dan mencari apabila belum
 */



int ballGet()
{
	limitSwitchKiri = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	limitSwitchKanan = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7);


	if(limitSwitchKanan || limitSwitchKiri)
		return true;
	else
		return false;
}

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

int temp;

int PID(int param, int desire, int vMaks){

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

}


int getGoalPosition()
{

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
		if(x == 1)
		{
			kiri(20);
		}
		else if(x == 1)
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

void controlWifi()
{
	//kontrol wifi
	if(gameState == '$')
   {
	kiri(120);
   }
   else if(gameState == '%')
   {
	kanan(120);
   }
   else if(gameState == '@')
   {
	maju(120);
   }
   else if(gameState == '#')
   {
	mundur(120);
   }
   else if(gameState == '^')
   {
	rotateAntiClockWise(90);
   }
   else if(gameState == '&')
   {
	rotateClockWise(90);
   }
   else if(gameState == '*')
   {
	motorSpeed(0,0,0,0);
   }
   else if(gameState == '=')
   {
//	   if(keeperBalik(200))
//	   {
		   USART_SendData(USART2, ballXCoor);
//	   }
   }
}

int flagEvade = 0;

void moveLocal(double x, double y, double w)
{
	w += current_w;

	if(w >= 360)
		w -= 360;

	double w2 = w * PI / 180;

	x = (cos(current_w2)*x) - (sin(current_w2)*y);
	y = (sin(current_w2)*x) + (cos(current_w2)*y);

	traInit(x + current_x, y + current_y, w, 0, 0, 3);
}

void evade()
{
	if(flagEvade == 0)
	{
		if(current_x < 0)
		{
			moveLocal(-150, 0, 90);
			Delayms(3000);
		}
		else if(current_w > 0)
		{
			moveLocal(150, 0, 270);
			Delayms(3000);
		}

		if(ballGet())
			flagEvade = 1;
	}
	else if(flagEvade == 1)
	{
		moveLocal(0, 150, 0);
		Delayms(3000);

		flagEvade = 0;
	}
}

void evadeFront()
{
	double w = current_w + 90;

	if(x > 360)
		x -= 360;

	traInit(current_x, current_y, x,0 ,0, 3);

	int i = 0;
	while(i < 20)
	{
		Delayms(100);

//		if(!ballGet())
//			break;


	}
}

void evadeLeft()
{
	moveLocal(150, 0, 270);

	int i = 0;
	while(i < 20)
	{
		Delayms(100);

//		if(!ballGet())
//			break;
	}
}

void evadeRight()
{
	moveLocal(-150, 0, 90);

	int i = 0;
	while(i < 20)
	{
		Delayms(100);

//		if(!ballGet())
//			break;
	}
}

void evadeStrategy()
{
	getProxy();

	if(proxyDepanKanan || proxyDepanKiri)
	{
		evadeFront();

		flagEvade = 0;
		evade();
	}
	else if(proxyKanan)
	{
		evadeRight();

		flagEvade = 1;
		evade();
	}
	else if(proxyKiri)
	{
		evadeLeft();

		flagEvade = 1;
		evade();
	}
}
