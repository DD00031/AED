#include "config.h"

void main_function(){
  if (!armed) {
    Serial.println("Not armed! Type 'arm' first.");
    return;
  }
  Serial.println("Starting main function");
  delay(500);

  bool success = standardBend();

  delay(500);
  if (success) {
    Serial.println("Main function finished");
  }
  else {
    Serial.println("Main function ABORTED! Check the messages above.");
  }
}
