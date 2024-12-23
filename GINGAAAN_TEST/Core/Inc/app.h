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

#define CAN_SEND_INTERVAL 1000
#define SM_SEND_INTERVAL 2
#define SM_STEPCOUNT 3800

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

#define BLDC_MAX_SPEED 5000

#define JPC_MAX_SPEED 900
#define JPC_MIN_SPEED 50
#define JPC_SLOW_SPEED 8
#define JPC_DECELERATOIN_TIME 20000

#define JP_POCKET_JP 1
#define JP_POCKET_6_R 2     //JPポケットの右
#define JP_POCKET_12_R 3    //JPポケットの右右
#define JP_POCKET_Q 4       // ?ポケット
#define JP_POCKET_12_L 5    //JPポケットの左左
#define JP_POCKET_6_L 6     //JPポケットの左

#define IO_SET_STEP() (D_GPIO_Set(GPIOA,GPIO_PIN_12))
#define IO_RESET_STEP() (D_GPIO_Reset(GPIOA,GPIO_PIN_12))
#define IO_SET_DIR() (D_GPIO_Set(GPIOA,GPIO_PIN_11))
#define IO_RESET_DIR() (D_GPIO_Reset(GPIOA,GPIO_PIN_11))
#define IO_SET_ENA() (D_GPIO_Set(GPIOB,GPIO_PIN_12))
#define IO_RESET_ENA() (D_GPIO_Reset(GPIOB,GPIO_PIN_12))

#define IO_READ_SM_R() (D_GPIO_Read(GPIOC,GPIO_PIN_0))
#define IO_READ_SM_L() (D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_SM_C() (D_GPIO_Read(GPIOC,GPIO_PIN_2))

//1st and 2nd BLDC controller
#define IO_RESET_BLDC1_ENA() (D_GPIO_Set(GPIOC,GPIO_PIN_6))
#define IO_SET_BLDC1_ENA() (D_GPIO_Reset(GPIOC,GPIO_PIN_6))
#define IO_SET_BLDC1_DIR() (D_GPIO_Set(GPIOC,GPIO_PIN_5))
#define IO_RESET_BLDC1_DIR() (D_GPIO_Reset(GPIOC,GPIO_PIN_5))

//3rd BLDC controller
#define IO_RESET_BLDC2_ENA() (D_GPIO_Set(GPIOA,GPIO_PIN_12))
#define IO_SET_BLDC2_ENA() (D_GPIO_Reset(GPIOA,GPIO_PIN_12))
#define IO_SET_BLDC2_DIR() (D_GPIO_Set(GPIOA,GPIO_PIN_11))
#define IO_RESET_BLDC2_DIR() (D_GPIO_Reset(GPIOA,GPIO_PIN_11))

//JPC BLDC controller
#define IO_RESET_BLDC3_ENA() (D_GPIO_Set(GPIOB,GPIO_PIN_12))
#define IO_SET_BLDC3_ENA() (D_GPIO_Reset(GPIOB,GPIO_PIN_12))
#define IO_SET_BLDC3_DIR() (D_GPIO_Set(GPIOB,GPIO_PIN_2))
#define IO_RESET_BLDC3_DIR() (D_GPIO_Reset(GPIOB,GPIO_PIN_2))

#define IO_SET_JPTOWER_ENA1() (D_GPIO_Set(GPIOB,GPIO_PIN_15))
#define IO_RESET_JPTOWER_ENA1() (D_GPIO_Reset(GPIOB,GPIO_PIN_15))
#define IO_SET_JPTOWER_ENA2() (D_GPIO_Set(GPIOB,GPIO_PIN_14))
#define IO_RESET_JPTOWER_ENA2() (D_GPIO_Reset(GPIOB,GPIO_PIN_14))

#define IO_SET_JP_LED() (D_GPIO_Set(GPIOC,GPIO_PIN_8))
#define IO_RESET_JP_LED() (D_GPIO_Reset(GPIOC,GPIO_PIN_8))

#define IO_READ_JP_HOME() (D_GPIO_Read(GPIOA,GPIO_PIN_1))
#define IO_READ_JP_ENC() (D_GPIO_Read(GPIOA,GPIO_PIN_4))
#define IO_READ_JP_FRONT() (D_GPIO_Read(GPIOC,GPIO_PIN_1))
#define IO_READ_JP_REAR() (D_GPIO_Read(GPIOC,GPIO_PIN_0))
#define IO_READ_JP_UPPER() (D_GPIO_Read(GPIOC,GPIO_PIN_3))
#define IO_READ_JP_LOWER() (D_GPIO_Read(GPIOC,GPIO_PIN_2))

#define IO_READ_1ST_HOME() (D_GPIO_Read(GPIOB,GPIO_PIN_7))
#define IO_READ_1ST_ENC() (D_GPIO_Read(GPIOA,GPIO_PIN_15))
#define IO_READ_2ND_HOME() (D_GPIO_Read(GPIOC,GPIO_PIN_12))
#define IO_READ_2ND_ENC() (D_GPIO_Read(GPIOC,GPIO_PIN_10))
#define IO_READ_3RD_HOME() (D_GPIO_Read(GPIOC,GPIO_PIN_11))
#define IO_READ_3RD_ENC() (D_GPIO_Read(GPIOD,GPIO_PIN_2))

#define IO_SET_USERLED() (D_GPIO_Set(GPIOA,GPIO_PIN_5))
#define IO_RESET_USERLED() (D_GPIO_Reset(GPIOA,GPIO_PIN_5))
#define IO_READ_USERBUTTON() (!D_GPIO_Read(GPIOC,GPIO_PIN_13))

#define IO_SET_ROOMLIGHT() (D_GPIO_Set(GPIOA,GPIO_PIN_0))
#define IO_RESET_ROOMLIGHT() (D_GPIO_Reset(GPIOA,GPIO_PIN_0))

#define BLDC1 4
#define BLDC2 2
#define BLDC3 1


#endif /* INC_APP_H_ */
