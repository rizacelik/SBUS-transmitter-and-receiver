/*
https://github.com/nRF24/RF24
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct joystick {
  byte potLX;
  byte potLY;
  byte potRX;
  byte potRY;
  byte analog1;
  byte analog2;
  byte analog3;
  byte aux1;
  byte aux2;
  byte aux3;
  byte aux4;
  byte aux5;
};

joystick joystickData;

RF24 radio(9, 8); // CE, CSN;

const uint64_t pipeOut = 000322;


void setup() {

  //Serial.begin(57600);

  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP); 
  
  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setChannel(125);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  
 /*
  // Inicia Radio
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.stopListening();
  radio.openWritingPipe(pipeOut);
   */

}

void loop() {

  int x = constrain(1023 - analogRead(A3), 513, 1023);
  joystickData.potLX = map(x , 513, 1023, 0, 255);
  joystickData.potLY = map(analogRead(A2), 0, 1023, 0, 255);
  joystickData.potRX = map(analogRead(A1), 0, 1023, 0, 255);
  joystickData.potRY = map(analogRead(A0), 0, 1023, 0, 255);

  joystickData.analog1 = map(analogRead(A4), 0, 1023, 0, 255);
  joystickData.analog2 = map(analogRead(A5), 0, 1023, 0, 255);
  joystickData.analog3 = map(analogRead(A6), 0, 1023, 0, 255);
 

  if (digitalRead(2)) {
    bitWrite(joystickData.aux1, 0, 0);
  } else {
    bitWrite(joystickData.aux1, 0, 1);
  }

  if (digitalRead(3)) {
    bitWrite(joystickData.aux2, 0, 0);
  } else {
    bitWrite(joystickData.aux2, 0, 1);
  }

  if (digitalRead(4)) {
    bitWrite(joystickData.aux3, 0, 0);
  } else {
    bitWrite(joystickData.aux3, 0, 1);
  }

  if (digitalRead(5)) {
    bitWrite(joystickData.aux4, 0, 0);
  } else {
    bitWrite(joystickData.aux4, 0, 1);
  }

  if (digitalRead(6)) {
    bitWrite(joystickData.aux5, 0, 0);
  } else {
    bitWrite(joystickData.aux5, 0, 1);
  }
/*
Serial.print("LX:");
Serial.print(joystickData.potLX); 
Serial.print(" LY:");
Serial.print(joystickData.potLY); 
Serial.print(" RX:");
Serial.print(joystickData.potRX); 
Serial.print(" RY:");
Serial.print(joystickData.potRY); 
Serial.print("   A1:");
Serial.print(analogRead(A4)); 
Serial.print(" A2:");
Serial.print(analogRead(A5)); 
Serial.print(" A3:");
Serial.print(analogRead(A6)); 

Serial.print("   B1:");
Serial.print(digitalRead(3)); 
Serial.print(" B2:");
Serial.print(digitalRead(4)); 
Serial.print(" B3:");
Serial.print(digitalRead(5)); 
Serial.print(" B4:");
Serial.print(digitalRead(6)); 
Serial.print(" B5:");
Serial.println(digitalRead(7)); 
*/
  // Send write data to RF
  radio.write(&joystickData, sizeof(joystick));
  //delay(50);

}
