#include "Wire.h"

#define buttonPin 4 
    
#define slaveAdress 0x08

boolean buttonState;             
boolean lastButtonState = LOW;   
boolean ledState = HIGH;         

void setup() {
  Wire.begin();
  
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(buttonPin);

	if (reading == HIGH) {
		ledState = HIGH;
	}
	else{
		ledState = LOW;
	}
	Wire.beginTransmission(slaveAdress);
	Wire.write(ledState); 
	Wire.endTransmission(); 
}
