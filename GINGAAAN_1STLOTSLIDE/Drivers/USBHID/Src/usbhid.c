/*
 * usbhid.c
 *
 *  Created on: Mar 6, 2024
 *      Author: UnknownSP
 */


#include "../Inc/usbhid.h"
#include <stdlib.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"
#include "usb_device.h"

static struct keyboardHID_t keyboardHID;

const uint8_t KeyboardOrder[38] = {
	KEYBOARD_I,
	KEYBOARD_COMMA,
	KEYBOARD_8,
	KEYBOARD_K,
	KEYBOARD_U,
	KEYBOARD_M,
	KEYBOARD_7,
	KEYBOARD_J,
	KEYBOARD_Y,
	KEYBOARD_N,
	KEYBOARD_6,
	KEYBOARD_H,
	KEYBOARD_T,
	KEYBOARD_B,
	KEYBOARD_5,
	KEYBOARD_G,
	KEYBOARD_R,
	KEYBOARD_V,
	KEYBOARD_4,
	KEYBOARD_F,
	KEYBOARD_E,
	KEYBOARD_C,
	KEYBOARD_3,
	KEYBOARD_D,
	KEYBOARD_W,
	KEYBOARD_X,
	KEYBOARD_2,
	KEYBOARD_S,
	KEYBOARD_Q,
	KEYBOARD_Z,
	KEYBOARD_1,
	KEYBOARD_A,
	KEYBOARD_ATMARK,
	KEYBOARD_LEFTSQUAREBRACKETS,
	KEYBOARD_SEMICOLON,
	KEYBOARD_COLON,
	KEYBOARD_PERIOD,
	KEYBOARD_SLASH,
};

void D_USBHID_Init(){
	keyboardHID.modifiers = 0;
	keyboardHID.reserved = 0;
	for(int i = 0 ;i < SENDDATA_SIZE; i++){
		keyboardHID.key[i] = 0;
	}
}

void D_USBHID_SysticUpdate(){
	D_USBHID_Send();
}

void D_USBHID_SetSendData(bool* touchData, bool* sideData){
	int count = 0;
	for(int i = 0 ;i < SENDDATA_SIZE; i++){
		keyboardHID.key[i] = 0;
	}
	for(int i=0; i<32; i++){
		if(touchData[i]){
			keyboardHID.key[count] = KeyboardOrder[i];
			count++;
		}
	}
	for(int i=0; i<6; i++){
		if(sideData[i]){
			keyboardHID.key[count] = KeyboardOrder[i+32];
			count++;
		}
	}
}

void D_USBHID_SetSendData_Touch(bool* touchData){
	int count = 0;
	for(int i = 0 ;i < SENDDATA_SIZE; i++){
		keyboardHID.key[i] = 0;
	}
	for(int i=0; i<32; i++){
		if(touchData[i]){
			keyboardHID.key[count] = KeyboardOrder[i];
			count++;
		}
		if(count >= SENDDATA_SIZE){
			break;
		}
	}
}

void D_USBHID_Send(){
	USBD_HID_SendReport(&hUsbDeviceFS, &keyboardHID, sizeof(struct keyboardHID_t));
}

