#include "config.h"

void main_function(){
  if (!armed) {
    Serial.println("Not armed! Type 'arm' first.");
    return;
  }
  Serial.println("Starting main function...");
  Serial.println("Order: Bend, Twist, Straight, Twist, Bend.")
  delay(500);
  
  standardBend();
  standardTorsion();
  transportStrip(50);
  standardTorsion();
  standardBend();
  
  delay(500);
  Serial.println("Main function finished.");
}