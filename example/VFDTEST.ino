#include <pt6311.h>

pt6311 VFD(10,12);

void setup() {
  delay(200);
}

void loop() {
  static uint8_t brightness = 0;
  VFD.setBrightness(brightness);
  if(brightness>7) brightness = 0; else brightness++;
  for(int i=0; i<5; i++){
    static bool b = false;
    VFD.ledWrite(i, b);
    b = !b;
  }
 VFD.print(0,"VFD-Test1234");
 delay(1000);
 VFD.clearBuffer();
 VFD.setCursor(0);
 VFD.writeLine();
 delay(500);
}
