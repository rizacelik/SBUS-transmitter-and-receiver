#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>


#define RC_CHANNEL_MIN 990
#define RC_CHANNEL_MAX 2010

#define SBUS_MIN_OFFSET 173
#define SBUS_MID_OFFSET 992
#define SBUS_MAX_OFFSET 1811
#define SBUS_CHANNEL_NUMBER 16
#define SBUS_PACKET_LENGTH 25
#define SBUS_FRAME_HEADER 0x0f
#define SBUS_FRAME_FOOTER 0x00
#define SBUS_FRAME_FOOTER_V2 0x04
#define SBUS_STATE_FAILSAFE 0x08
#define SBUS_STATE_SIGNALLOSS 0x04
#define SBUS_UPDATE_RATE 15  //ms


Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;
Servo ch7;



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

joystick data;

const uint64_t pipeOut = 000322;

RF24 radio(9, 8);

// Default value

void ResetData() {
  data.potLX = 0;
  data.potLY = 127;
  data.potRX = 127;
  data.potRY = 127;
  data.analog1 = 0;
  data.analog2 = 0;
  data.analog3 = 0;
  data.aux1 = 0;
  data.aux2 = 0;
  data.aux3 = 0;
  data.aux4 = 0;
  data.aux5 = 0;
}

void sbusPreparePacket(uint8_t packet[], int channels[], bool isSignalLoss, bool isFailsafe) {

  static int output[SBUS_CHANNEL_NUMBER] = { 0 };

  /*
     * Map 1000-2000 with middle at 1500 chanel values to
     * 173-1811 with middle at 992 S.BUS protocol requires
     */
  for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
    output[i] = map(channels[i], RC_CHANNEL_MIN, RC_CHANNEL_MAX, SBUS_MIN_OFFSET, SBUS_MAX_OFFSET);
  }

  uint8_t stateByte = 0x00;
  if (isSignalLoss) {
    stateByte |= SBUS_STATE_SIGNALLOSS;
  }
  if (isFailsafe) {
    stateByte |= SBUS_STATE_FAILSAFE;
  }
  packet[0] = SBUS_FRAME_HEADER;  //Header

  packet[1] = (uint8_t)(output[0] & 0x07FF);
  packet[2] = (uint8_t)((output[0] & 0x07FF) >> 8 | (output[1] & 0x07FF) << 3);
  packet[3] = (uint8_t)((output[1] & 0x07FF) >> 5 | (output[2] & 0x07FF) << 6);
  packet[4] = (uint8_t)((output[2] & 0x07FF) >> 2);
  packet[5] = (uint8_t)((output[2] & 0x07FF) >> 10 | (output[3] & 0x07FF) << 1);
  packet[6] = (uint8_t)((output[3] & 0x07FF) >> 7 | (output[4] & 0x07FF) << 4);
  packet[7] = (uint8_t)((output[4] & 0x07FF) >> 4 | (output[5] & 0x07FF) << 7);
  packet[8] = (uint8_t)((output[5] & 0x07FF) >> 1);
  packet[9] = (uint8_t)((output[5] & 0x07FF) >> 9 | (output[6] & 0x07FF) << 2);
  packet[10] = (uint8_t)((output[6] & 0x07FF) >> 6 | (output[7] & 0x07FF) << 5);
  packet[11] = (uint8_t)((output[7] & 0x07FF) >> 3);
  packet[12] = (uint8_t)((output[8] & 0x07FF));
  packet[13] = (uint8_t)((output[8] & 0x07FF) >> 8 | (output[9] & 0x07FF) << 3);
  packet[14] = (uint8_t)((output[9] & 0x07FF) >> 5 | (output[10] & 0x07FF) << 6);
  packet[15] = (uint8_t)((output[10] & 0x07FF) >> 2);
  packet[16] = (uint8_t)((output[10] & 0x07FF) >> 10 | (output[11] & 0x07FF) << 1);
  packet[17] = (uint8_t)((output[11] & 0x07FF) >> 7 | (output[12] & 0x07FF) << 4);
  packet[18] = (uint8_t)((output[12] & 0x07FF) >> 4 | (output[13] & 0x07FF) << 7);
  packet[19] = (uint8_t)((output[13] & 0x07FF) >> 1);
  packet[20] = (uint8_t)((output[13] & 0x07FF) >> 9 | (output[14] & 0x07FF) << 2);
  packet[21] = (uint8_t)((output[14] & 0x07FF) >> 6 | (output[15] & 0x07FF) << 5);
  packet[22] = (uint8_t)((output[15] & 0x07FF) >> 3);

  packet[23] = stateByte;          //Flags byte
  packet[24] = SBUS_FRAME_FOOTER;  //Footer
}

