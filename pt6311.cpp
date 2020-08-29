#include "pt6311.h"
#include <SPI.h>

pt6311::pt6311(uint8_t STB_pin, uint8_t num_Of_Digit){
  STB = STB_pin;
  numOfDigit = num_Of_Digit;
  pinMode(STB, OUTPUT);
  digitalWrite(STB, HIGH);
  SPI.begin();
  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setDataMode(SPI_MODE3);
  SendByte(COMMAND_2 | NORMAL_OPERATION_MODE | INCREMENT_ADDRESS | WRITE_DATA_TO_DISPLAY_MODE);
  digitalWrite(STB, LOW);
  SPI.transfer(COMMAND_3);
  for (uint8_t i = 0; i < 13; i++) {
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);
  }
  digitalWrite(STB, HIGH);
  delayMicroseconds(spi_timeout);
  switch(numOfDigit){
    case 9:
      SendByte(COMMAND_1 | DIG9_SEG19);
      break;
    case 10:
      SendByte(COMMAND_1 | DIG10_SEG18);
      break;
    case 11:
      SendByte(COMMAND_1 | DIG11_SEG17);
      break;
    case 12:
      SendByte(COMMAND_1 | DIG12_SEG16);
      break;
    case 13:
      SendByte(COMMAND_1 | DIG13_SEG15);
      break;
    case 14:
      SendByte(COMMAND_1 | DIG14_SEG14);
      break;
    case 15:
      SendByte(COMMAND_1 | DIG15_SEG13);
      break;
    case 16:
      SendByte(COMMAND_1 | DIG16_SEG12);
      break;
    default:
      SendByte(COMMAND_1 | DIG8_SEG20);
      break;
  }
  SendByte(COMMAND_4 | DISPLAY_ON | PULSE_WIDTH14);
  displayState = DISPLAY_ON;
  displayBrighthess = PULSE_WIDTH14;
  for(uint8_t i = 0; i < 12; i++)displayBuffer[i] = ' ';
};

void pt6311::setBrightness(uint8_t br){ 
      displayBrighthess = br;
      SendByte(COMMAND_4 | displayState | displayBrighthess);
};

void pt6311::displayOn(){
  displayState = DISPLAY_ON;
  SendByte(COMMAND_4 | displayState | displayBrighthess);
}
    
void pt6311::displayOff(){
  displayState = DISPLAY_OFF;
  SendByte(COMMAND_4 | displayState | displayBrighthess);
}

uint8_t pt6311::getBrightness(){ 
  return displayBrighthess;
}

bool pt6311::getState(){ 
  return displayState>0?true:false;
}

void pt6311::SendByte(uint8_t data) {
  digitalWrite(STB, LOW);
  SPI.transfer(data);
  digitalWrite(STB, HIGH);
  delayMicroseconds(spi_timeout);
}

void pt6311::setLeds(uint8_t data){
  digitalWrite(STB, LOW);
  SPI.transfer(COMMAND_2 | NORMAL_OPERATION_MODE | INCREMENT_ADDRESS | WRITE_DATA_TO_LED_PORT);
  SPI.transfer(data);
  digitalWrite(STB, HIGH);
}

void pt6311::ledWrite(uint8_t number, uint8_t state){
	number--;
	uint8_t mask = 1 << number;
	
	ledState_ = (ledState_ & ~mask) | ((!state << number) & mask);
	setLeds(ledState_);
}


void pt6311::writeChar(uint8_t pos, unsigned char data){ 
	displayBuffer[pos] = data;
}

void pt6311::writeString(uint8_t pos, unsigned char data[]){
	for(uint8_t i = pos; i<strlen((const char*)data)+pos; i++){
		writeChar(i, data[i-pos]);
	}
}
void pt6311::writeString(uint8_t pos, const char data[]){
	for(uint8_t i = pos; i<strlen((const char*)data)+pos; i++){
		writeChar(i, data[i-pos]);
	}
}

void pt6311::setCursor(uint8_t pos){
	digitalWrite(STB, LOW);
	SPI.transfer(COMMAND_3 | (pos * 0x03));
	digitalWrite(STB, HIGH);
}

void pt6311::getBytes(uint8_t *first, uint8_t *second, uint8_t pos){
	uint8_t currentChar = displayBuffer[11-pos];
	uint16_t data = pgm_read_word_near(alphafonttable+currentChar);
	
//Shuffle the bit positions to be compatible with your pinout.
//bit permutation code generated using: http://programming.sirrida.de/calcperm.php
data = ((data & 0x00002000) << 1)
  | ((data & 0x00001002) << 3)
  | ((data & 0x00000020) << 6)
  | ((data & 0x00000008) << 9)
  | ((data & 0x00000001) << 10)
  | ((data & 0x00000004) << 11)
  | ((data & 0x00000800) >> 11)
  | ((data & 0x00008000) >> 7)
  | ((data & 0x00004480) >> 5)
  | ((data & 0x00000250) >> 3)
  | ((data & 0x00000100) >> 1);
 
 //Now reverse order of the bits
 uint16_t revData = 0;
  for(int i=0;i<16;i++)
     if(data & (1 << i)) revData |= 1 << (15-i);
 
 *first =  (uint8_t)((revData >> 8)&0xFF);
 *second = (uint8_t)(revData & 0xFF);
}

void pt6311::print(uint8_t pos, const char data[]){
	writeString(pos, data);
	setCursor(pos);
	writeLine();
}

void pt6311::writeLine(){
	uint8_t firstByte;
	uint8_t secondByte;
	digitalWrite(STB, LOW);
	SPI.transfer(0b01000000);
	for(int i = 0; i<12; i++){
		getBytes(&firstByte, &secondByte, i);
		SPI.transfer(firstByte);
		SPI.transfer(secondByte);
		SPI.transfer(0b00000000);	//write third unused byte to increment address
	}
	digitalWrite(STB, HIGH);
}

void pt6311::clearBuffer(void){
	for(uint8_t i = 0; i<12; i++)
		displayBuffer[i] = ' ';
}
