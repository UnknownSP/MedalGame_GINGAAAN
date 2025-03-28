/*
 * app.c
 *
 *  Created on: Mar 11, 2023
 *      Author: UnknownSP
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "app.h"

static int SM_CalSpeed(int targetSpeed);
static int SM_CalSpeedFunc1(int nowPos, int minSpeed, int maxSpeed);
static int SM_CalSpeedFunc2(int nowPos, int minSpeed, int maxSpeed);
static int SM_CalSpeedFunc3(int nowPos, int minSpeed, int maxSpeed);
static void SM_1stLot_SetSpeed(int speed, int dir);

static bool SM_Divide_Init(void);
static bool SM_Divide_Move(bool _target);

static int SM_1stLot_speed = 0;
static int SM_1stLot_stepCount = 0;
static int SM_1stLot_SetMaxSpeed = 100;
static uint8_t SM_Lifter_Speed = 10;
static int SM_Lifter_stepCount = 0;
static uint8_t SM_Divide_Speed = 40;
static int SM_Divide_stepCount = 0;

static bool _1stLot_Ena = false;
static bool _Divide_Ena = false;
//初期化
int appInit(void){
	//D_CAN_SetReceiveAddress(8,9,10,11);
	D_CAN_SetReceiveAddressAll();
	for(int i=0; i<8; i++){
		for(int j=0; j<4; j++){
			rcvData[j][i] = 0;
			sndData[j][i] = 0;
		}
	}
	return 0;
}

int appTask(void){
	static int errorHandle = 0;
	static uint32_t recent_System_counter = 0;
	static uint32_t sndTime = 0;
	static uint32_t rcvTime = 0;
	static bool _userButton = false;

	//CANでエンコーダの値を送信
	static int sendLength = 8;
	static int receiveLength = 8;

	static int testSendData = 0;
	static int testSendData2 = 1;
	static int testSendData3 = 0;
	static int testSendData4 = 0;
	static int count1 = 0;
	static int idCount = 8;
	static int ownIdCount = 200;

	static bool _StepEna = false;
	static uint8_t StepSpeed[19] = {
		//66,73,79,80,83,85,88,91,94, 96, 95,94,93,91,88,84,79,73,66
		66,73,79,80,83,85,88,91,94, 96, 94,91,88,85,83,80,79,73,66
	};

	static bool _1stLot_Init = false;
	static bool _Divide_Init = false;

	int diff = G_System_counter - recent_System_counter;
	sndTime += diff;
	rcvTime += diff;
	recent_System_counter = G_System_counter;
	//CAN送信タイミングの場合送信
	if(sndTime >= CAN_SEND_INTERVAL){
		sndTime = 0;
		static int sendLength = 8;
		sndData[0][0] = testSendData;
		sndData[0][1] = testSendData2;
		sndData[0][2] = testSendData2;
		sndData[0][3] = testSendData2;
		sndData[0][4] = testSendData2;
		sndData[0][5] = testSendData2;
		sndData[0][6] = testSendData2;
		sndData[0][7] = testSendData2;
		uint8_t dataSum = 0;
		for(int j=0; j<sendLength-1; j++){
			dataSum += sndData[0][j];
		}
		//int checkSum = 256 - (int)dataSum;
		sndData[0][sendLength-1] = (uint8_t)dataSum;


		errorHandle = D_CAN_Transmit(0x7ff/*testSendData*/,sndData[0],sendLength);
		sndTime = 0;
		count1++;
		if(count1 >= 1 ){//&& errorHandle == 0){
			count1 = 0;
			testSendData++;
		}
		if(testSendData >= 1000){ //&& errorHandle == 0){
			testSendData = 0;
			testSendData2++;
			ownIdCount++;
		}
		if(testSendData2 >= 256 && errorHandle == 0){
			testSendData2 = 1;
			testSendData3++;
		}
		//D_PWM_Set(1,700);
		/*
		if(ownIdCount >= 1024){
			ownIdCount = 0;
			idCount += 4;
			D_CAN_SetReceiveAddress(idCount,idCount+1,idCount+2,idCount+3);
		}
		*/
		/*
		if(testSendData >= 256){
			testSendData = 0;
			ownIdCount++;
		}
		*/
	}
	//CAN受信処理
	D_CAN_Receive(0,rcvData[0],receiveLength);
	D_CAN_Receive(1,rcvData[1],receiveLength);

	/*
	for(int i=0; i<4; i++){
		sndData[i][3] = allHoldPocket;
	}
	*/

	/*
	static int upCount = 0;
	static int speed = 70;
	static int speedCoeff = 1;
	static bool _ChangeSpeedCoeff = false;
	upCount++;
	if(upCount >= 200){
		upCount = 0;
		speed += speedCoeff;
	}
	if(IO_READ_SM_R() || IO_READ_SM_L()){
		_ChangeSpeedCoeff = true;
		speedCoeff = 1;
	}
	if(IO_READ_SM_C() && _ChangeSpeedCoeff){
		_ChangeSpeedCoeff = false;
		speedCoeff = -1;
	}
	if(speed >= 90) speed = 90;
	if(speed < 70) speed = 70;
	*/

	static int speed = 0;
	static int step = 0;
	//speed = SM_CalSpeed(StepSpeed[(int)(step / 100)]);
	speed = SM_CalSpeedFunc3(SM_1stLot_stepCount,10,90);
	SM_1stLot_SetSpeed(speed,1);
	//SM_Set();

	static bool _divideTarget = false;
	if(IO_READ_DIVIDE_BALL_L()) _divideTarget = true;
	if(IO_READ_DIVIDE_BALL_R()) _divideTarget = false;
	if(!_Divide_Init){
		_Divide_Init = SM_Divide_Init();
	}else{
		SM_Divide_Move(_divideTarget);
	}

	//D_Mess_printf("%d\n", G_System_counter);
	//D_PWM_Set(1,100);
	//D_PWM_Set(2,100);
	//IO_SET_BLDC2_DIR();
	if(IO_READ_USERBUTTON()){
		IO_SET_USERLED();
		if(_userButton){
			_userButton = false;
			testSendData += 1;
			if(_StepEna){
				IO_SET_1STLOT_ENA();
				IO_SET_LIFTER_ENA();
				IO_SET_DIVIDE_ENA();
				_StepEna = false;
			}else{
				IO_RESET_1STLOT_ENA();
				IO_RESET_LIFTER_ENA();
				IO_RESET_DIVIDE_ENA();
				_StepEna = true;
			}
		}
		//IO_SET_BLDC1_ENA();
		//IO_SET_BLDC2_ENA();
		//D_PWM_Set(BLDC1,50);
		//D_PWM_Set(BLDC2,50);
		//D_PWM_Set(BLDC3,300);
		//IO_SET_JP_LED();
		//Lottery_1st2nd_SetSpeed(500);
		//Lottery_3rd_SetSpeed(800);
		//Lottery_JP_SetSpeed(300,0);
		//Lottery_JP_SetSpeed(JPC_MAX_SPEED,0);

		//IO_SET_ROOMLIGHT();
		//JP_Lift_Down();
	}else{
		_userButton = true;
		IO_RESET_USERLED();
		//IO_RESET_BLDC1_ENA();
		//IO_RESET_BLDC2_ENA();
		//IO_RESET_BLDC3_ENA();
		//D_PWM_Set(BLDC1,300);
		//D_PWM_Set(BLDC2,3000);
		//D_PWM_Set(BLDC3,3500);
		//JP_Lift_Up();
	}

	/*
	for(int i=0; i<8; i++){
		if(rcvData[0][i] != 0) rcvDataJudge[i] = rcvData[0][i];
	}
	*/
	//デバッグ用
	int16_t debug_bits = 0;
	debug_bits &= 0;
	debug_bits |= ((int)IO_READ_1STLOT_R() << 2);
	debug_bits |= ((int)IO_READ_1STLOT_C() << 1);
	debug_bits |= ((int)IO_READ_1STLOT_L() << 0);

	debug_bits |= ((int)IO_READ_DIVIDE_BALL_R() << 3);
	debug_bits |= ((int)IO_READ_DIVIDE_BALL_L() << 4);
	debug_bits |= ((int)IO_READ_LIFTER_HOME() << 5);
	debug_bits |= ((int)IO_READ_LIFTER_BALL() << 6);
	debug_bits |= ((int)IO_READ_DIVIDE_HOME() << 7);
	//D_Mess_printf("\033[1;1H");
	//D_Mess_printf("testSendData : %02d\n", testSendData);
	//D_Mess_printf("%d\n", G_System_MicroCounter);
	//D_Mess_printf("%08b\n", debug_bits);
	/*
	D_Mess_printf("%02d\n", errorHandle);
	D_Mess_printf("%010d\n", step);
	D_Mess_printf("ownId : %02d\n", ownIdCount);
	D_Mess_printf("ownId : %02d\n", testSendData);
	D_Mess_printf("sndId : %02d\n", idCount);
	D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", sndData[0][0],sndData[0][1],sndData[0][2],sndData[0][3],sndData[0][4],sndData[0][5],sndData[0][6],sndData[0][7]);
	D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", rcvData[0][0],rcvData[0][1],rcvData[0][2],rcvData[0][3],rcvData[0][4],rcvData[0][5],rcvData[0][6],rcvData[0][7]);
	D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", rcvDataJudge[0],rcvDataJudge[1],rcvDataJudge[2],rcvDataJudge[3],rcvDataJudge[4],rcvDataJudge[5],rcvDataJudge[6],rcvDataJudge[7]);
	D_Mess_printf("id : %3d, err : %3d\n", rcvData[1][0],rcvData[1][1]);
	*/
	return 0;
}

