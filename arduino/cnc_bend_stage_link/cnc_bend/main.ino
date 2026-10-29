#include "config.h"

void main_function(){
  if (!armed) {
    Serial.println("Not armed! Type 'arm' first.");
    return;
  }
  Serial.println("Starting main function");
  delay(500);

  bendStrip(130);
  delay(200);
  bendStrip(-40);

  delay(500);
  if (success) {
    Serial.println("Main function finished");
  }
  else {
    Serial.println("Main function ABORTED! Check the messages above.");
  }
}

void test_function(){
  for(int x = 0; x<100; x++){
    transportStrip(50);
    delay(500);
    transportStrip(-50);
    delay(500);
  }
}
