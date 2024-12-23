/*
 * app.h
 *
 *  Created on: Mar 11, 2023
 *      Author: UnknownSP
 */

#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"

int appTask(void);
int appInit(void);

static uint8_t rcvData[8] = {0}; //CAN受信データ
static uint8_t sndData[8] = {0}; //CAN送信データ

static volatile uint8_t rcvData_UART[32] = {0}; //PCからの受信データ
static volatile uint8_t sndData_UART[16] = {0};

//static uint8_t controllerSendData[101] = {0};
//static uint8_t controllerRecvData[100] = {0};
static bool TouchSegment[32] = {false};
static bool SideSegment[6] = {false};
static uint8_t TouchThreshold[32] = {0};
static uint8_t SliderLEDData[16][3] = {{0}};
static uint8_t SliderBarLEDData[16][3] = {{0}}; //16はない

static bool SliderLEDDataR[5][3] = {{false}};
static bool SliderLEDDataL[5][3] = {{false}};

#define STATION_NUMBER 1

#define CAN_SEND_INTERVAL 10
#define UART_RECEIVE_INTERVAL 20

#define CAN_ST1_ADDRESS 0x100
#define CAN_ST2_ADDRESS 0x200
#define CAN_ST3_ADDRESS 0x300
#define CAN_ST4_ADDRESS 0x400
#define CAN_MAIN_ADDRESS 0x500
#define CAN_ST1 0
#define CAN_ST2 1
#define CAN_ST3 2
#define CAN_ST4 3
#define CAN_MAIN 4

#define IO_SET_USERLED() 		(D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() 		(D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() 	(!D_GPIO_Read(GPIOC,GPIO_PIN_13))

#define IO_SET_SHUTTER_SOL() 	(D_GPIO_Set(GPIOB,GPIO_PIN_6))
#define IO_RESET_SHUTTER_SOL() 	(D_GPIO_Reset(GPIOB,GPIO_PIN_6))

#define IO_READ_1ST_IN() 		(D_GPIO_Read(GPIOA,GPIO_PIN_15))
#define IO_READ_1ST_COLLECT() 	(D_GPIO_Read(GPIOB,GPIO_PIN_7))
#define IO_READ_1ST_SLOPE() 	(D_GPIO_Read(GPIOA,GPIO_PIN_0))
#define IO_READ_2ND_IN() 		(D_GPIO_Read(GPIOA,GPIO_PIN_1))
#define IO_READ_2ND_COLLECT() 	(D_GPIO_Read(GPIOA,GPIO_PIN_4))
#define IO_READ_2ND_SLOPE() 	(D_GPIO_Read(GPIOC,GPIO_PIN_2))
#define IO_READ_3RD_IN() 		(D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_3RD_COLLECT() 	(D_GPIO_Read(GPIOC,GPIO_PIN_3))
#define IO_READ_3RD_SHUTTER() 	(D_GPIO_Read(GPIOC,GPIO_PIN_0))

#define IO_READ_L_MECHA() 		(D_GPIO_Read(GPIOD,GPIO_PIN_2))
#define IO_READ_L_BALLWAIT() 	(D_GPIO_Read(GPIOC,GPIO_PIN_11))
#define IO_READ_L_START() 		(D_GPIO_Read(GPIOC,GPIO_PIN_10))
#define IO_READ_L_BALLSUPPLY() 	(D_GPIO_Read(GPIOC,GPIO_PIN_12))

#define IO_READ_SIG_1() 		(!D_GPIO_Read(GPIOB,GPIO_PIN_14))

#define IO_1ST_IN		0
#define IO_1ST_COLLECT	1
#define IO_1ST_SLOPE	2
#define IO_2ND_IN		3
#define IO_2ND_COLLECT	4
#define IO_2ND_SLOPE 	5
#define IO_3RD_IN		6
#define IO_3RD_COLLECT 	7
#define IO_3RD_SHUTTER 	8
#define IO_L_MECHA		9
#define IO_L_BALLWAIT 	10
#define IO_L_START		11
#define IO_L_BALLSUPPLY 12
#define IO_SIG_1		13

#define TIM_MAX_COUNT 5000


#endif /* INC_APP_H_ */
