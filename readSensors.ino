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

#include <Tsensors.h>
#include <Wire.h>
#include <math.h>
#include <avr/wdt.h>

/**********************************************************
 * Constants and definitions
 *********************************************************/

Tsensors *tSensors = new Tsensors();
int counter = 0;

/**********************************************************
 * Auxiliar functions
 *********************************************************/

void read_sensors(){
  float temperature = (tSensors->getTemp());
  Serial.print("temperature:");
  Serial.println(temperature); 
}

/**********************************************************
 * Main Functions
 *********************************************************/

void setup() {
  Wire.begin();
  Wire.setClock(100000);

  Serial.begin(9600);
  // Init sensors on Thinxtra Module
  tSensors->initSensors();
  Serial.println(""); // Make a clean start
  delay(1000);

}

void loop() {
  read_sensors();
  delay(1000);
}
