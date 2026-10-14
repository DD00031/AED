#include "config.h"

void initBending(){
  pinMode(bendingStep, OUTPUT);
  pinMode(bendingDir, OUTPUT);
}

void bendPlate(float bendAngle){
  if (!checkArmed()) return;
  float redBendingAngle;
  int steps;
  bool bendingDirection;

  if (bendAngle < 0){
    bendingDirection = false;
    bendAngle = abs(bendAngle);
  }
  else {
    bendingDirection = true;
  }

  redBendingAngle = angleGearReduction(bendAngle, bendingGearReduction);
  steps = n_steps(redBendingAngle);
  moveStepper(steps, bending, bendingDirection);

  Serial.print("Bend plate ");
  Serial.print(bendAngle);
  Serial.println("deg");
}

void standardBend() {
  transportStrip(bendTransportDist);
  delay(500);
  moveStage(bendStageDist);
  delay(500);
  bendPlate(90);
  delay(500);
  moveStage(-1 * bendStageDist);
  delay(200);
  bendPlate(-90);
}