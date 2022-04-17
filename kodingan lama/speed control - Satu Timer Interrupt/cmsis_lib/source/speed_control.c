// speed_control.c & speed_control.h: Adika Bintang Sulaeman, Jan 16th 2015
// edited Jan 11th 2017 by Yunus Maulana
// update notes : 4 motor with 4 encoder
// This code was tested using Pololu motor 19:1 with encoder with max speed: 200 rpm
// To use this code, one should plug the following cables to the microcrontroller:
// motor 1   : in2: PB12, in1: PB13
// encoder 1 : Yellow PD0, White PD4
// motor 2   : in3: PB14, in4: PB15
// encoder 2 : Yellow PD1, White PD5
// motor 3   : in3: PE12, in4: PE14
// encoder 3 : Yellow PD2, White PD6
// motor 4   : in3: PE13, in4: PE15
// encoder 4 : Yellow PD3, White PD7
// dependencies: as they are included in #include
// Before implementing code in while (1), you have to call void init_speed_control(void) inside int main()
// To set the motor speed, use motorSpeed(kiri belakang, kiri depan, kanan depan, kanan belakang);
// Credit to: Tilen Majerle (stm32f4-discovery.com)

// Important:
// after initialize, directly call motorSpeed(0,0);
// if you want motor to stop, always motorSpeed(0, 0);
// e.g.: muter: motorSpeed(0,0); motorSpeed(-60,60); Delayms(1000); motorSpeed(0,0);

// edited 2018
// kinematic and trajector calculation
// credit KRAI 2018


#include "speed_control.h"


int xCoor2;
int yCoor2;
extern uint16_t xCoor;
extern uint16_t yCoor;
double sudut2;
int vRotate = 5000;

double time_sampling = 0.004679 * 10;
double min_pid_limit = -2147483640.0;
double max_pid_limit = 2147483640.0;
//2147483640.0
double min_pwm_limit = -12500;
double max_pwm_limit = 12500;

//hitung koordinat global
double a = 0, b = 0;
int abs1 = 1, abs2 = 1;
int ballCoorX = 0, ballCoorY = 0;
double sudutBall = 0;
int iterBall = 0;
int avgxBall = 0;
int avgyBall = 0;

/*
 * Variables
 */
int8_t startHeading;
int8_t prevStartHeading;
int32_t rotaryCount = 0;
int32_t lastCount = 0;
int16_t error = 0;
int16_t errorDua = 0;
int16_t errorTiga = 0;
int16_t errorEmpat = 0;
volatile int32_t rpm = 0;
volatile int32_t rpmDua = 0;
volatile int32_t rpmTiga = 0;
volatile int32_t rpmEmpat = 0;
int16_t setPointSatu = 0;
int16_t setPointDua = 0;
int16_t setPointTiga = 0;
int16_t setPointEmpat = 0;
int32_t output = 0;
int32_t outputDua = 0;
int32_t outputTiga = 0;
int32_t outputEmpat = 0;
int32_t speed = 0;
int16_t prevError = 0;
int16_t derivError = 0;
int16_t intError = 0;
int16_t prevErrorDua = 0;
int16_t derivErrorDua = 0;
int16_t intErrorDua = 0;
int16_t prevErrorTiga = 0;
int16_t derivErrorTiga = 0;
int16_t intErrorTiga = 0;
int16_t prevErrorEmpat = 0;
int16_t derivErrorEmpat = 0;
int16_t intErrorEmpat = 0;
int coorCount;

extern int32_t TM_INT_RE_Count;
extern int32_t secondCount;
extern int32_t thirdCount;
extern int32_t fourthCount;
extern int32_t fifthCount;
extern int32_t sixthCount;

TM_RE_t data;
TM_RE_t data2;
TM_RE_t data3;
TM_RE_t data4;
TM_RE_t data5;
TM_RE_t data6;

//-667.2512755101973

double sudut = 0;
extern float compassHeading;

double ppr = 19.2 * 7 * 275 / 40;
//double ppr = 19.2 * 7 * 275 / 80;
double robot_radius = 28;
double wheel_radius = 5;

double current_x = 0.0;
double current_y = 0.0;
double current_w = 0.0;
double current_w2 = 0.0;
double currentLocal_x = 0.0;
double currentLocal_y = 0.0;
double currentLocal_w = 0.0;

