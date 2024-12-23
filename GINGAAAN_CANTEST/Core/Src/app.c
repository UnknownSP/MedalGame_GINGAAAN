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

//static bool CheckSum(uint8_t* data, int length);
//static void CheckTouch(uint8_t* recvData, bool* setData);
//static void SetColorData(uint8_t* setArray, uint8_t setColorTouch[][3], uint8_t setColorBar[][3]);
static bool IO_Read(uint8_t sensor);

//初期化
int appInit(void){
	/*
	for(int i=0; i<32; i++){
		TouchThreshold[i] = 40;
	}
	for(int i=0; i<16; i++){
		SliderBarLEDData[i][0] = 128;
		SliderBarLEDData[i][1] = 00;
		SliderBarLEDData[i][2] = 128;
	}
	D_Slider_SetThreshold(TouchThreshold);
	*/
	return 0;
}

int appTask(void){

	static bool _firstSend = false;

	static bool _userButton = false;
	static uint32_t successCount = 0;

	static uint32_t recent_System_counter = 0;
	static uint32_t sndTime = 0;
	static uint32_t rcvTime = 0;

	static int retSnd = 0;
	static int retRcv = 0;
	static int rcvJudge[16] = {};

	static int testCount = 252;
	static int testCount1 = 0;
	static int testCount2 = 0;
	static int testCount3 = 0;
	static int testCount4 = 0;

	sndTime += G_System_counter - recent_System_counter;
	rcvTime += G_System_counter - recent_System_counter;
	recent_System_counter = G_System_counter;

	//CAN送信タイミングなら送信
	if(sndTime >= CAN_SEND_INTERVAL){

		sndTime = 0;
	}

	static uint8_t sum = 0;
	if(rcvTime >= UART_RECEIVE_INTERVAL){
		/*
		static int sendLength = 10;
		sndData_UART[0] = 0xe0;//testCount1;
		sndData_UART[1] = testCount1;//testCount1;//testCount2;
		sndData_UART[2] = testCount;
		sndData_UART[3] = 5;
		sndData_UART[4] = testCount1;
		sndData_UART[5] = 200;
		sndData_UART[6] = 200;
		sndData_UART[7] = 200;
		*/
		/*
		static int sendLength = 8;
		sndData_UART[0] = 0xe0;//testCount1;
		sndData_UART[1] = 0x01;//testCount1;//testCount2;
		sndData_UART[2] = testCount;
		sndData_UART[3] = 3;
		sndData_UART[4] = 225;
		*/
		static int sendLength = 8;
		sndData_UART[0] = 0xe0;//testCount1;
		sndData_UART[1] = 0x01;//testCount1;//testCount2;
		sndData_UART[2] = 255;
		sndData_UART[3] = 3;
		sndData_UART[4] = testCount;
		sndData_UART[5] = 1;
		sndData_UART[6] = 8;
		sndData_UART[7] = 250;
		sndData_UART[8] = 250;
		/*
		static int sendLength = 12;
		sndData_UART[0] = 0xe0;//testCount1;
		sndData_UART[1] = 0x01;//testCount1;//testCount2;
		sndData_UART[2] = testCount;
		sndData_UART[3] = 5;
		sndData_UART[4] = 50;
		testCount3 = 0;
		int R = testCount & 1;
		int G = (testCount>>1) & 1;
		int B = (testCount>>2) & 1;
		sndData_UART[5] = 0;//R * 36 + G * 146 + B * 72;//testCount;//150;//testCount;
		sndData_UART[6] = 208;//R * 73 + G * 36 + B * 146;//testCount;//testCount;//testCount2;
		sndData_UART[7] = 223;//R * 36 + G * 146 + B * 72;//testCount;//testCount2;
		sndData_UART[8] = 0;//R * 73 + G * 36 + B * 146;//testCount;//243;//testCount2;
		sndData_UART[9] = 208;//testCount2;//testCount1;
		sndData_UART[10] = 223;//testCount2;
		sndData_UART[11] = 0;//testCount1;
		sndData_UART[12] = 0;//testCount2;
		sndData_UART[13] = 0;//testCount1;
		sndData_UART[14] = 0;//testCount2;
		sndData_UART[15] = 0;//testCount1;
		*/

		uint8_t dataSum = 0;
		//dataSum += sndData_UART[1];
		for(int j=1; j<sendLength-1; j++){
			dataSum += sndData_UART[j];
		}
		//dataSum += 2;

		//int checkSum = 256 - (int)dataSum;
		//sndData_UART[2/*sendLength-1*/] = (uint8_t)checkSum - 1;
		sndData_UART[sendLength-1] = (uint8_t)dataSum;

		//retSnd = D_Slider_Send(sndData_UART,sendLength);
		//retRcv = D_Slider_Receive(rcvData_UART,16);
		rcvTime = 0;

		/*
		if(rcvData_UART[0] != FIRSTBYTE || rcvData_UART[1] != SENSORGET_SEQNUM || rcvData_UART[2] != ADDRESS) retRcv = 2;

		sum = 0;
		for(int i=1; i<9; i++){
			sum += rcvData_UART[i];
		}
		if(sum != rcvData_UART[SENSORGET_RECEIVE_LENGTH-1]) retRcv = 3;
		for(int i=0; i<5; i++){
			if(((rcvData_UART[7]>>i) & 1) == 1) retRcv++;
		}
		*/
		/*
		for(int i=0; i<5; i++){
			if(((data[7]>>i) & 1) == 1) SensorCountL[i]++;
		}
		for(int i=0; i<5; i++){
			if(((data[8]>>i) & 1) == 1) SensorCountR[i]++;
		}
		*/

		testCount1++;
		if(testCount1 >= 256){
			testCount1 = 0;
			testCount2++;
		}
		if(testCount2 >= 256){
			testCount2 = 0;
			testCount3++;
		}
		if(testCount3 >= 256){
			testCount3 = 0;
			testCount4++;
		}
	}

	bool R = (testCount & 1) == 1;
	bool G = ((testCount>>1) & 1) == 1;
	bool B = ((testCount>>2) & 1) == 1;
	for(int i=0; i<5; i++){
		if (D_Slider_GetSensorData(0,i) != 0){
			SliderLEDDataR[i][0] = true;
			SliderLEDDataR[i][1] = true;
			SliderLEDDataR[i][2] = true;
		}
		if (D_Slider_GetSensorData(1,i) != 0){
			SliderLEDDataL[i][0] = true;
			SliderLEDDataL[i][1] = true;
			SliderLEDDataL[i][2] = true;
		}
	}
	//SliderLEDDataL[3][0] = true;
	//SliderLEDDataL[4][0] = true;
	//SliderLEDDataL[4][1] = true;
	//SliderLEDDataL[4][2] = true;
	//SliderLEDDataR[3][0] = false;
	//SliderLEDDataR[3][1] = false;
	//SliderLEDDataR[3][2] = false;
	/*
	SliderLEDDataR[0][0] = testCount1 % 2 == 0;
	SliderLEDDataR[0][1] = testCount1 % 2 == 0;
	SliderLEDDataR[0][2] = testCount1 % 2 == 0;
	SliderLEDDataR[3][0] = testCount1 % 2 == 0;
	SliderLEDDataR[3][2] = testCount1 % 2 == 0;
	SliderLEDDataR[2][0] = testCount1 % 2 == 0;
	SliderLEDDataL[0][0] = testCount1 % 2 == 0;
	SliderLEDDataL[0][1] = testCount1 % 2 == 0;
	SliderLEDDataL[0][2] = testCount1 % 2 == 0;
	SliderLEDDataL[3][0] = testCount1 % 2 == 0;
	SliderLEDDataL[3][2] = testCount1 % 2 == 0;
	SliderLEDDataL[2][0] = testCount1 % 2 == 0;
	*/
	//D_Slider_SetColorData(SliderLEDDataR, SliderLEDDataL);
	static bool _first = false;
	static bool _firstR = false;
	static bool _firstL = false;

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
	/*
	if(D_Slider_GetSensorData(0,1) != 0 || D_Slider_GetSensorData(0,3) != 0){
		if(!_firstR){
			r = SL_POCKET_IN;
			pocketR = 0;
			_firstR = true;
			D_Slider_SetLightingR(r,pocketR);
		}
		//r = SL_DISABLE;
	}
	if(D_Slider_GetSensorData(1,1) != 0 || D_Slider_GetSensorData(1,3) != 0){
		if(!_firstL){
			l = SL_POCKET_IN;
			pocketL = 0;
			_firstL = true;
			D_Slider_SetLightingL(l,pocketL);
		}
	}
	*/
	/*
	if(!_firstSend){
		_firstSend = true;
		controllerSendData[0] = 0xff;
		controllerSendData[1] = 0x03;
		controllerSendData[2] = 0x00;
		controllerSendData[3] = 0xfe;
		HAL_UART_Transmit(&huart1, (uint8_t*)controllerSendData, 4, 10);
	}
	*/

	//マイコンについてるボタンかスタートボタンが押されたらボール発射
	if(IO_READ_USERBUTTON() && _userButton){
		_userButton = false;
		testCount--;
		if(testCount < 0) testCount = 255;
		//IO_SET_USERLED();
		//IO_SET_SHUTTER_SOL();
		//LED_SetBrightness(PWM_LED_1ST,1000);
		//LED_SetBrightness(PWM_LED_2ND,1000);
		//LED_SetBrightness(PWM_LED_3RD,1000);
		/*
		controllerSendData[0] = 0xff;
		controllerSendData[1] = 0x03;
		controllerSendData[2] = 0x00;
		controllerSendData[3] = 0xfe;
		HAL_UART_Transmit(&huart1, (uint8_t*)controllerSendData, 4, 10);

		controllerSendData[0] = 0xff;
		controllerSendData[1] = 0x02;
		controllerSendData[2] = 0x61;
		controllerSendData[3] = 0x3f;
		for(int i=0; i<31; i++){
			controllerSendData[i*3+4] = 0xee;
			controllerSendData[i*3+5] = 0x00;
			controllerSendData[i*3+6] = 0x00;
		}
		controllerSendData[97] = 0x00;
		controllerSendData[98] = 0x00;
		controllerSendData[99] = 0x00;
		uint8_t dataSum = 0;
		for(int i=0; i<100; i++){
			dataSum += controllerSendData[i];
		}
		int checkSum = 256 - (int)dataSum;
		controllerSendData[100] = (uint8_t)checkSum;
		HAL_UART_Transmit_DMA(&huart1, (uint8_t*)controllerSendData, 101);
		*/
	}
	if(!IO_READ_USERBUTTON()){
		_userButton = true;
	}

	//D_Slider_GetTouch((bool*)TouchSegment);
	/*
	for(int i=0; i<16; i++){
		SliderLEDData[i][0] = 0;
		SliderLEDData[i][1] = 0;
		SliderLEDData[i][2] = 0;
		if(TouchSegment[i*2] && TouchSegment[i*2 + 1]){
			SliderLEDData[i][0] = 0;
			SliderLEDData[i][1] = 254;
			SliderLEDData[i][2] = 0;
		}else if(TouchSegment[i*2]){
			SliderLEDData[i][0] = 254;
			SliderLEDData[i][1] = 0;
			SliderLEDData[i][2] = 0;
		}else if(TouchSegment[i*2 + 1]){
			SliderLEDData[i][0] = 0;
			SliderLEDData[i][1] = 0;
			SliderLEDData[i][2] = 254;
		}
	}
	*/
	//D_Slider_SetColorData(SliderLEDData, SliderBarLEDData);

	//D_USBHID_SetSendData((bool*)TouchSegment, (bool*)SideSegment);
	/*
	HAL_UART_Receive(&huart1, (uint8_t*)controllerRecvData, 36, 10);
	if(CheckSum((uint8_t*)controllerRecvData, 36) && controllerRecvData[0] != 0){
		successCount++;
		CheckTouch((uint8_t*)controllerRecvData, (bool*)TouchSegment);
		for(int i=0; i<16; i++){
			SliderLEDData[i][0] = 0;
			SliderLEDData[i][1] = 0;
			SliderLEDData[i][2] = 0;
			if(TouchSegment[i*2] && TouchSegment[i*2 + 1]){
				SliderLEDData[i][0] = 0;
				SliderLEDData[i][1] = 254;
				SliderLEDData[i][2] = 0;
			}else if(TouchSegment[i*2]){
				SliderLEDData[i][0] = 254;
				SliderLEDData[i][1] = 0;
				SliderLEDData[i][2] = 0;
			}else if(TouchSegment[i*2 + 1]){
				SliderLEDData[i][0] = 0;
				SliderLEDData[i][1] = 0;
				SliderLEDData[i][2] = 254;
			}
		}
		SetColorData((uint8_t*)controllerSendData, SliderLEDData, SliderBarLEDData);
		HAL_UART_Transmit_DMA(&huart1, (uint8_t*)controllerSendData, 101);
		D_USBHID_Send((bool*)TouchSegment, (bool*)SideSegment);
	}
	for(int i=0; i<100; i++){
		controllerRecvData[i] = 0;
	}
	*/
	/*
	bool _recv = false;
	for(int i=0; i<10; i++){
		if(controllerRecvData[i] != 0){
			_recv = true;
		}
		controllerRecvData[i] = 0;
	}
	if(_recv){
		IO_SET_USERLED();
	}else{
		IO_RESET_USERLED();
	}
	*/
	//testMode---------------------------------------------------

	for(int i=0; i<16; i++){
		if(rcvData_UART[i] != 0) rcvJudge[i] = rcvData_UART[i];
	}

	//デバッグ用
	int16_t debug_bits = 0;
	debug_bits &= 0;
	/*
	debug_bits |= ((int)IO_READ_1ST_IN() << 0);
	debug_bits |= ((int)IO_READ_1ST_COLLECT() << 1);
	debug_bits |= ((int)IO_READ_1ST_SLOPE() << 2);
	debug_bits |= ((int)IO_READ_2ND_IN() << 3);
	debug_bits |= ((int)IO_READ_2ND_COLLECT() << 4);
	debug_bits |= ((int)IO_READ_2ND_SLOPE() << 5);
	debug_bits |= ((int)IO_READ_3RD_IN() << 6);
	debug_bits |= ((int)IO_READ_3RD_COLLECT() << 7);
	debug_bits |= ((int)IO_READ_3RD_SHUTTER() << 8);
	debug_bits |= ((int)IO_READ_L_MECHA() << 9);
	debug_bits |= ((int)IO_READ_L_BALLWAIT() << 10);
	debug_bits |= ((int)IO_READ_L_START() << 11);
	debug_bits |= ((int)IO_READ_L_BALLSUPPLY() << 12);
	*/
	D_Mess_printf("\033[1;1H");

	//D_Mess_printf("%016b\n", debug_bits);
	//D_Mess_printf("%10d\n", G_System_counter);
	//D_Mess_printf("R :[%3d],[%3d],[%3d],[%3d],[%3d]\n", D_Slider_GetSensorData(0,0), D_Slider_GetSensorData(0,1), D_Slider_GetSensorData(0,2), D_Slider_GetSensorData(0,3), D_Slider_GetSensorData(0,4));
	D_Mess_printf("L :[%3d],[%3d],[%3d],[%3d],[%3d]\n", D_Slider_GetSensorData(1,0), D_Slider_GetSensorData(1,1), D_Slider_GetSensorData(1,2), D_Slider_GetSensorData(1,3), D_Slider_GetSensorData(1,4));

	//D_Mess_printf("retSnd : %2d\n", retSnd);
	//D_Mess_printf("retRcv : %02d  \n", retRcv);
	//D_Mess_printf("sum : %03d  \n", sum);
	//D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", sndData_UART[0],sndData_UART[1],sndData_UART[2],sndData_UART[3],sndData_UART[4],sndData_UART[5],sndData_UART[6],sndData_UART[7],sndData_UART[8],sndData_UART[9],sndData_UART[10],sndData_UART[11],sndData_UART[12],sndData_UART[13],sndData_UART[14],sndData_UART[15]);
	//D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", rcvData_UART[0],rcvData_UART[1],rcvData_UART[2],rcvData_UART[3],rcvData_UART[4],rcvData_UART[5],rcvData_UART[6],rcvData_UART[7], rcvData_UART[8],rcvData_UART[9],rcvData_UART[10],rcvData_UART[11],rcvData_UART[12],rcvData_UART[13],rcvData_UART[14],rcvData_UART[15]);
	//D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", rcvData_UART[16],rcvData_UART[17],rcvData_UART[18],rcvData_UART[19],rcvData_UART[20],rcvData_UART[21],rcvData_UART[22],rcvData_UART[23], rcvData_UART[24],rcvData_UART[25],rcvData_UART[26],rcvData_UART[27],rcvData_UART[28],rcvData_UART[29],rcvData_UART[30],rcvData_UART[31]);
	//D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n", rcvJudge[0],rcvJudge[1],rcvJudge[2],rcvJudge[3],rcvJudge[4],rcvJudge[5],rcvJudge[6],rcvJudge[7], rcvJudge[8],rcvJudge[9],rcvJudge[10],rcvJudge[11],rcvJudge[12],rcvJudge[13],rcvJudge[14],rcvJudge[15]);
	//D_Mess_printf("%3d\n", successCount);
	/*
	for(int i=0; i<16; i++){
		D_Mess_printf("%1d ", (int)TouchSegment[i*2]);
	}
	D_Mess_printf("\n");
	for(int i=0; i<16; i++){
		D_Mess_printf("%1d ", (int)TouchSegment[i*2 + 1]);
	}
	D_Mess_printf("\n");
	*/
	//D_Mess_printf("%02d\n", errorHandle);
	//D_Mess_printf("%3d,%3d,%3d,%3d\n", sndData[0],sndData[1],sndData[2],sndData[3]);
	//D_Mess_printf("%3d,%3d,%3d,%3d,%3d,%3d\n", rcvData[0],rcvData[1],rcvData[2],rcvData[3],rcvData[4],rcvData[5]);
	//D_Mess_printf("%02d\n", inPocket_1st);
	//D_Mess_printf("%02d\n", inPocket_2nd);
	//D_Mess_printf("%02d\n", inPocket_3rd);
	//D_Mess_printf("%08b\n", ownHold_Pocket);
	//D_Mess_printf("%08b\n", allHold_Pocket);
	//D_Mess_printf("processState: ,%2d\n", processState);
	//D_Mess_printf("PC: ,%3d,%3d,%3d,%3d,%3d,%3d,%3d,%3d\n",rcvData_UART[0],rcvData_UART[1],rcvData_UART[2],rcvData_UART[3],rcvData_UART[4],rcvData_UART[5],rcvData_UART[6],rcvData_UART[7]);

	return 0;
}

