/*
 * DeviceController.c
 *
 *  Created on: Mar 7, 2024
 *      Author: UnknownSP
 */
#include "../Inc/DeviceController.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"

static uint8_t SendData[32] = {0};
static uint8_t SensorGetSendData[32] = {0};
static uint8_t AdditionalPacket = 0;
static uint8_t RecvData[32] = {0};

static int SensorCount[2][5] = {{0}};

static
volatile bool had_completed_tx = true;
static
volatile bool had_completed_rx = true;

static bool _send = false;

static bool _firstSend = false;
static int receiveFaultCount = 0;

static SliderLightingMode LightingMode[2] = {SL_DISABLE};
static uint8_t inPocket[2] = {0};

//static volatile uint32_t SndErrTime = 0;
//static volatile uint32_t RcvErrTime = 0;

//static volatile int SndCount = 0;

void D_Slider_SystickUpdate(){
	static uint32_t recent_System_counter = 0;
	static uint32_t sndTime = 0;
	static uint32_t rcvTime = 0;

	static uint8_t count = 0;

	int deltaT = G_System_counter - recent_System_counter;
	sndTime += deltaT;
	rcvTime += deltaT;
	//SndErrTime += deltaT;
	//RcvErrTime += deltaT;
	recent_System_counter = G_System_counter;

	if(!_firstSend){
		_firstSend = true;
		D_Slider_Start();
		//D_Slider_InitColorData();
		return;
	}
	if(sndTime >= SLIDER_SEND_INTERVAL){
		sndTime = 0;
		count++;
		if(count == 1){
			D_Slider_Send((uint8_t*)SendData, COLORSEND_LENGTH + AdditionalPacket);
		}else{
			D_Slider_Send((uint8_t*)SensorGetSendData, SENSORGET_LENGTH);
		}
		if(count >= 10) count = 0;
	}

	if(rcvTime >= SLIDER_RECEIVE_INTERVAL){
		rcvTime = 0;
		D_Slider_Receive((uint8_t*)RecvData, 10);
	}

	/*
	if(RcvErrTime >= 100){
		//D_Slider_Send((uint8_t*)SensorGetSendData, SENSORGET_LENGTH);
		//D_Slider_Receive((uint8_t*)RecvData, 10);
		RcvErrTime = 0;
	}
	*/

	D_Slider_LightingUpdate();
	/*
	if(D_Slider_Receive((uint8_t*)RecvData, 36) == -1){
		receiveFaultCount++;
	}else{
		receiveFaultCount = 0;
	}
	if(receiveFaultCount >= 30){
		D_Slider_Start();
		receiveFaultCount = 0;
	}
	*/
}

void D_Slider_Start(void){
	SensorGetSendData[0] = FIRSTBYTE;
	SensorGetSendData[1] = ADDRESS;
	SensorGetSendData[2] = SENSORGET_SEQNUM;
	SensorGetSendData[3] = SENSORGET_DATANUM;
	SensorGetSendData[4] = SENSORGET_CMD;
	uint8_t dataSum = 0;
	for(int i=1; i<SENSORGET_LENGTH-1; i++){
		dataSum += SensorGetSendData[i];
	}
	SensorGetSendData[SENSORGET_LENGTH-1] = (uint8_t)dataSum;
	/*
	static uint8_t data[4];
	data[0] = 0xff;
	data[1] = 0x03;
	data[2] = 0x00;
	data[3] = 0xfe;
	had_completed_tx = true;
	had_completed_rx = true;
	D_Slider_Send((uint8_t*)data, 4);
	*/
}

void D_Slider_SetLightingR(SliderLightingMode rightType, int pocketR){
	//LightingMode[0] = rightType;
	inPocket[0] = pocketR;
	if(rightType == SL_POCKET_IN){
		switch(pocketR){
		case 0:
			LightingMode[0] = SL_POCKET_IN_OUT;
			break;
		case 1:
			LightingMode[0] = SL_POCKET_IN_DIA;
			break;
		case 2:
			LightingMode[0] = SL_POCKET_IN_UP;
			break;
		case 3:
			LightingMode[0] = SL_POCKET_IN_DIA;
			break;
		case 4:
			LightingMode[0] = SL_POCKET_IN_OUT;
			break;
		}
	}else{
		LightingMode[0] = rightType;
	}
}

