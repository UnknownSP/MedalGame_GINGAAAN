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

void SM1_Set(void);

#define USE_I2C false

static uint8_t sndData[4][8]; //各ステーションへのCAN送信データ
static uint8_t rcvData[4][8]; //各ステーションからのCAN受信データ
static uint8_t rcvDataJudge[8]; //各ステーションからのCAN受信データ

#define CAN_SEND_INTERVAL 100
#define CAN_RECEIVE_INTERVAL 100

#define CAN_MECH1_ADDRESS 0x009
#define CAN_ST2_ADDRESS 0x200
#define CAN_ST3_ADDRESS 0x300
#define CAN_ST4_ADDRESS 0x400
#define CAN_MAIN_ADDRESS 0x000
#define CAN_MECH1 0
#define CAN_ST2 1
#define CAN_ST3 2
#define CAN_ST4 3
#define CAN_MAIN 4

#define IO_SET_STEP() (D_GPIO_Set(GPIOA,GPIO_PIN_12))
#define IO_RESET_STEP() (D_GPIO_Reset(GPIOA,GPIO_PIN_12))
#define IO_SET_DIR() (D_GPIO_Set(GPIOA,GPIO_PIN_11))
#define IO_RESET_DIR() (D_GPIO_Reset(GPIOA,GPIO_PIN_11))
#define IO_SET_ENA() (D_GPIO_Set(GPIOB,GPIO_PIN_12))
#define IO_RESET_ENA() (D_GPIO_Reset(GPIOB,GPIO_PIN_12))

#define IO_READ_SM_R() (D_GPIO_Read(GPIOC,GPIO_PIN_0))
#define IO_READ_SM_L() (D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_SM_C() (D_GPIO_Read(GPIOC,GPIO_PIN_2))

//GINGAAAN
#define IO_SET_SHOOTER_R() (D_GPIO_Set(GPIOC,GPIO_PIN_0))
#define IO_RESET_SHOOTER_R() (D_GPIO_Reset(GPIOC,GPIO_PIN_0))
#define IO_SET_SHOOTER_L() (D_GPIO_Set(GPIOC,GPIO_PIN_3))
#define IO_RESET_SHOOTER_L() (D_GPIO_Reset(GPIOC,GPIO_PIN_3))

#define IO_READ_WAITBALL_R() (D_GPIO_Read(GPIOB,GPIO_PIN_4))
#define IO_READ_SHOOTCOUNT_R() (D_GPIO_Read(GPIOB,GPIO_PIN_14))
#define IO_READ_SOLENOID_R() (D_GPIO_Read(GPIOB,GPIO_PIN_10))
#define IO_READ_WAITBALL_L() (D_GPIO_Read(GPIOC,GPIO_PIN_4))
#define IO_READ_SHOOTCOUNT_L() (D_GPIO_Read(GPIOB,GPIO_PIN_5))
#define IO_READ_SOLENOID_L() (D_GPIO_Read(GPIOB,GPIO_PIN_13))

#define IO_SET_USERLED() (D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() (D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() (!D_GPIO_Read(GPIOC,GPIO_PIN_13))

#define LED_RIGHT_R 5
#define LED_RIGHT_G 6
#define LED_RIGHT_B 3
#define LED_LEFT_R 1
#define LED_LEFT_G 2
#define LED_LEFT_B 4

#endif /* INC_APP_H_ */
