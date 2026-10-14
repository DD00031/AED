#include "config.h"

void main_function(){
  if (!armed) {
    Serial.println("Not armed! Type 'arm' first.");
    return;
  }
  Serial.println("Starting main function");
  delay(500);
  
  standardBend();
  
  delay(500);
  Serial.println("Main function finished");
}