static int SM_CalSpeed(int targetSpeed){
	static int recentTargetSpeed = 0;
	static int nowSpeed = 0;
	static int count = 0;
	count++;
	if(count >= 8){
		count = 0;
		if (targetSpeed != recentTargetSpeed) nowSpeed = recentTargetSpeed;
		nowSpeed += (targetSpeed - nowSpeed) > 0 ? 1 : -1;
	}
	recentTargetSpeed = targetSpeed;
	return nowSpeed;
}

static int SM_CalSpeedFunc1(int nowPos, int minSpeed, int maxSpeed){
	double stepHalf = SM_STEPCOUNT / 2.0;
	double x = ((double)nowPos-stepHalf) / stepHalf;
	if (x > 1.0) x = 1.0;
	if (x < -1.0) x = -1.0;
	double y = -x*x + 1.0;
	double edgeVal = 0.93;
	double offsetSpeed = 1;
	double stopTime = 1000;
	static bool _stop = false;
	static bool _stopEna = true;
	static int stopCount = 0;
	if (_stopEna && (IO_READ_SM_R() || IO_READ_SM_L())){
		_stopEna = false;
		_stop = true;
	}
	if(_stop){
		stopCount++;
		if(stopCount >= stopTime){
			_stop = false;
			stopCount = 0;
		}else{
			return 0;
		}
	}
	if (x >= -0.5 && x <= 0.5) _stopEna = true;
	if (x <= -edgeVal || x >= edgeVal){
		return ((1.0 - fabs(x)) / (1.0 - edgeVal)) * (minSpeed - offsetSpeed) + offsetSpeed;
	}
	return minSpeed + (int)((double)(maxSpeed-minSpeed) * y);
}

