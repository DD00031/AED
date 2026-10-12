#include "config.h"

void initTransport(){
  pinMode(transportStep, OUTPUT);
  pinMode(transportDir, OUTPUT);
}

void transportStrip(int d) {
  float a;
  int steps;
  bool transportDirection;

  if (d < 0){
    transportDirection = false;
    d = abs(d);
  }
  else {
    transportDirection = true;
  }

  a = angle(rWheel, d);
  steps = n_steps(a);
  moveStepper(steps, transport, transportDirection);

  Serial.print("Moved strip ");
  Serial.print(d);
  Serial.print("mm, over ");
  Serial.print(a);
  Serial.print("deg and ");
  Serial.print(steps);
  Serial.println("steps");
}