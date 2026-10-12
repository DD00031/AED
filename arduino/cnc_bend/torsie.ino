#include "config.h"

void initTorsion(){
  pinMode(torsionStep, OUTPUT);
  pinMode(torsionDir, OUTPUT);
}

void torsionPlate(int torsionAngle){
  Serial.print("Torsion placeholder, entered angle: ");
  Serial.println(torsionAngle);
}