double current_speed_1 = 0.0;
double current_speed_2 = 0.0;
double current_speed_3 = 0.0;
double current_speed_4 = 0.0;
double current_speed_5 = 0.0;
double current_speed_6 = 0.0;

double odo_local_vx = 0.0;
double odo_local_vy = 0.0;
double odo_local_w = 0.0;
double odo_global_vx = 0.0;
double odo_global_vy = 0.0;
double odo_global_w = 0.0;
double odo_global_vx_prev = 0.0;
double odo_global_vy_prev = 0.0;
double odo_global_w_prev = 0.0;

//begin variable trajectory
double tra_input_x = 0.0; //input tajectory x
double tra_input_y = 0.0; //input tajectory y
double tra_input_w = 0.0; //input tajectory w

double tra_count_total_x = 0.0; //total waktu trajectory x
double tra_count_x = 0.0; //counter x trajectory
double tra_current_time_x = 0.0; //current x trajectory time
volatile double tra_current_x = 0.0; //current x trajectory position
double last_tra_x = 0.0;

double tra_count_total_y = 0.0; //total waktu trajectory y
double tra_count_y = 0.0; //counter y trajectory
double tra_current_time_y = 0.0; //current y trajectory time
volatile double tra_current_y = 0.0; //current y trajectory position
double last_tra_y = 0.0;

double tra_count_total_w = 0.0; //total waktu trajectory w
double tra_count_w = 0.0; //counter w trajectory
double tra_current_time_w = 0.0; //current w trajectory time
volatile double tra_current_w = 0.0; //current w trajectory position
double last_tra_w = 0.0;

int is_tra_x_done = 0; //trajectory done or not
int is_tra_y_done = 0;
int is_tra_w_done = 0;

int is_tra_sin = 0; //sinusoidal trajectory

int_is_counted_up = 0;
int move_ccounter = 1;
//end variable trajectory

//pid variable
volatile double pid_current_x = 0.0;
double pid_error_x = 0.0;
double pid_proportional_x = 0.0;
double pid_integral_x = 0.0;
double pid_derivative_x = 0.0;

volatile double pid_current_y = 0.0;
double pid_error_y = 0.0;
double pid_proportional_y = 0.0;
double pid_integral_y = 0.0;
double pid_derivative_y = 0.0;

volatile double pid_current_w = 0.0;
double pid_error_w = 0.0;
double pid_proportional_w = 0.0;
double pid_integral_w = 0.0;
double pid_derivative_w = 0.0;
//end pid variable

//begin variable kinematic
double L_speed = 0.0;
double R_speed = 0.0;
double F_speed = 0.0;
double B_speed = 0.0;

double kine_local_vx = 0.0;	//hasil kecepatan vx dari kine
double kine_local_vy = 0.0;	//hasil kecepatan vy dari kine
double kine_local_w = 0.0;	//hasil w dari kine

double speed_1_value = 0.0; //hasil kecepatan roda 1 dari kine
double speed_2_value = 0.0;	//hasil kecepatan roda 2 dari kine
double speed_3_value = 0.0;	//hasil kecepatan roda 3 dari kine
double speed_4_value = 0.0;	//hasil kecepatan roda 4 dari kine

double pwm_1_value = 0.0;	//hasil pwm motor 1 dari kine
double pwm_2_value = 0.0;	//hasil pwm motor 2 dari kine
double pwm_3_value = 0.0;	//hasil pwm motor 3 dari kine
double pwm_4_value = 0.0;	//hasil pwm motor 4 dari kine
//end variable kinematic

//fungsi limit pid
#ifdef LIMIT
	#define LIMITOUT(amt,low,high) constrain(amt,low,high)
#else
	#define LIMITOUT(amt,low,high) ((amt))
#endif
//end variable pid


void changeStartHeading(int now) {
	startHeading = now;
}



/*
 * inisialisasi timer interrupt 2
 */
//timer interrupt setiap 0.0042 detik

