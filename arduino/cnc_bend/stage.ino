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

  float stageAngle = (stageDist * 360) / pitch;
  steps = n_steps(stageAngle);
  moveStepper(steps, stage, stageDirection);

  Serial.print("Moved stage ");
  Serial.print(stageDist);
  Serial.println("mm");
}
