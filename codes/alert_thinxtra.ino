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
  const uint8_t payloadSize = 2; //in bytes
  uint8_t bytes[2];
  bytes[0] = 0;
  bytes[1] = 2;
  
  Serial.println("sending payload");
  Send_Pload(bytes, payloadSize);

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
