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
	D_PWM_Init();
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

	/*
	for(int i=0; i<4; i++){
		sndData[i][0] = 255-testSendData;
		sndData[i][1] = testSendData;
		sndData[i][2] = 255-testSendData;
		sndData[i][3] = testSendData;
		sndData[i][4] = 255-testSendData;
		sndData[i][5] = testSendData;
		sndData[i][6] = 255-testSendData;
		sndData[i][7] = testSendData;
		uint8_t dataSum = 0;
		for(int j=0; j<sendLength-1; j++){
			dataSum += sndData[i][j];
		}
		int checkSum = 256 - (int)dataSum;
		sndData[i][sendLength-1] = (uint8_t)checkSum;
	}
	*/

	//SMsndTime += G_System_MicroCounter - recent_System_counter;
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
			//testSendData++;
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

	static bool _first = false;

	if(!_first){
		D_Slider_SetLightingR(SL_NORMAL_FLASH,0);
		D_Slider_SetLightingL(SL_NORMAL_FLASH,0);
		_first = true;
	}
	int pocketR = D_Slider_GetPocketR(false);
	if(pocketR != -1){
		D_Slider_SetLightingR(SL_POCKET_IN,pocketR);
		D_Slider_GetPocketR(true);
	}
	int pocketL = D_Slider_GetPocketL(false);
	if(pocketL != -1){
		D_Slider_SetLightingL(SL_POCKET_IN,pocketL);
		D_Slider_GetPocketL(true);
	}



	//D_Mess_printf("%d\n", G_System_counter);
	//D_PWM_Set(1,100);
	//D_PWM_Set(2,100);
	//IO_SET_BLDC2_DIR();
	if(IO_READ_USERBUTTON()){
		IO_SET_USERLED();
		if(_userButton){
			_userButton = false;
			testSendData += 100;
		}
		/*
		for(int i=1; i<7; i++){
			D_PWM_Set(i,1000);
		}
		*/
		IO_SET_SHOOTER_R();
		IO_SET_SHOOTER_L();
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
		/*
		for(int i=1; i<7; i++){
			D_PWM_Set(i,0);
		}
		*/
		IO_RESET_SHOOTER_R();
		IO_RESET_SHOOTER_L();
		//IO_RESET_BLDC1_ENA();
		//IO_RESET_BLDC2_ENA();
		//IO_RESET_BLDC3_ENA();
		//D_PWM_Set(BLDC1,300);
		//D_PWM_Set(BLDC2,3000);
		//D_PWM_Set(BLDC3,3500);
		//JP_Lift_Up();
	}

	if(IO_READ_WAITBALL_R()){
		D_PWM_Set(LED_RIGHT_R, testSendData);
	}else{
		D_PWM_Set(LED_RIGHT_R,0);
	}
	if(IO_READ_SHOOTCOUNT_R()){
		D_PWM_Set(LED_RIGHT_G, testSendData);
	}else{
		D_PWM_Set(LED_RIGHT_G,0);
	}
	if(IO_READ_SOLENOID_R()){
		D_PWM_Set(LED_RIGHT_B, testSendData);
	}else{
		D_PWM_Set(LED_RIGHT_B,0);
	}

	if(IO_READ_WAITBALL_L()){
		D_PWM_Set(LED_LEFT_R, testSendData);
	}else{
		D_PWM_Set(LED_LEFT_R,0);
	}
	if(IO_READ_SHOOTCOUNT_L()){
		D_PWM_Set(LED_LEFT_G, testSendData);
	}else{
		D_PWM_Set(LED_LEFT_G,0);
	}
	if(IO_READ_SOLENOID_L()){
		D_PWM_Set(LED_LEFT_B, testSendData);
	}else{
		D_PWM_Set(LED_LEFT_B,0);
	}

	for(int i=0; i<8; i++){
		if(rcvData[0][i] != 0) rcvDataJudge[i] = rcvData[0][i];
	}
	//デバッグ用
	int16_t debug_bits = 0;
	debug_bits &= 0;
	debug_bits |= ((int)IO_READ_SM_R() << 2);
	debug_bits |= ((int)IO_READ_SM_C() << 1);
	debug_bits |= ((int)IO_READ_SM_L() << 0);
	//D_Mess_printf("\033[1;1H");
	//D_Mess_printf("testSendData : %02d\n", testSendData);
	//D_Mess_printf("%d\n", G_System_MicroCounter);
	//D_Mess_printf("%03b\n", debug_bits);
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