void D_Slider_SetLightingL(SliderLightingMode leftType, int pocketL){
	//LightingMode[1] = leftType;
	inPocket[1] = pocketL;
	if(leftType == SL_POCKET_IN){
		switch(pocketL){
		case 0:
			LightingMode[1] = SL_POCKET_IN_OUT;
			break;
		case 1:
			LightingMode[1] = SL_POCKET_IN_DIA;
			break;
		case 2:
			LightingMode[1] = SL_POCKET_IN_UP;
			break;
		case 3:
			LightingMode[1] = SL_POCKET_IN_DIA;
			break;
		case 4:
			LightingMode[1] = SL_POCKET_IN_OUT;
			break;
		}
	}else{
		LightingMode[1] = leftType;
	}
}

void D_Slider_LightingUpdate(void){
	static bool slideColor[2][5][3] = {{{false}}};
	static int updateCount[2] = {0};
	static int recentUpdateCount = 0;
	static SliderLightingMode recentMode[2] = {SL_DISABLE};
	static uint8_t recentInPocket[2] = {0};
	static bool _changeMode[2] = {false};
	static bool _animUpdate_1[2] = {false};
	static bool _animUpdate_2[2] = {false};
	static int counter1[2] = {0};

	int nowCount = G_System_counter;

	for(int side = 0; side < 2; side++){
		updateCount[side] += nowCount - recentUpdateCount;
		if(LightingMode[side] != recentMode[side] || inPocket[side] != recentInPocket[side]){
			_changeMode[side] = true;
			updateCount[side] = 0;
			_animUpdate_1[side] = false;
			_animUpdate_2[side] = false;
		}
		recentMode[side] = LightingMode[side];
		recentInPocket[side] = inPocket[side];
		switch(LightingMode[side]){
		case SL_DISABLE:
			if(_changeMode[side]){
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = false;
					slideColor[side][i][1] = false;
					slideColor[side][i][2] = false;
				}
				_changeMode[side] = false;
			}
			break;

		case SL_NORMAL:
			if(_changeMode[side]){
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = true;
					slideColor[side][i][1] = true;
					slideColor[side][i][2] = true;
				}
				_changeMode[side] = false;
			}
			break;

		case SL_NORMAL_FLASH:
			if(updateCount[side] <= 1700 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = true;
					slideColor[side][i][1] = true;
					slideColor[side][i][2] = true;
				}
			}
			if(updateCount[side] > 1700 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = false;
					slideColor[side][i][1] = false;
					slideColor[side][i][2] = false;
				}
			}
			if(updateCount[side] >= 2000) updateCount[side] = 0;
			break;

		case SL_POCKET_IN_OUT:
			if(_changeMode[side]){
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = false;
					slideColor[side][i][1] = false;
					slideColor[side][i][2] = false;
				}
				_changeMode[side] = false;
			}
			if(updateCount[side] < 100 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 100 && updateCount[side] < 200 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = false;
				slideColor[side][inPocket[side]][2] = false;
			}
			if(updateCount[side] >= 200 && updateCount[side] < 375 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 375 &&updateCount[side] < 475 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = false;
				slideColor[side][inPocket[side]][2] = false;
			}
			if(updateCount[side] >= 475 &&updateCount[side] < 650 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}

			if(updateCount[side] > 650) LightingMode[side] = SL_NORMAL_FLASH;

			break;

		case SL_POCKET_IN_DIA:
			if(_changeMode[side]){
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = false;
					slideColor[side][i][1] = false;
					slideColor[side][i][2] = false;
				}
				_changeMode[side] = false;
			}
			if(updateCount[side] < 100 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 100 && updateCount[side] < 180 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 180 && updateCount[side] < 300 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 300 &&updateCount[side] < 380 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 380 &&updateCount[side] < 500 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 500 &&updateCount[side] < 580 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 580 &&updateCount[side] < 700 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = true;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}

			if(updateCount[side] > 700) LightingMode[side] = SL_NORMAL_FLASH;

			break;

		case SL_POCKET_IN_UP:
			if(_changeMode[side]){
				for(int i=0; i<5; i++){
					slideColor[side][i][0] = false;
					slideColor[side][i][1] = false;
					slideColor[side][i][2] = false;
				}
				_changeMode[side] = false;
				counter1[side] = 0;
			}
			if(updateCount[side] >= 100 + counter1[side]*200 && updateCount[side] < 180 + counter1[side]*200 && !_animUpdate_1[side]){
				_animUpdate_1[side] = true;
				_animUpdate_2[side] = false;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = true;
				slideColor[side][inPocket[side]][2] = true;
			}
			if(updateCount[side] >= 180 + counter1[side]*200 && updateCount[side] < 300 + counter1[side]*200 && !_animUpdate_2[side]){
				_animUpdate_1[side] = false;
				_animUpdate_2[side] = true;
				slideColor[side][inPocket[side]][0] = false;
				slideColor[side][inPocket[side]][1] = false;
				slideColor[side][inPocket[side]][2] = false;
				counter1[side]++;
			}

			if(updateCount[side] > 2000) LightingMode[side] = SL_NORMAL_FLASH;

			break;
		}
	}

	recentUpdateCount = G_System_counter;
	D_Slider_SetColorData(slideColor[0], slideColor[1]);
}

