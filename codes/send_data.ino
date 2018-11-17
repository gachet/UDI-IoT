/**********************************************************
 * 
 * 
 * Example by Jose Garcia
 * github.com/jotathebest
 * 
 * Based on original example by Thinxtra
 * 
 * 
 *********************************************************/

/**********************************************************
 * Libraries to include
 *********************************************************/


#include <WISOL.h>
#include <Tsensors.h>
#include <Wire.h>
#include <math.h>
#include <avr/wdt.h>

/**********************************************************
 * Constants and definitions
 *********************************************************/
typedef union{
    float number;
    uint8_t bytes[4];
} FLOATUNION_t;

typedef union{
    uint16_t number;
    uint8_t bytes[2];
} UINT16_t;

typedef union{
    int16_t number;
    uint8_t bytes[2];
} INT16_t;

Isigfox *Isigfox = new WISOL();
Tsensors *tSensors = new Tsensors();

uint8_t buttonCounter;
const uint8_t buttonPin = A1;

/**********************************************************
 * Auxiliar functions
 *********************************************************/
// SendPayload Function => Send messages to the Sigfox Network
void Send_Pload(uint8_t *sendData, int len) {
  recvMsg *RecvMsg;

  RecvMsg = (recvMsg *)malloc(sizeof(recvMsg));
  Isigfox->sendPayload(sendData, len, 0, RecvMsg);
  for (int i = 0; i < RecvMsg->len; i++) {
    Serial.print(RecvMsg->inData[i]);
  }
  Serial.println("");
  free(RecvMsg);
}

void Send_Sensors(){
  UINT16_t tempt, photo, pressure;
  INT16_t x_g, y_g, z_g;
  acceleration_xyz *xyz_g;
  FLOATUNION_t a_g;

  // Sending a float requires at least 4 bytes
  // In this demo, the measure values (temperature, pressure, sensor) are scaled to ranged from 0-65535.
  // Thus they can be stored in 2 bytes
  tempt.number = (uint16_t) (tSensors->getTemp() * 100);
  Serial.print("Temp: "); Serial.println((float)tempt.number/100);
  pressure.number =(uint16_t) (tSensors->getPressure()/3);
  Serial.print("Pressure: "); Serial.println((float)pressure.number*3);
  photo.number = (uint16_t) (tSensors->getPhoto() * 1000);
  Serial.print("Photo: "); Serial.println((float)photo.number/1000);

  xyz_g = (acceleration_xyz *)malloc(sizeof(acceleration_xyz));
  tSensors->getAccXYZ(xyz_g);
  x_g.number = (int16_t) (xyz_g->x_g * 250);
  y_g.number = (int16_t) (xyz_g->y_g * 250);
  z_g.number = (int16_t) (xyz_g->z_g * 250);
  Serial.print("Acc X: "); Serial.println((float)x_g.number/250);
  Serial.print("Acc Y: "); Serial.println((float)y_g.number/250);
  Serial.print("Acc Z: "); Serial.println((float)z_g.number/250);
  Serial.print("\0");
  free(xyz_g);

  const uint8_t payloadSize = 12; //in bytes
//  byte* buf_str = (byte*) malloc (payloadSize);
  uint8_t buf_str[payloadSize];

  buf_str[0] = tempt.bytes[0];
  buf_str[1] = tempt.bytes[1];
  buf_str[2] = pressure.bytes[0];
  buf_str[3] = pressure.bytes[1];
  buf_str[4] = photo.bytes[0];
  buf_str[5] = photo.bytes[1];
  buf_str[6] = x_g.bytes[0];
  buf_str[7] = x_g.bytes[1];
  buf_str[8] = y_g.bytes[0];
  buf_str[9] = y_g.bytes[1];
  buf_str[10] = z_g.bytes[0];
  buf_str[11] = z_g.bytes[1];

  Serial.println("sending payload");
  Send_Pload(buf_str, payloadSize);
//  free(buf_str);
}

void checkPress() {
  while (digitalRead(buttonPin) != 1) {
    buttonCounter++;
    delay(50);
    if (buttonCounter > 4) break;
  }

  if (buttonCounter > 4) {
    Send_Sensors();
    delay(1000);
  }

  buttonCounter = 0;
}

/**********************************************************
 * Main Functions
 *********************************************************/

void setup() {
  int flagInit;
  
  Wire.begin();
  Wire.setClock(100000);

  Serial.begin(9600);

  // WISOL test
  flagInit = -1;
  while (flagInit == -1) {
    Serial.println(""); // Make a clean restart
    delay(1000);
    flagInit = Isigfox->initSigfox();
    Isigfox->testComms();
  }
  
  // Init sensors on Thinxtra Module
  tSensors->initSensors();
  buttonCounter = 0;

  Serial.println(""); // Make a clean start
  delay(1000);

}

void loop() {
  // put your main code here, to run repeatedly:
  checkPress();
}
