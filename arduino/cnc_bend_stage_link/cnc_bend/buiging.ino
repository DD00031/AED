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

  if (bendAngle > 0){
    bendingDirection = false;
  }
  else {
    bendingDirection = true;
    bendAngle = abs(bendAngle);
  }

  redBendingAngle = angleGearReduction(bendAngle, bendingGearReduction);
  steps = n_steps(redBendingAngle);
  moveStepper(steps, bending, bendingDirection);

  Serial.print("Bend plate ");
  Serial.print(bendAngle);
  Serial.println("deg");
}

// Returns true if the whole sequence was completed, false if it was aborted
bool standardBend() {
  transportStrip(bendTransportDist);
  delay(500);
  if (!moveStage(bendStageDist)) {
    Serial.println("Aborted: stage did not move in. Plate is fed through, bender not moved.");
    return false;
  }
  delay(500);
  bendPlate(90);
  delay(500);
  if (!moveStage(-1 * bendStageDist)) {
    Serial.println("Aborted: stage did not move back. Bender is still at 90 deg!");
    return false;
  }
  delay(200);
  bendPlate(-90);
  return true;
}
