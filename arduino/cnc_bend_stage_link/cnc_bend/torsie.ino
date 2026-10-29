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

// Returns true if the whole sequence was completed, false if it was aborted
bool standardTorsion(){
  transportStrip(torsionTransportDist);
  delay(500);
  if (!moveStage(torsionStageDist)) {
    Serial.println("Aborted: stage did not move in. Plate is fed through, torsion module not moved.");
    return false;
  }
  delay(500);
  torsionPlate(120);
  delay(500);
  torsionPlate(-30);
  delay(500);
  if (!moveStage(-1 * torsionStageDist)) {
    Serial.println("Aborted: stage did not move back. Torsion module is still at 90 deg!");
    return false;
  }
  delay(500);
  torsionPlate(-90);
  delay(500);
  return true;
}
