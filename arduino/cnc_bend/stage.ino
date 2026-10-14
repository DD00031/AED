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
  Serial.println("mm");
}
