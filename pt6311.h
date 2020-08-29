#include <SPI.h>
#include <avr/pgmspace.h>

#include "fonts.h"

#pragma once
#define COMMAND_1 0b00000000
#define COMMAND_2 0b01000000
#define COMMAND_3 0b11000000
#define COMMAND_4 0b10000000
//parameters COMMAND_1
#define DIG8_SEG20 0b0000
#define DIG9_SEG19 0b1000
#define DIG10_SEG18 0b1001
#define DIG11_SEG17 0b1010
#define DIG12_SEG16 0b1011
#define DIG13_SEG15 0b1100
#define DIG14_SEG14 0b1101
#define DIG15_SEG13 0b1110
#define DIG16_SEG12 0b1111
//parameters COMMAND_2
#define NORMAL_OPERATION_MODE 0b0000
#define TEST_MODE 0b1000
#define INCREMENT_ADDRESS 0b000
#define FIXED_ADDRESS 0b100
#define WRITE_DATA_TO_DISPLAY_MODE 0b00
#define WRITE_DATA_TO_LED_PORT 0b01
#define READ_KEY_DATA 0b10
#define READ_SW_DATA 0b11
//parameters COMMAND_4
#define DISPLAY_ON 0b1000
#define DISPLAY_OFF 0b0000
#define PULSE_WIDTH1 0b000
#define PULSE_WIDTH2 0b001
#define PULSE_WIDTH4 0b010
#define PULSE_WIDTH10 0b011
#define PULSE_WIDTH11 0b100
#define PULSE_WIDTH12 0b101
#define PULSE_WIDTH13 0b110
#define PULSE_WIDTH14 0b111

class pt6311{
  private:
    const int spi_timeout = 25;
    uint8_t STB;
    uint8_t numOfDigit;
    uint8_t displayState;
    uint8_t displayBrighthess;
	uint8_t ledState_;
	unsigned char displayBuffer[12];
	
	void setLeds(uint8_t data);
	void SendByte(uint8_t data);
    
  public:
  
    pt6311(uint8_t STB_pin, uint8_t num_Of_Digit);
    void setBrightness(uint8_t br); // from 0 to 7
    void displayOn(void);
    void displayOff(void);
    uint8_t getBrightness(); //return brightness
    bool getState(); // return on or off
	
	void ledWrite(uint8_t number, uint8_t state);
	void writeChar(uint8_t pos, unsigned char data);
	void writeLine(void);
	void getBytes(uint8_t *first, uint8_t *second, uint8_t pos);
	void writeString(uint8_t pos, unsigned char data[]);
	void writeString(uint8_t pos, const char data[]);
	void setCursor(uint8_t pos);
	void print(uint8_t pos, const char data[]);
	void clearBuffer(void);
};
/*
	Adafruit							  1  2 3 4 5 6 7 8 9  10 11 12 13 14 15 16
	bit position -> segment map: 0 DP N M L K J H G2 G1 F  E  D  C  B  A
	(source: https://learn.adafruit.com/14-segment-alpha-numeric-led-featherwing/usage )
	
	C1-1 = L
	C1-2 = E
	C1-3 = G2
	C1-4 = G1
	C1-5 = B
	C1-6 = K
	C1-7 = J
	C1-8 = H
	
	C2-1 = NC
	C2-2 = NC
	C2-3 = A
	C2-4 = F
	C2-5 = D
	C2-6 = C
	C2-7 = N
	c2-8 = M
	
	0-8
	1-9
	2-14
	3-15
	4-0
	5-5
	6-6
	7-7
	8-2
	9-3
	10-11
	11-1
	12-12
	13-13
	14-4
	15-10

//bit permutation code generated using: http://programming.sirrida.de/calcperm.php	
8 9 14 15 0 5 6 7 2 3 11 1 12 13 4 10
*/