uint8_t sbusPacket[SBUS_PACKET_LENGTH];
int rcChannels[SBUS_CHANNEL_NUMBER];

uint32_t currentMillis = 0;
uint32_t sbusTime = 0;

unsigned long lastRecvTime = 0;
unsigned long now = 0;


void setup() {

  ch1.attach(2);  // Sorvo or ESC output D2
  ch2.attach(3);  // Sorvo or ESC output D3
  ch3.attach(4);  // Sorvo or ESC output D4
  ch4.attach(5);  // Sorvo or ESC output D5
  ch5.attach(6);  // Sorvo or ESC output D6
  ch6.attach(7);  // Sorvo or ESC output D6
  ch7.attach(10); // Sorvo or ESC output D10


  ResetData();  // Defaukt channel value
  delay(1000);

  radio.begin();
  radio.openReadingPipe(1,pipeOut);
  radio.setChannel(125);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();



  delay(1000);
  for (uint8_t i = 0; i < SBUS_CHANNEL_NUMBER; i++) {
    rcChannels[i] = 1500;
    if(i == 2)  rcChannels[i] = 1000;
  }
  delay(1000);
  Serial.begin(100000, SERIAL_8E2); 
}

void loop() {

  if (radio.available()) {
    radio.read(&data, sizeof(joystick));
    lastRecvTime = millis();
  } 

  
  now = millis();
  if (now - lastRecvTime > 1000) {
    ResetData();
  }


 int randNumber = 1;


  rcChannels[0] = map(data.potRY, 0, 255, 1000, 2000);
  rcChannels[1] = map(255 - data.potRX, 0, 255, 1000, 2000);
  rcChannels[2] = map(data.potLX, 0, 255, 1000, 2000);
  rcChannels[3] = map(data.potLY, 0, 255, 1000, 2000);
  rcChannels[4] = map(data.analog1, 0, 255, 1000, 2000);
  rcChannels[5] = map(data.analog2, 0, 255, 1000, 2000);
  rcChannels[6] = map(data.analog3, 0, 255, 1000, 2000);
  rcChannels[7] = map(data.aux1, 0, 1, 1000, 2000);
  rcChannels[8] = map(data.aux2, 0, 1, 1000, 2000);
  rcChannels[9] = map(data.aux3, 0, 1, 1000, 2000);
  rcChannels[10] = map(data.aux4, 0, 1, 1000, 2000);
  rcChannels[11] = map(data.aux5, 0, 1, 1000, 2000);


  
  currentMillis = millis();

  if (currentMillis > sbusTime) {
    sbusPreparePacket(sbusPacket, rcChannels, false, false);
    Serial.write(sbusPacket, SBUS_PACKET_LENGTH);
    sbusTime = currentMillis + SBUS_UPDATE_RATE;

    // Write the PWM signal

    ch1.writeMicroseconds(rcChannels[0]);
    ch2.writeMicroseconds(rcChannels[1]);
    ch3.writeMicroseconds(rcChannels[2]);
    ch4.writeMicroseconds(rcChannels[3]);
    ch5.writeMicroseconds(rcChannels[4]);
    ch6.writeMicroseconds(rcChannels[5]);
    ch7.writeMicroseconds(rcChannels[6]);
  }
}
