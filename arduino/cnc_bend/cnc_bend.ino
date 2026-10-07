#include "config.h"

void setup() {
  Serial.begin(9600);
  initTransport();
  initTorsion();
  initBending();
}

void loop() {
  Serial.println("Blink");
  delay(1000);
  Serial.println("Blonk");
  delay(1000);
}