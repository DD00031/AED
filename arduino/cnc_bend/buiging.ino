#include "config.h"

void initBending(){
  pinMode(bendingStep, OUTPUT);
  pinMode(bendingDir, OUTPUT);
}

void bendPlate(int bendAngle){
  Serial.print("Bending placeholder, entered angle: ");
  Serial.println(bendAngle);

  
}