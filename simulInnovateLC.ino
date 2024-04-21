#include <Wire.h>
#include "ADS1X15.h"
//#define WORDS_BIGENDIAN
ADS1115 ADS(0x48);

// Ver 21.04.2024

char ch[6];
uint16_t cc;

void setup() {
  Serial.begin(19200);
  ch[0] = 0xB2;//178
  ch[1] = 0x82;//130
  ch[2] = 0x43;//67
  ch[3] = 0x13;//19
  ch[4] = 0x02;//2
  ch[5] = 0x2C;//2
  cc = 500;
  Wire.begin();
  ADS.begin();
 
  ADS.setGain(0);
}
void loop() {
//Serial.println(Print_Hexa(compteur(130)));
 // cc = cc + 10;
 // if(cc > 600){cc = 130;}
  float f = ADS.toVoltage(1); // voltage factor
  int16_t adc0 = ADS.readADC(0);
  float lambda = 0.68 + (adc0 * f * 0.68 /5); //AFR 10-20 Lambda 0,68 - 1,36 (0-5V)
  int16_t inno_lambda = (lambda * 1000) - 500; //Innovate Air/Fuel Ratio = ((L12..L0) + 500)* (AF7..0) / 10000
  uint16_t val = compteur(inno_lambda);
  byte low = val & 0xff;
  byte high = (val >> 8) & 0xff;
  ch[4] = high;
  ch[5] = low;

  for(int i=0;i<6;i++){
  Serial.write(ch[i]);
  //Print_Hexa(ch[i]);
  }
  delay(50);
}
void Print_Hexa(uint16_t num) {
  char Hex_Array[4];

  sprintf(Hex_Array, "%02X", num);
  Serial.println(Hex_Array);
}
uint16_t compteur(uint16_t n){
      uint16_t  iWordLo = n & 0x7F;
      uint16_t  iWordHi = n & 0xFF80;
      uint16_t  iWord = iWordHi * 2 + iWordLo;
      return iWord;
 }