//ralat event time = 0.00467857142857142857142857142857 detik
//					 0.004679
//1.37 = 1 / (ppr * event time)
void InitializeTimer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// TIM clock = 84 MHz,
	//Update event time = 1 / [(84 * 10^6) / (TIM_Prescaler * TIM_Period)]
    //kalo mau sedetik: prescaler = 42000-1, period = 2000-1
    TIM_TimeBaseInitTypeDef timerInitStructure;

    timerInitStructure.TIM_Prescaler = 39300-1;
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 10-1;
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void EnableTimerInterrupt()
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void init_speed_control(void)
{
	InitializeTimer();
	EnableTimerInterrupt();
	TM_RE_Init(&data);
	InitSecondEncoder(&data2);
	InitThirdEncoder(&data3);
	InitFourthEncoder(&data4);
//	InitFifthEncoder(&data5);
//	InitSixthEncoder(&data6);
	init_pwm();
	init_motor();

	odoInit();
//	traInit(5000, 0, 0, 6);
	pidInit();
}

void odoInit(){
	current_speed_1 = 0.0;
	current_speed_2 = 0.0;
	current_speed_3 = 0.0;
	current_speed_4 = 0.0;

	odo_local_vx = 0.0;
	odo_local_vy = 0.0;
	odo_local_w = 0.0;

	odo_global_vx = 0.0;
	odo_global_vy = 0.0;
	odo_global_w = 0.0;

	odo_global_vx_prev = 0.0;
	odo_global_vy_prev = 0.0;
	odo_global_w_prev = 0.0;

	current_x = 0.0;
	current_y = 0.0;
	current_w = 0.0;
}

void traSinInit(){
	traInit(0.0,0.0,0.0,0.0);
	traXInit(5250.0,6.0);
	is_tra_sin = 1;
	current_y = -500.0;
	is_tra_y_done = 1;
}

void traSinOff(){
	traInit(0.0,0.0,0.0,0.0);
	is_tra_sin = 0;
}

void traInit(double x, double y, double w, double time) {
	//transfer variabel ke masing masing fungsi
	traXInit(x, time);
	traYInit(y, time);
	traWInit(w, time);
}

void traXInit(double x, double time_x) {
	//initialisasi trajectory
	resetInit();								//reset odometry
	tra_input_x = x;							//input total trajectory yang dimau
	tra_count_total_x = time_x / time_sampling;	//hitung total waktu yang dibutuhkan untuk trajectory
	tra_count_x = 0.0;							//nol kan counter trajectory
	tra_current_x = 0.0;						//nol kan hasil trajectory
	is_tra_x_done = 0;							//reset tra status
//	moderobot = OTOMATIS;
}

void traYInit(double y, double time_y) {
//	resetInit();
	tra_input_y = y;
	tra_count_total_y = time_y / time_sampling;
	tra_count_y = 0.0;
	tra_current_y = 0.0;
	is_tra_y_done = 0;
//	moderobot = OTOMATIS;
}

void traWInit(double w, double time_w) {
//	resetInit();
//	tra_input_w = w*M_PI/180.0;
	tra_input_w = w;
	tra_count_total_w = time_w / time_sampling;
	tra_count_w = 0.0;
	tra_current_w = 0.0;
	is_tra_w_done = 0;
//	moderobot = OTOMATIS;
}

void pidInit(){
	//nol kan seluruh variabel pid
	pid_integral_x = 0.0;
	last_tra_x = 0.0;

	pid_integral_y = 0.0;
	last_tra_y = 0.0;

	pid_integral_w = 0.0;
	last_tra_w = 0.0;
}

void resetInit(){
	odoInit();
	pidInit();

	TM_INT_RE_Count = 0;
	data.Diff = 0;
	data.Absolute = 0;
	secondCount = 0;
	data2.Diff = 0;
	data2.Absolute = 0;
	thirdCount = 0;
	data3.Diff = 0;
	data3.Absolute = 0;
	fourthCount = 0;
	data4.Diff = 0;
	data4.Absolute = 0;
}


/*
 * interrupt handler untuk uart3
 */

/*PERUBAHAN:
 * Ditambah iteration time 0.0042
 * */
int motor = 0;
volatile int aaa = 0;
volatile int iii1 = 0;
volatile int iii2 = 0;
volatile int iii3 = 0;
volatile int iii4 = 0;