static int SM_CalSpeedFunc2(int nowPos, int minSpeed, int maxSpeed){
	double stepHalf = SM_STEPCOUNT / 2.0;
	double x = ((double)nowPos-stepHalf) / stepHalf;
	x /= 0.9;
	if (x > 1.0) x = 1.0;
	if (x < -1.0) x = -1.0;
	double y = -2.0 * x*x*x*x * (x*x-1.0) + 0.7;
	double edgeVal = 0.98;
	double offsetSpeed = 1;
	double stopTime = 500;
	static bool _stop = false;
	static bool _stopEna = true;
	static int stopCount = 0;
	if (_stopEna && (IO_READ_SM_R() || IO_READ_SM_L())){
		_stopEna = false;
		_stop = true;
	}
	if(_stop){
		stopCount++;
		if(stopCount >= stopTime){
			_stop = false;
			stopCount = 0;
		}else{
			return 0;
		}
	}
	if (x >= -0.5 && x <= 0.5) _stopEna = true;
	if (x <= -edgeVal || x >= edgeVal){
		return ((1.0 - fabs(x)) / (1.0 - edgeVal)) * (minSpeed - offsetSpeed) + offsetSpeed;
	}
	return minSpeed + (int)((double)(maxSpeed-minSpeed) * y);
}

static int SM_CalSpeedFunc3(int nowPos, int minSpeed, int maxSpeed){
	double stepHalf = SM_STEPCOUNT / 2.0;
	double x = ((double)nowPos-stepHalf) / stepHalf;
	if (x > 1.0) x = 1.0;
	if (x < -1.0) x = -1.0;
	double y = -0.22 * cos(x/0.15)*exp(x*x) + 0.6;
	double edgeVal = 0.99;
	double offsetSpeed = 1;
	double stopTime = 500;
	static bool _stop = false;
	static bool _stopEna = true;
	static int stopCount = 0;
	if (_stopEna && (IO_READ_1STLOT_R() || IO_READ_1STLOT_L())){
		_stopEna = false;
		_stop = true;
	}
	if(_stop){
		stopCount++;
		if(stopCount >= stopTime){
			_stop = false;
			stopCount = 0;
		}else{
			return 0;
		}
	}
	if (x >= -0.5 && x <= 0.5) _stopEna = true;
	if (x <= -edgeVal || x >= edgeVal){
		return ((1.0 - fabs(x)) / (1.0 - edgeVal)) * (minSpeed - offsetSpeed) + offsetSpeed;
	}
	return minSpeed + (int)((double)(maxSpeed-minSpeed) * y);
}

