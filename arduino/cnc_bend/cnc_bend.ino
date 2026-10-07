#include "config.h"

void setup() {
  Serial.begin(9600);
  blinkSetup();
}

void loop() {
  Serial.println("Blink");
  delay(1000);
  blinkLoop();
  delay(1000);
}