void TIM2_IRQHandler()
{

	// if(motor == 0)
	// {
// 	 	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
// 	 	{
// 	 		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//
// 	 		//Motor 2 kiri depan
// 			SecondGet(&data2);
//
// 			rpmDua = (int32_t)(((1 / ppr) * data2.Diff) * 60/ (0.0042 * 10));
//
// 			errorDua = setPointDua - rpmDua;
// 			derivErrorDua = (errorDua - prevErrorDua);
// 			intErrorDua += errorDua;
//
// 	 		if(intErrorDua > 32000)
// 	 			intErrorDua = 32000;
// 	 		else if(intErrorDua < -32000)
// 	 			intErrorDua = -32000;
//
// 			outputDua += (float)KP0 * errorDua + (float)KD0 * derivErrorDua + (float)KI0 * intErrorDua;
//
// 			if (outputDua > max_pwm_limit) outputDua = max_pwm_limit;
// 			else if (outputDua < min_pwm_limit) outputDua = min_pwm_limit;
// 			motorDC(2, outputDua);
//
// 			prevErrorDua = errorDua;
//
// 	 		//Motor 3 Kanan depan
// 	 		ThirdGet(&data3);
//
// 	 		rpmTiga = (int32_t)(((1 / ppr) * data3.Diff) * 60/ (0.0042 * 10));
//
// 	 		errorTiga = setPointTiga - rpmTiga;
// 	 		derivErrorTiga = (errorTiga - prevErrorTiga);
// 	 		intErrorTiga += errorTiga;
//
// 	 		if(intErrorTiga > 32000)
// 	 			intErrorTiga = 32000;
// 	 		else if(intErrorTiga < -32000)
// 	 			intErrorTiga = -32000;
//
// 	 		outputTiga += (float)KP0 * errorTiga + (float)KD0 * derivErrorTiga + (float)KI0 * intErrorTiga;
//
// 	 		if (outputTiga > max_pwm_limit) outputTiga = max_pwm_limit;
// 	 		else if (outputTiga < min_pwm_limit) outputTiga = min_pwm_limit;
// 	 		motorDC(3, outputTiga);
//
// 	 		prevErrorTiga = errorTiga;
//
// 	 		//Motor 1 Kiri belakang
// 	 		TM_RE_Get(&data);
//
// 	 		rpm = (int32_t)(((1 / ppr) * data.Diff) * 60/ (0.0042 * 10));
// //	 		current_speed_1 = wheel_radius *(((2.0 * PI / ppr) * data.Diff) / time_sampling);
//
// 	 		error = setPointSatu - rpm;
// 	 		derivError = (error - prevError);
// 	 		intError += error;
//
// 	 		if(intError > 32000)
// 	 			intError = 32000;
// 	 		else if(intError < -32000)
// 	 			intError = -32000;
//
// 	 		output += (float)KP0 * error + (float)KD0 * derivError + (float)KI0 * intError;
//
// 	 		if (output > max_pwm_limit) output = max_pwm_limit;
// 	 		else if (output < min_pwm_limit) output = min_pwm_limit;
// 	 		motorDC(1, output);
//
// 	 		prevError = error;
//
// 	 		//Motor 4 Kanan belakang
// 	 		FourthGet(&data4);
//
// 	 		rpmEmpat = (int32_t)(((1 / ppr) * data4.Diff) * 60/ (0.0042 * 10));
//
// 	 		errorEmpat = setPointEmpat - rpmEmpat;
// 	 		derivErrorEmpat = (errorEmpat - prevErrorEmpat);
// 	 		intErrorEmpat += errorEmpat;
//
// 	 		if(intErrorEmpat > 32000)
// 	 			intErrorEmpat = 32000;
// 	 		else if(intErrorEmpat < -32000)
// 	 			intErrorEmpat = -32000;
//
// 	 		outputEmpat += (float)KP0 * errorEmpat + (float)KD0 * derivErrorEmpat + (float)KI0 * intErrorEmpat;
//
// 	 		if (outputEmpat > max_pwm_limit) outputEmpat = max_pwm_limit;
// 	 		else if (outputEmpat < min_pwm_limit) outputEmpat = min_pwm_limit;
// 	 		motorDC(4, outputEmpat);
//
// 	 		prevErrorEmpat = errorEmpat;
//
// 	 //        //int rata2 = (data6.Diff - data5.Diff)/2;
// 	 //        coorCount++;
// 	 //        if(coorCount == 10){
// 	 //        	FifthGet(&data5);
// 	 //        	SixthGet(&data6);
// 	 //        	getCoor(false,-data5.Diff,data6.Diff);
// 	 //        	coorCount =0;
// 	 //        }
// 			odometers();
// 	 		accelerometers();
// 	 	}
//	 }

	//
	// else if(motor == 1)
	// {
		//menggunakan encoder untuk mapping
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
		{
			TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	 		if(iii1 == 9)
	 		{
	 			iii1 = -1;
				TM_RE_Get(&data);
				SecondGet(&data2);
				ThirdGet(&data3);
				FourthGet(&data4);
	 		}
	 		iii1++;

//	 		FifthGet(&data5);
//	 		SixthGet(&data6);

	 		gotoBall = 0;


//	 		if(gotoBall == 1)
//	 		{
//	 			moveInput();
//	 		}
//	 		else
//	 		{
//				calculatePosition();
				calculatePosition2();
//				calculateTrj();
				calculatePID();
				kinematic();
//			}









			if(output > max_pwm_limit) output = max_pwm_limit;
			else if(output < min_pwm_limit) output = min_pwm_limit;

			if(outputDua > max_pwm_limit) outputDua = max_pwm_limit;
			else if(outputDua < min_pwm_limit) outputDua = min_pwm_limit;

			if(outputTiga > max_pwm_limit) outputTiga = max_pwm_limit;
			else if(outputTiga < min_pwm_limit) outputTiga = min_pwm_limit;

			if(outputEmpat > max_pwm_limit) outputEmpat = max_pwm_limit;
			else if(outputEmpat < min_pwm_limit) outputEmpat = min_pwm_limit;

//			if(xCoor == 320 && yCoor == 240)
//			{
//				output = 0;
//				outputDua = 0;
//				outputTiga = 0;
//				outputEmpat = 0;
//			}

			pid_error_w = tra_input_w - current_w;

			if((pid_error_w > 7 && pid_error_w < 180) || pid_error_w < -180)
			{
				output += vRotate;
				outputDua += vRotate;
				outputTiga += vRotate;
				outputEmpat += vRotate;
			}
			else if(pid_error_w < -7 || pid_error_w > 180)
			{
				output -= vRotate;
				outputDua -= vRotate;
				outputTiga -= vRotate;
				outputEmpat -= vRotate;
			}

//			output = 0;
//			outputDua = 0;
//			outputTiga = 0;
//			outputEmpat = 0;

//			output = -7500;
//			outputDua = -7500;
//			outputTiga = -7500;
//			outputEmpat = -7500;

			if(controlMode == 0)
			{
				motorDC(1, output);
				motorDC(2, outputDua);
				motorDC(3, outputTiga);
				motorDC(4, outputEmpat);
			}

//			}
		}
//	 }
}

