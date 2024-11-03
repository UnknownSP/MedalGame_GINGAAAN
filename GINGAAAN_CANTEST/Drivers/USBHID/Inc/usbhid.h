/*
 * usbhid.h
 *
 *  Created on: Mar 6, 2024
 *      Author: UnknownSP
 */

#ifndef USBHID_INC_USBHID_H_
#define USBHID_INC_USBHID_H_

#include "main.h"

#define SENDDATA_SIZE 32
#define KEYBOARD_A 0x04
#define KEYBOARD_B 0x05
#define KEYBOARD_C 0x06
#define KEYBOARD_D 0x07
#define KEYBOARD_E 0x08
#define KEYBOARD_F 0x09
#define KEYBOARD_G 0x0A
#define KEYBOARD_H 0x0B
#define KEYBOARD_I 0x0C
#define KEYBOARD_J 0x0D
#define KEYBOARD_K 0x0E
#define KEYBOARD_L 0x0F
#define KEYBOARD_M 0x10
#define KEYBOARD_N 0x11
#define KEYBOARD_O 0x12
#define KEYBOARD_P 0x13
#define KEYBOARD_Q 0x14
#define KEYBOARD_R 0x15
#define KEYBOARD_S 0x16
#define KEYBOARD_T 0x17
#define KEYBOARD_U 0x18
#define KEYBOARD_V 0x19
#define KEYBOARD_W 0x1A
#define KEYBOARD_X 0x1B
#define KEYBOARD_Y 0x1C
#define KEYBOARD_Z 0x1D
#define KEYBOARD_1 0x1E
#define KEYBOARD_2 0x1F
#define KEYBOARD_3 0x20
#define KEYBOARD_4 0x21
#define KEYBOARD_5 0x22
#define KEYBOARD_6 0x23
#define KEYBOARD_7 0x24
#define KEYBOARD_8 0x25
#define KEYBOARD_9 0x26
#define KEYBOARD_0 0x27
#define KEYBOARD_SEMICOLON 0x33
#define KEYBOARD_COLON 0x34
#define KEYBOARD_COMMA 0x36
#define KEYBOARD_PERIOD 0x37
#define KEYBOARD_SLASH 0x38
#define KEYBOARD_ATMARK 0x2F
#define KEYBOARD_LEFTSQUAREBRACKETS 0x30

struct keyboardHID_t {
	uint8_t modifiers;
	uint8_t reserved;
	uint8_t key[SENDDATA_SIZE];
};

void D_USBHID_Init();
void D_USBHID_SysticUpdate();
void D_USBHID_SetSendData(bool* touchData, bool* sideData);
void D_USBHID_SetSendData_Touch(bool* touchData);
void D_USBHID_Send();

#endif /* USBHID_INC_USBHID_H_ */
