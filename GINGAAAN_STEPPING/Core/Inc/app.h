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

void SM_1stLot_Set(void);
void SM_Lifter_Set(void);
void SM_Divide_Set(void);

#define USE_I2C false
#define USE_PWM false

static uint8_t sndData[4][8]; //各ステーションへのCAN送信データ
static uint8_t rcvData[4][8]; //各ステーションからのCAN受信データ
static uint8_t rcvDataJudge[8]; //各ステーションからのCAN受信データ

#define CAN_SEND_INTERVAL 1000
#define SM_SEND_INTERVAL 2
#define SM_STEPCOUNT 3800

#define SM_DIVIDE_R_STEP 430

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

#define IO_SET_1STLOT_STEP() (D_GPIO_Set(GPIOC,GPIO_PIN_5))
#define IO_RESET_1STLOT_STEP() (D_GPIO_Reset(GPIOC,GPIO_PIN_5))
#define IO_SET_1STLOT_DIR() (D_GPIO_Set(GPIOC,GPIO_PIN_11))
#define IO_RESET_1STLOT_DIR() (D_GPIO_Reset(GPIOC,GPIO_PIN_11))
#define IO_RESET_1STLOT_ENA() (D_GPIO_Set(GPIOC,GPIO_PIN_10))
#define IO_SET_1STLOT_ENA() (D_GPIO_Reset(GPIOC,GPIO_PIN_10))

#define IO_SET_LIFTER_STEP() (D_GPIO_Set(GPIOC,GPIO_PIN_6))
#define IO_RESET_LIFTER_STEP() (D_GPIO_Reset(GPIOC,GPIO_PIN_6))
#define IO_SET_LIFTER_DIR() (D_GPIO_Set(GPIOD,GPIO_PIN_2))
#define IO_RESET_LIFTER_DIR() (D_GPIO_Reset(GPIOD,GPIO_PIN_2))
#define IO_RESET_LIFTER_ENA() (D_GPIO_Set(GPIOC,GPIO_PIN_12))
#define IO_SET_LIFTER_ENA() (D_GPIO_Reset(GPIOC,GPIO_PIN_12))

#define IO_SET_DIVIDE_STEP() (D_GPIO_Set(GPIOC,GPIO_PIN_8))
#define IO_RESET_DIVIDE_STEP() (D_GPIO_Reset(GPIOC,GPIO_PIN_8))
#define IO_SET_DIVIDE_DIR() (D_GPIO_Set(GPIOA,GPIO_PIN_0))
#define IO_RESET_DIVIDE_DIR() (D_GPIO_Reset(GPIOA,GPIO_PIN_0))
#define IO_RESET_DIVIDE_ENA() (D_GPIO_Set(GPIOB,GPIO_PIN_7))
#define IO_SET_DIVIDE_ENA() (D_GPIO_Reset(GPIOB,GPIO_PIN_7))

#define IO_READ_1STLOT_R() (D_GPIO_Read(GPIOC,GPIO_PIN_4))
#define IO_READ_1STLOT_L() (D_GPIO_Read(GPIOB,GPIO_PIN_5))
#define IO_READ_1STLOT_C() (D_GPIO_Read(GPIOB,GPIO_PIN_13))

#define IO_READ_DIVIDE_BALL_R() (D_GPIO_Read(GPIOB,GPIO_PIN_15))
#define IO_READ_DIVIDE_BALL_L() (D_GPIO_Read(GPIOA,GPIO_PIN_8))
#define IO_READ_LIFTER_HOME() (D_GPIO_Read(GPIOB,GPIO_PIN_2))
#define IO_READ_LIFTER_BALL() (D_GPIO_Read(GPIOC,GPIO_PIN_7))
#define IO_READ_DIVIDE_HOME() (D_GPIO_Read(GPIOB,GPIO_PIN_6))

#define IO_SET_USERLED() (D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() (D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() (!D_GPIO_Read(GPIOC,GPIO_PIN_13))


#endif /* INC_APP_H_ */