void moveInput(){

/*x
 * +25
 *
 * 40	461
 * 60	483
 * 80	495
 * 100	511
 * 120	515
 * 140	521
 * 160	525
 * 180	525
 */
	//ubah titik tengah pixel

	xCoor2 = - ((int)xCoor - 320);
	yCoor2 = - ((int)yCoor - 240);

	//default diam, kalau ga ada bola
	//	atau ke tempat bola terakhir
	if(xCoor2 == 0 && yCoor2 == 0)
	{
//		xCoor2 = 0;
//		yCoor2 = 0;
//		a = 0;
//		b = 0;

//		traInit(current_x, current_y, current_w, 0.0);
//		return;
	}
	else{

		//=========hitung sudut bola dari kamera ke sudut global

		//cari sudut bola terhadap kamera
		if(xCoor2 == 0 && yCoor2 == 0)
			{sudutBall = 0;}
		else
			{sudutBall = atan2(yCoor2, xCoor2) * 180 / PI;}

		//ubah sudut untuk kuadran
		if(xCoor2 < 0  && yCoor2 < 0)
			{sudutBall = 360 + sudutBall;}
		else if(xCoor2 > 0  && yCoor2 < 0)
			{sudutBall += 360;}

		//shift 90 agar 0 derajat bola di depan robot
		if(sudutBall > 90)
			{sudutBall -= 90;}
		else
			{sudutBall += 270;}
		//========selesai hitung sudut global bola di variabel sudutBall



		//=========mulai hitung jarak bola dalam centimeter

		//penanda nilai negatif untuk hitung pixel ke centimeter
		abs1 = 1;
		abs2 = 1;

		//ubah koordinat jadi absolut dulu
		if (xCoor2 < 0)
		{
			xCoor2 *= -1;
			abs1 = 0;
		}

		if(yCoor2 < 0)
		{
			yCoor2 *= -1;
			abs2 = 0;
		}

		//ubah pixel ke centimeter
		if(xCoor2 < 120)
			a = 0;
		else if(xCoor2 < 141)	//40
			a =  65;
		else if(xCoor2 < 163)	//60
			a =  85;
		else if(xCoor2 < 175)	//80
			a =  105;
		else if(xCoor2 < 191)	//100
			a =  125;
		else if(xCoor2 < 195)	//120
			a =  145;
		else if(xCoor2 < 201)	//140
			a =  165;
		else if(xCoor2 < 205)	//160
			a =  185;

		if(yCoor2 < 120)
			b = 0;
		else if(yCoor2 < 141)		//40
			b =  65;
		else if(yCoor2 < 163)	//60
			b =  85;
		else if(yCoor2 < 175)	//80
			b =  105;
		else if(yCoor2 < 191)	//100
			b =  125;
		else if(yCoor2 < 195)	//120
			b =  145;
		else if(yCoor2 < 201)	//140
			b =  165;
		else if(yCoor2 < 205)	//160
			b =  185;

		if(abs1 == 0)
			a *= -1;

		if(abs2 == 0)
			b *= -1;
		//=========selesai hitung centimeter


		//=========gerak dengan referensi koordinat lokal
		if(a <= 65 && b <= 65)
		{
			handleRotateIn();
		}

		//perintah gerak ke arah bola
//		traInit(ballCoorX, ballCoorY, sudutBall, 0.0);
//		traInit(0, 0, sudutBall, 0.0);

		tra_input_w = sudutBall;

		output = a - b;
		outputDua = -a - b;
		outputTiga = -a + b;
		outputEmpat = a + b;

		output *= KPBall;
		outputDua *= KPBall;
		outputTiga *= KPBall;
		outputEmpat *= KPBall;

		//=========selesai perintah gerak
	}
}

