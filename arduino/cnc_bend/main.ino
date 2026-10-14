#include "config.h"

void main_function(){
  Serial.println("Starting main function");
  delay(500);
  
  standardBend();
  
  delay(500);
  Serial.println("Main function finished");
}