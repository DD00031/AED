#include "config.h"

void initTorsion(){
  pinMode(torsionStep, OUTPUT);
  pinMode(torsionDir, OUTPUT);
}

void torsionPlate(float torsionAngle){
  if (!checkArmed()) return;
  float redTorsionAngle;
  int steps;
  bool torsionDirection;

  if (torsionAngle < 0){
    torsionDirection = false;
    torsionAngle = abs(torsionAngle);
  }
  else {
    torsionDirection = true;
  }

  redTorsionAngle = angleGearReduction(torsionAngle, torsionGearReduction);
  steps = n_steps(redTorsionAngle);
  moveStepper(steps, torsion, torsionDirection);

  Serial.print("Twisted plate ");
  Serial.print(torsionAngle);
  Serial.println("deg");
}

void standardTorsion(){
  transportStrip(torsionTransportDist);
  delay(500);
  moveStage(torsionStageDist);
  delay(500);
  torsionPlate(90);
  delay(500);
  moveStage(-1 * torsionStageDist);
  delay(200);
  torsionPlate(-90);
}