void calculatePosition2(){ //lokal
	//hitung kecepatan roda
//	current_speed_1 = wheel_radius *(((2.0 * PI / ppr) * data5.Diff) / time_sampling);
//	current_speed_2 = wheel_radius *(((2.0 * PI / ppr) * data6.Diff) / time_sampling);

//	current_speed_5 = (100 * data5.Diff) / (time_sampling * 1326);
//	current_speed_6 = (100 * data6.Diff * 1.1) / (time_sampling * 1326);

	current_speed_1 = wheel_radius *(((2.0 * PI / ppr) * data2.Diff) / time_sampling);
	current_speed_2 = wheel_radius *(((2.0 * PI / ppr) * data.Diff) / time_sampling);
	current_speed_3 = wheel_radius *(((2.0 * PI / ppr) * data4.Diff) / time_sampling);
	current_speed_4 = wheel_radius *(((2.0 * PI / ppr) * data3.Diff) / time_sampling);

	//ubah kecepatan roda ke kecepatan lokal robot
//	odo_local_vx = 0.5 * -(current_speed_5 + (current_speed_6));
	odo_local_vx = 0;

	odo_local_vy = 0.5 * (-current_speed_5 + current_speed_6);

//	odo_local_w = (current_speed_1+current_speed_2+current_speed_3+current_speed_4)/(4*robot_radius);
	odo_local_w = (current_speed_5+current_speed_6) * 1.3554 / (2*robot_radius);

	//fungsi butuh radian
	current_w2 = current_w * (PI/ 180);

	//ubah kecepatan lokal ke kecepatan global
	odo_global_vx = (cos(current_w2)*odo_local_vx) - (sin(current_w2)*odo_local_vy);
	odo_global_vy = (sin(current_w2)*odo_local_vx) + (cos(current_w2)*odo_local_vy);
	odo_global_w = odo_local_w;
	odo_global_w = 0;

	//ubah keceptan global ke koordinat global dengan trapezoidal rule
	current_x += (odo_global_vx + odo_global_vx_prev)* 0.5 * time_sampling;
	current_y += (odo_global_vy + odo_global_vy_prev)* 0.5 * time_sampling;
	current_w += (odo_global_w + odo_global_w_prev)* 0.5 * time_sampling * (180 / PI);

	if(current_w >= 360) current_w -= 360;
	else if(current_w < 0) current_w += 360;

//	current_w = compassHeading;

	//simpan value saat ini untuk perhitungan selanjutnya
	odo_global_vx_prev = odo_global_vx;
	odo_global_vy_prev = odo_global_vy;
	odo_global_w_prev = odo_global_w;
}

float cosw;
float sinw;
double current_w_cmps;