/*
static bool CheckSum(uint8_t* data, int length){
	uint8_t sum = 0;
	for(int i=0; i<length; i++){
		sum += data[i];
	}
	if(sum == 0){
		return true;
	}else{
		return false;
	}
}

static void CheckTouch(uint8_t* recvData, bool* setData){
	for(int i=0; i<32; i++){
		if(recvData[i+3] >= TouchThreshold[i]){
			setData[i] = true;
		}else{
			setData[i] = false;
		}
	}
}

static void SetColorData(uint8_t* setArray, uint8_t setColorTouch[][3], uint8_t setColorBar[][3]){
	setArray[0] = 0xff;
	setArray[1] = 0x02;
	setArray[2] = 0x61;
	setArray[3] = 0x3f;
	for(int i=0; i<16; i++){
		setArray[i*6 + 4] = (setColorTouch[i][2] != 255) ? setColorTouch[i][2] : 254;
		setArray[i*6 + 5] = (setColorTouch[i][0] != 255) ? setColorTouch[i][0] : 254;
		setArray[i*6 + 6] = (setColorTouch[i][1] != 255) ? setColorTouch[i][1] : 254;
		setArray[i*6 + 7] = (setColorBar[i][2] != 255) ? setColorBar[i][2] : 254;
		setArray[i*6 + 8] = (setColorBar[i][0] != 255) ? setColorBar[i][0] : 254;
		setArray[i*6 + 9] = (setColorBar[i][1] != 255) ? setColorBar[i][1] : 254;
	}
	setArray[97] = 0x00;
	setArray[98] = 0x00;
	setArray[99] = 0x00;
	uint8_t dataSum = 0;
	for(int i=0; i<100; i++){
		dataSum += setArray[i];
	}
	int checkSum = 256 - (int)dataSum;
	setArray[100] = (uint8_t)checkSum;
}
*/