int D_Slider_GetPocketR(bool _reset){
	static int recentSensorCount[5] = {{0}};
	static int returnPocket = -1;
	if (_reset) {
		returnPocket = -1;
		return returnPocket;
	}
	for(int i=0; i<5; i++){
		if(SensorCount[0][i] != recentSensorCount[i]){
			returnPocket = i;
		}
		recentSensorCount[i] = SensorCount[0][i];
	}
	return returnPocket;
}

int D_Slider_GetPocketL(bool _reset){
	static int recentSensorCount[5] = {{0}};
	static int returnPocket = -1;
	if (_reset) {
		returnPocket = -1;
		return returnPocket;
	}
	for(int i=0; i<5; i++){
		if(SensorCount[1][i] != recentSensorCount[i]){
			returnPocket = i;
		}
		recentSensorCount[i] = SensorCount[1][i];
	}
	return returnPocket;
}

int D_Slider_Send(uint8_t* data, int length){
	if(!had_completed_tx){
		return -1;
	}
	HAL_UART_Transmit_DMA(&huart1, (uint8_t*)data, length);
	_send = true;
	had_completed_tx = false;
	return 0;
}

int D_Slider_Receive(uint8_t* data, int length){
	if(!had_completed_rx){
		return -1;
	}
	HAL_UART_Receive_DMA(&huart1, (uint8_t*)data, length);
	had_completed_rx = false;
	return 0;
}

int D_Slider_GetSensorData(int side, int num){
	if(side == 0){
		return SensorCount[0][num];
	}else{
		return SensorCount[1][num];
	}
}

void D_Slider_CheckData(uint8_t* data, int length){
	if(data[0] != FIRSTBYTE || data[1] != SENSORGET_SEQNUM || data[2] != ADDRESS) return;
	uint8_t sum = 0;
	for(int i=1; i<SENSORGET_RECEIVE_LENGTH-1; i++){
		sum += data[i];
	}
	if(sum != data[SENSORGET_RECEIVE_LENGTH-1]) return;
	for(int i=0; i<5; i++){
		if(((data[7]>>i) & 1) == 1) SensorCount[1][i]++;
		if(((data[8]>>i) & 1) == 1) SensorCount[0][i]++;
	}
}

