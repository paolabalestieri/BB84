#include "Wire.h"

#define ledPin 13 
  
#define myAdress 0x08

void setup() {
  Wire.begin(myAdress);

  Wire.onReceive(receiveEvent);

  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  
}

void loop() {
}

void receiveEvent(int howMany) {
  if (Wire.available()) {
    char received = Wire.read();
    Serial.println(received);

    if (received == 0) {
      digitalWrite(ledPin, LOW);
    }

    if (received == 1) {
      digitalWrite(ledPin, HIGH);
    }
  }
}