void calculatePosition(){

	//hitung kecepatan roda
	current_speed_1 = wheel_radius *(((2.0 * PI / ppr) * data2.Diff) / time_sampling);
	current_speed_2 = wheel_radius *(((2.0 * PI / ppr) * data.Diff) / time_sampling);
	current_speed_3 = wheel_radius *(((2.0 * PI / ppr) * data4.Diff) / time_sampling);
	current_speed_4 = wheel_radius *(((2.0 * PI / ppr) * data3.Diff) / time_sampling);

	// average left encoders and right encoders 45
	L_speed = -(current_speed_2 + current_speed_1)/2;
	R_speed = (current_speed_4 + current_speed_3)/2;
	F_speed = -(current_speed_1 + current_speed_4)/2;
	B_speed = (current_speed_2 + current_speed_3)/2;

	//ubah kecepatan roda ke kecepatan lokal robot 45
	odo_local_vx = 0.5 * (F_speed + B_speed);
	odo_local_vy = 0.5 * (R_speed + L_speed);
	odo_local_w = (current_speed_1+current_speed_2+current_speed_3+current_speed_4)/(4*robot_radius);

	//fungsi butuh radian
	current_w2 = current_w * (PI/ 180);

	//ubah kecepatan lokal ke kecepatan global
	odo_global_vx = (cos(current_w2)*odo_local_vx) - (sin(current_w2)*odo_local_vy);
	odo_global_vy = (sin(current_w2)*odo_local_vx) + (cos(current_w2)*odo_local_vy);
	odo_global_w = odo_local_w;

	//ubah keceptan global ke koordinat global dengan trapezoidal rule
	current_x += (odo_global_vx + odo_global_vx_prev)* 0.5 * time_sampling;
	current_y += (odo_global_vy + odo_global_vy_prev)* 0.5 * time_sampling;
	current_w += (odo_global_w + odo_global_w_prev)* 0.5 * time_sampling * (180 / PI);

	//derajat roda
	if(current_w >= 360) current_w -= 360;
	else if(current_w < 0) current_w += 360;

	//derajat mirror cmps
	current_w_cmps = compassHeading;
	current_w_cmps -= 360;
	if(current_w_cmps < 0)
		current_w_cmps *= -1;

	current_w = current_w_cmps;

	//simpan value saat ini untuk perhitungan selanjutnya
	odo_global_vx_prev = odo_global_vx;
	odo_global_vy_prev = odo_global_vy;
	odo_global_w_prev = odo_global_w;
}

void calculatePID(){

	pid_error_x = tra_input_x - current_x;									//hitung error pid
	pid_proportional_x = pid_error_x * KP0;									//hitung proportional output
	pid_integral_x += KI0 * (pid_error_x * time_sampling);					//hitung integral output
//	pid_integral_x = LIMITOUT(pid_integral_x,min_pid_limit,max_pid_limit);	//batasi hasil integral
	pid_derivative_x = KD0 * ((tra_current_x - last_tra_x) / time_sampling);//hitung derivative output
//	pid_derivative_x = KD0 * ((pid_error_x - last_tra_x) / time_sampling);
//	last_tra_x = pid_error_x;
	pid_current_x = pid_proportional_x - pid_derivative_x + pid_integral_x;	//hitung total pid
//	pid_current_x = LIMITOUT(pid_current_x,min_pid_limit,max_pid_limit);	//batasi total pid
	last_tra_x = tra_current_x;												//simpan trajectory sekarang untuk perhitungan setelah ini

	pid_error_y = tra_input_y - current_y;
	pid_proportional_y = pid_error_y * KP0;
	pid_integral_y += KI0 * (pid_error_y * time_sampling);
//	pid_integral_y = LIMITOUT(pid_integral_y,min_pid_limit,max_pid_limit);
	pid_derivative_y = KD0 * ((tra_current_y - last_tra_y) / time_sampling);
//	pid_derivative_y = KD0 * ((pid_error_y - last_tra_y) / time_sampling);
//	last_tra_y = pid_error_y;
	pid_current_y = pid_proportional_y - pid_derivative_y + pid_integral_y;
//	pid_current_y = LIMITOUT(pid_current_y,min_pid_limit,max_pid_limit);
	last_tra_y = tra_current_y;

	pid_error_w = tra_input_w - current_w;
	pid_proportional_w = pid_error_w * KP_W;
	pid_integral_w += KI_W * (pid_error_w * time_sampling);
//	pid_integral_w = LIMITOUT(pid_integral_w,min_pid_limit,max_pid_limit);
	pid_derivative_w = KD_W * ((tra_current_w - last_tra_w) / time_sampling);
//	pid_derivative_w = KD_W * ((pid_error_w - last_tra_w) / time_sampling);
//	last_tra_w = pid_error_w;
	pid_current_w = pid_proportional_w - pid_derivative_w + pid_integral_w;
//	pid_current_w = LIMITOUT(pid_current_w,min_pid_limit,max_pid_limit);
	last_tra_w = tra_current_w;
}

