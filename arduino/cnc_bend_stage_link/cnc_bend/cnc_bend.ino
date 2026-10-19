#include <SoftwareSerial.h>
#include "config.h"

void setup() {
  Serial.begin(9600);
  initArm();
  initTransport();
  initTorsion();
  initBending();
  initStage();

  Serial.println("Ready. Drivers disarmed.");
}

void loop() {
  handleSerialCommands();
}