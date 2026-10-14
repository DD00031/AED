#include "config.h"

void initTorsion(){
  pinMode(torsionStep, OUTPUT);
  pinMode(torsionDir, OUTPUT);
}

void torsionPlate(int torsionAngle){
  int redTorsionAngle;
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