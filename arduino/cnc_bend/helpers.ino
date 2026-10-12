#include "config.h"

// Calculates circumference circle
float d_circ(int r){
  float result;
  result = 2 * r * PI;
  return result;
}

// Calculates angle needed to move provided distance
// Inputs:
//    r: radius (mm) (int)
//    d: distance (mm) (int)
// Outputs:
//    angle: angle needed to move distance (deg) (float)
float angle(int r, int d){
  float circ;
  float dist_per_deg;
  float result; 
  circ = 2 * r * PI;
  dist_per_deg = circ / 360;
  result = d / dist_per_deg;
  return result;
}

// Calculates number of steps needed to reach provided angle
// Inputs:
//    angle: angle that the stepper should move (mm) (float)
// Outputs: 
//    steps: # of steps needed to move angle (-) (int)
int n_steps(float angle){
  int result;
  result = angle / 1.8;
  return result;
}

// Waits until the user types the given word in the Serial Monitor.
// Input:
//    word: text that must be typed to continue (String)
void waitForCommand(String word) {
  Serial.print("Type '");
  Serial.print(word);
  Serial.println("' and press Enter to start.");

  while (true) {
    if (Serial.available() > 0) {
      String input = Serial.readStringUntil('\n');
      input.trim();                 // removes spaces and \r
      input.toLowerCase();          // accepts Start, START, start

      if (input == word) {
        return;
      }
      Serial.println("Not recognised, try again.");
    }
  }
}

