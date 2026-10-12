#include "config.h"

void setup() {
  Serial.begin(9600);
  initTransport();
  initTorsion();
  initBending();
  initStage();

  Serial.println("Ready.");
//  transportStrip(100);
//  Serial.println("Done.");
}

void loop() {
  handleSerialCommands();
}