void SM_1stLot_Set(void){
	static bool _StepM = true;
	static int counter = 0;
	counter++;
	if(!_StepM || SM_1stLot_speed == 0){
		_StepM = true;
		IO_RESET_1STLOT_STEP();
		return;
	}
	if(counter >= SM_1stLot_SetMaxSpeed - SM_1stLot_speed){
		counter = 0;
		SM_1stLot_stepCount++;
		_StepM = false;
		IO_SET_1STLOT_STEP();
	}
	/*
	static bool _set = false;
	if(_set){
		IO_SET_STEP();
		_set = false;
	}else{
		_set = true;
		IO_RESET_STEP();
	}
	*/
	/*
	static int step = 0;
	static int speed = 0;
	speed = SM_CalSpeedFunc3(step,10,90);
	step = SM1st_SetSpeed(speed,1);
	*/
}

static void SM_1stLot_SetSpeed(int speed, int dir){
	static bool _StepDir = false;
	if(speed >= SM_1stLot_SetMaxSpeed) speed = SM_1stLot_SetMaxSpeed;
	if(speed < 0) speed = 0;
	SM_1stLot_speed = speed;
	if(_StepDir){
		if(IO_READ_1STLOT_L()){
			IO_RESET_1STLOT_DIR();
			_StepDir = false;
			SM_1stLot_stepCount = 0;
		}else{
			IO_SET_1STLOT_DIR();
		}
	}else{
		if(IO_READ_1STLOT_R()){
			IO_SET_1STLOT_DIR();
			_StepDir = true;
			SM_1stLot_stepCount = 0;
		}else{
			IO_RESET_1STLOT_DIR();
		}
	}

	/*
	static uint32_t recent_System_counter = 0;
	static uint32_t SMsendTime = 0;
	static bool _StepM = false;
	static bool _StepDir = false;
	static uint32_t stepCount = 0;
	if(speed >= 100) speed = 100;
	if(speed < 0) speed = 0;
	SMsendTime += G_System_MicroCounter - recent_System_counter;
	recent_System_counter = G_System_MicroCounter;
	if(!_StepM){
		_StepM = true;
		IO_RESET_STEP();
		return stepCount;
	}
	if (speed == 0) return stepCount;
	if(SMsendTime >= SM_SEND_INTERVAL + (100 - speed)){
		SMsendTime = 0;
		stepCount++;
		if(_StepM){
			_StepM = false;
			IO_SET_STEP();
		}
		if(_StepDir){
			if(IO_READ_SM_L()){
				IO_RESET_DIR();
				_StepDir = false;
				stepCount = 0;
			}else{
				IO_SET_DIR();
			}
		}else{
			if(IO_READ_SM_R()){
				IO_SET_DIR();
				_StepDir = true;
				stepCount = 0;
			}else{
				IO_RESET_DIR();
			}
		}
	}
	*/
	//return;
}


void SM_Lifter_Set(void){
	static bool _StepM = true;
	static uint8_t counter = 0;
	counter++;
	if(!_StepM){
		_StepM = true;
		IO_RESET_LIFTER_STEP();
		return;
	}
	if(counter >= SM_Lifter_Speed){
		counter = 0;
		SM_Lifter_stepCount++;
		_StepM = false;
		IO_SET_LIFTER_STEP();
	}
}

void SM_Divide_Set(void){
	static bool _StepM = true;
	static uint8_t counter = 0;
	counter++;
	if(!_StepM){
		_StepM = true;
		IO_RESET_DIVIDE_STEP();
		return;
	}
	if(!_Divide_Ena) return;
	if(counter >= SM_Divide_Speed){
		counter = 0;
		SM_Divide_stepCount++;
		_StepM = false;
		IO_SET_DIVIDE_STEP();
	}
}

static bool SM_Divide_Move(bool _target){
	static bool _recentPos = false;
	static bool _complete = false;
	if(_recentPos != _target){
		if(_target){
			IO_RESET_DIVIDE_DIR();
		}else{
			IO_SET_DIVIDE_DIR();
		}
		_complete = false;
		SM_Divide_stepCount = 0;
		IO_SET_DIVIDE_ENA();
		_Divide_Ena = true;
	}
	_recentPos = _target;
	if(_complete) return true;
	if(_target){
		if(SM_Divide_stepCount >= SM_DIVIDE_R_STEP) _complete = true;
	}else{
		if(IO_READ_DIVIDE_HOME()) _complete = true;
	}
	if(_complete){
		IO_RESET_DIVIDE_ENA();
		_Divide_Ena = false;
		SM_Divide_stepCount = 0;
	}
	return _complete;
}

static bool SM_Divide_Init(void){
	static bool _first = false;
	if(!_first){
		_first = true;
		IO_SET_DIVIDE_ENA();
		IO_SET_DIVIDE_DIR();
		_Divide_Ena = true;
	}
	if(IO_READ_DIVIDE_HOME()){
		IO_RESET_DIVIDE_ENA();
		IO_RESET_DIVIDE_DIR();
		_Divide_Ena = false;
		SM_Divide_stepCount = 0;
		_first = false;
		return true;
	}
	return false;
}

