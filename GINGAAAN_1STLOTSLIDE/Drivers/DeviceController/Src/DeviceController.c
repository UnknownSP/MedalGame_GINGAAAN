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

static int SensorCountR[5] = {0};
static int SensorCountL[5] = {0};

static
volatile bool had_completed_tx = true;
static
volatile bool had_completed_rx = true;

static bool _send = false;

static bool _firstSend = false;
static int receiveFaultCount = 0;

static SliderLightingMode rightLightingMode = SL_NORMAL;
static SliderLightingMode leftLightingMode = SL_NORMAL;
static uint8_t inPocketR = 0;
static uint8_t inPocketL = 0;

void D_Slider_SystickUpdate(){
	static uint32_t recent_System_counter = 0;
	static uint32_t sndTime = 0;
	static uint32_t rcvTime = 0;

	static uint8_t count = 0;

	sndTime += G_System_counter - recent_System_counter;
	rcvTime += G_System_counter - recent_System_counter;
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
		if(count >= 5) count = 0;
	}

	if(rcvTime >= SLIDER_RECEIVE_INTERVAL){
		rcvTime = 0;
		D_Slider_Receive((uint8_t*)RecvData, 10);
	}

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

void D_Slider_SetLighting(SliderLightingMode rightType, SliderLightingMode leftType, int pocketR, int pocketL){
	rightLightingMode = rightType;
	inPocketR = pocketR;
	leftLightingMode = leftType;
	inPocketL = pocketL;
	/*
	if(rightType = SL_POCKET_IN){
		switch(inPocketR){
		case 1:
			rightType = SL_POCKET_IN_1;
			break;
		case 2:
			rightType = SL_POCKET_IN_2;
			break;
		case 3:
			rightType = SL_POCKET_IN_3;
			break;
		case 4:
			rightType = SL_POCKET_IN_4;
			break;
		case 5:
			rightType = SL_POCKET_IN_5;
			break;
		}
	}else{
		rightLightingMode = rightType;
	}
	if(leftType = SL_POCKET_IN){
		switch(inPocketL){
		case 1:
			leftType = SL_POCKET_IN_1;
			break;
		case 2:
			leftType = SL_POCKET_IN_2;
			break;
		case 3:
			leftType = SL_POCKET_IN_3;
			break;
		case 4:
			leftType = SL_POCKET_IN_4;
			break;
		case 5:
			leftType = SL_POCKET_IN_5;
			break;
		}
	}else{
		leftLightingMode = leftType;
	}
	*/
}

void D_Slider_LightingUpdate(void){
	static bool slideColorR[5][3] = {{false}};
	static bool slideColorL[5][3] = {{false}};
	switch(rightLightingMode){
	case SL_NORMAL:

		break;
	}
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
		return SensorCountR[num];
	}else{
		return SensorCountL[num];
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
		if(((data[7]>>i) & 1) == 1) SensorCountL[i]++;
	}
	for(int i=0; i<5; i++){
		if(((data[8]>>i) & 1) == 1) SensorCountR[i]++;
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
}

void D_Slider_ReceptionCompletedCallBack(){
	had_completed_rx = true;
	D_Slider_CheckData((uint8_t*)RecvData, 10);
	/*
	if(D_Slider_CheckSum((uint8_t*)RecvData,36) && RecvData[0] != 0){
		for(int i=0; i<32; i++){
			TouchData[i] = RecvData[i+3];
		}
	}
	*/
	//D_USBHID_SetSendData_Touch(TouchData);
}
