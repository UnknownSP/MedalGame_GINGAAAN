/*
 * DeviceController.h
 *
 *  Created on: Mar 7, 2024
 *      Author: UnknownSP
 */

#ifndef DEVICECONTROLLER_INC_DEVICECONTROLLER_H_
#define DEVICECONTROLLER_INC_DEVICECONTROLLER_H_

#include "main.h"

int D_Slider_Send(uint8_t* data, int length);
int D_Slider_Receive(uint8_t* data, int length);

int D_Slider_GetSensorData(int side, int num);
void D_Slider_CheckData(uint8_t* data, int length);
bool D_Slider_CheckSum(uint8_t* data, int length);
void D_Slider_GetTouch(bool* setData);
void D_Slider_SetColorData(bool setColorR[][3], bool setColorL[][3]);

void D_Slider_TransitionCompletedCallBack();
void D_Slider_ReceptionCompletedCallBack();

void D_Slider_SystickUpdate();

#define SLIDER_SEND_INTERVAL 20
#define SLIDER_RECEIVE_INTERVAL 10

#define ESCAPE_PACKET 208 //the value after this packet will add 1 value

#define FIRSTBYTE 224
#define ADDRESS 1
#define SENSORGET_SEQNUM 1
#define COLORSEND_SEQNUM 5
#define SENSORGET_CMD 32
#define COLORSEND_CMD 50
#define SENSORGET_DATANUM 1
#define COLORSEND_DATANUM 5
#define SENSORGET_LENGTH 6
#define SENSORGET_RECEIVE_LENGTH 10
#define COLORSEND_LENGTH 10

#endif /* DEVICECONTROLLER_INC_DEVICECONTROLLER_H_ */
