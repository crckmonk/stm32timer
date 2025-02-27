#ifndef tm1637
#define tm1637

#include "main.h"

#define A_SEG				0x01
#define B_SEG				0x02
#define C_SEG				0x04
#define D_SEG				0x08
#define E_SEG				0x10
#define F_SEG				0x20
#define G_SEG				0x40
#define DP_SEG				0x80

//Settings
#define STM2DISPLAY			0x00
#define DISPLAY2STM			0x01

//Display Address
#define C0H					0xC0
#define C1H					0xC1
#define C2H					0xC2
#define C3H					0xC3
#define C4H					0xC4
#define C5H					0xC5

//Commands
#define DATA_SET			0x40
#define DATA_FIXED			0x44
#define DISPLAY_ON			0x88
#define DISPLAY_OFF			0x80
#define PACKET_SIZE			0x08

typedef enum
{
	TM1637_OK = 0U,
	TM1637_ERROR


}tm1637_StatusTypedef;

void tm1637_CLK_High();
void tm1637_CLK_Low();
void tm1637_DIO_High();
void tm1637_DIO_Low();
void tm1637_Start_Packet();
void tm1637_Stop_Packet();

void tm1637_Tx_Byte(uint8_t *TxData);

void tm1637_Tx_Command(uint8_t *Command);

void tm1637_Tx_Data(uint8_t *Data, uint8_t PacketSize);

void tm1637_Initialize(uint8_t Direction);

void tm1637_Display_Clear();

uint8_t tm1637_Display_Handle(uint8_t Brightness, uint8_t *DisplayBuffer);

tm1637_StatusTypedef tm1637_Set_Brighness(uint8_t BrighnessLevel);

//void tm1637_Parse_Display(uint8_t DisplayBuffer[][LED_DISPLAY], uint8_t *Text);

uint8_t char2segments(char c);

void tm1637_ACK_Check();

#endif