static bool IO_Read(uint8_t sensor){
	static bool _io_State[14] = {false};
	static bool _io_returnState[14] = {false};
	static bool _io_recentState[14] = {false};
	static uint8_t io_Count[14] = {0};

	_io_State[IO_1ST_IN]		= IO_READ_1ST_IN();
	_io_State[IO_1ST_COLLECT]	= IO_READ_1ST_COLLECT();
	_io_State[IO_1ST_SLOPE]		= IO_READ_1ST_SLOPE();
	_io_State[IO_2ND_IN]		= IO_READ_2ND_IN();
	_io_State[IO_2ND_COLLECT]	= IO_READ_2ND_COLLECT();
	_io_State[IO_2ND_SLOPE]		= IO_READ_2ND_SLOPE();
	_io_State[IO_3RD_IN]		= IO_READ_3RD_IN();
	_io_State[IO_3RD_COLLECT]	= IO_READ_3RD_COLLECT();
	_io_State[IO_3RD_SHUTTER]	= IO_READ_3RD_SHUTTER();
	_io_State[IO_L_MECHA]		= IO_READ_L_MECHA();
	_io_State[IO_L_BALLWAIT]	= IO_READ_L_BALLWAIT(); 	
	_io_State[IO_L_START]		= IO_READ_L_START();
	_io_State[IO_L_BALLSUPPLY]	= IO_READ_L_BALLSUPPLY(); 
	_io_State[IO_SIG_1]			= IO_READ_SIG_1();

	for(int i=0; i<14; i++){
		if(_io_State[i] != _io_recentState[i]){
			io_Count[i]++;
		}else{
			io_Count[i] = 0;
		}
		if(io_Count[i] >= 5){
			_io_recentState[i] = _io_State[i];
			io_Count[i] = 0;
			_io_returnState[i] = _io_State[i];
		}
	}

	return _io_returnState[sensor];
}

