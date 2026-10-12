#include "config.h"

void initStage(){
  pinMode(stageStep, OUTPUT);
  pinMode(stageDir, OUTPUT);
}

void moveStage(float stageDist){
  int steps;
  bool stageDirection;
  if (stageDist < 0){
    stageDirection = false;
    stageDist = abs(stageDist);
  }
  else {
    stageDirection = true;
  }
  steps = 5;
  moveStepper(steps, transport, stageDirection);

  Serial.print("Moved stage ");
  Serial.print(stageDist);
  Serial.print("mm");
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