void D_Slider_SetColorData(bool setColorR[][3], bool setColorL[][3]){
	if (!_send) return;
	_send = false;
	SendData[0] = FIRSTBYTE;
	SendData[1] = ADDRESS;
	SendData[2] = COLORSEND_SEQNUM;
	SendData[3] = COLORSEND_DATANUM;
	SendData[4] = COLORSEND_CMD;
	SendData[5] = 0;
	SendData[6] = 0;
	SendData[7] = 0;
	SendData[8] = 0;
	SendData[9] = 0;
	SendData[10] = 0;
	SendData[11] = 0;
	SendData[12] = 0;
	SendData[13] = 0;
	SendData[14] = 0;
	for(int i=1; i<16; i++){
		SendData[5 + (i/8)] |= (setColorR[4 - (i-1)/3][(i-1)%3] ? 1 : 0) << (i%8);
	}
	for(int i=1; i<16; i++){
		SendData[7 + (i/8)] |= (setColorL[4 - (i-1)/3][(i-1)%3] ? 1 : 0) << (i%8);
	}

	uint8_t dataSum = 0;
	for(int i=1; i<COLORSEND_LENGTH-1; i++){
		dataSum += SendData[i];
	}

	AdditionalPacket = 0;

	/*
	if(SendData[6] == ESCAPE_PACKET || SendData[6] == FIRSTBYTE){
		AdditionalPacket++;
		uint8_t tempData = SendData[6] - 1;
		SendData[6] = ESCAPE_PACKET;
		SendData[9] = SendData[8];
		SendData[8] = SendData[7];
		SendData[7] = tempData;
	}

	if(SendData[8] == ESCAPE_PACKET || SendData[8] == FIRSTBYTE){
		AdditionalPacket++;
		uint8_t tempData = SendData[8] - 1;
		SendData[8] = ESCAPE_PACKET;
		//SendData[10] = SendData[9];
		//SendData[9] = SendData[8];
		SendData[9] = tempData;
	}

	if(SendData[9] == ESCAPE_PACKET || SendData[9] == FIRSTBYTE){
		AdditionalPacket++;
		uint8_t tempData = SendData[9] - 1;
		SendData[9] = ESCAPE_PACKET;
		//SendData[10] = SendData[9];
		//SendData[8] = SendData[7];
		SendData[10] = tempData;
	}
	*/
	for(int i=5; i<9+AdditionalPacket; i++){
		if(SendData[i] == ESCAPE_PACKET || SendData[i] == FIRSTBYTE) {
			AdditionalPacket++;
			uint8_t tempData = SendData[i] - 1;
			SendData[i] = ESCAPE_PACKET;
			for(int j=i+3; j>i; j--){
				SendData[j+1] = SendData[j];
			}
			SendData[i+1] = tempData;
			i++;
		}
	}

	SendData[COLORSEND_LENGTH-1 + AdditionalPacket] = (uint8_t)dataSum;
}


void D_Slider_TransitionCompletedCallBack(){
	had_completed_tx = true;
	//SndErrTime = 0;
}

void D_Slider_ReceptionCompletedCallBack(){
	had_completed_rx = true;
	//RcvErrTime = 0;
	D_Slider_CheckData((uint8_t*)RecvData, 10);
	/*
	SndCount++;
	if(SndCount == 1){
		D_Slider_Send((uint8_t*)SendData, COLORSEND_LENGTH + AdditionalPacket);
	}else{
		D_Slider_Send((uint8_t*)SensorGetSendData, SENSORGET_LENGTH);
	}
	if(SndCount >= 10) SndCount = 0;
	*/
	/*
	if(D_Slider_CheckSum((uint8_t*)RecvData,36) && RecvData[0] != 0){
		for(int i=0; i<32; i++){
			TouchData[i] = RecvData[i+3];
		}
	}
	*/
	//D_USBHID_SetSendData_Touch(TouchData);
}