void kinematic(){

	kine_local_vx = (cos(current_w2)*pid_current_x) + (sin(current_w2)*pid_current_y);
	kine_local_vy = (cos(current_w2)*pid_current_y) - (sin(current_w2)*pid_current_x);
	kine_local_w = pid_current_w;

	//hitung kecepatan roda dari koordinat lokal 45

	speed_1_value = 0 - kine_local_vx - kine_local_vy;
	speed_2_value = kine_local_vx - kine_local_vy;
	speed_3_value = kine_local_vx + kine_local_vy;
	speed_4_value = 0 - kine_local_vx + kine_local_vy;

	//hitung kecepatan roda dari koordinat lokal
//	speed_1_value = robot_radius*kine_local_w + kine_local_vy;
//	speed_2_value = robot_radius*kine_local_w - kine_local_vx;
//	speed_3_value = robot_radius*kine_local_w - kine_local_vy;
//	speed_4_value = robot_radius*kine_local_w + kine_local_vx;

	//hitung pwm
	pwm_1_value = speed_1_value; // wheel_radius;
	pwm_2_value = speed_2_value; // wheel_radius;
	pwm_3_value = speed_3_value; // wheel_radius;
	pwm_4_value = speed_4_value; // wheel_radius;

	output = pwm_2_value;
	outputDua = pwm_1_value;
	outputTiga = pwm_4_value;
	outputEmpat = pwm_3_value;
}

int motorOutput()
{
	return outputDua;
}

void motorSpeed(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed) {
	//these two variables are interchangeably, depending on robot's need
	motor = 0;
	setPointSatu = leftBackSpeed;
	setPointDua = leftFrontSpeed;
	setPointTiga = righFrontSpeed;
	setPointEmpat = rightBackSpeed;

}

void motorSpeed1(int16_t leftBackSpeed, int16_t leftFrontSpeed, int16_t righFrontSpeed, int rightBackSpeed) {
	//these two variables are interchangeably, depending on robot's need
	motor = 1;
	setPointSatu = leftBackSpeed;
	setPointDua = leftFrontSpeed;
	setPointTiga = righFrontSpeed;
	setPointEmpat = rightBackSpeed;
}


void maju(int32_t speed)
{
	motorDC(1, -speed);
	motorDC(2, -speed);
	motorDC(3, speed);
	motorDC(4, speed);
}

void mundur(int32_t speed)
{
	motorDC(1, speed);
	motorDC(2, speed);
	motorDC(3, -speed);
	motorDC(4, -speed);
}

void kanan(int32_t speed)
{
	motorDC(1, speed);
	motorDC(2, -speed);
	motorDC(3, -speed);
	motorDC(4, speed);
}

void kiri(int32_t speed)
{
	motorDC(1, -speed);
	motorDC(2, speed);
	motorDC(3, speed);
	motorDC(4, -speed);
}

void rotate(int32_t speed)
{
	motorDC(1, speed);
	motorDC(2, speed);
	motorDC(3, speed);
	motorDC(4, speed);
}

void stop()
{
	traInit(0,0,0,0);
	motorDC(1, 0);
	motorDC(2, 0);
	motorDC(3, 0);
	motorDC(4, 0);
}


void tesMotor(int rpm){
	maju(rpm);
	Delayms(2000);
	mundur(rpm);
	Delayms(2000);
	kiri(rpm);
	Delayms(2000);
	kanan(rpm);
	Delayms(2000);
	baratLaut(rpm);
	Delayms(2000);
	tenggara(rpm);
	Delayms(2000);
	baratDaya(rpm);
	Delayms(2000);
	timurLaut(rpm);
	Delayms(2000);
	rotateAntiClockWise(rpm);
	Delayms(2000);
	rotateClockWise(rpm);
	Delayms(2000);
}



