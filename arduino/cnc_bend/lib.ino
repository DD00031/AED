#include "config.h"

// Move stepper
// Input:
//    steps: number of steps (-) (int)
//    stepper: array met: [stepPin, dirPin] 
//    dir: direction that the stepper should move (TRUE = forward, FALSE = backward) (bool)
void moveStepper(int steps, int stepper[], bool dir){
  digitalWrite(stepper[1], dir);
  delayMicroseconds(5);

  for(int x = 0; x<steps; x++) {
   digitalWrite(stepper[0],HIGH);
   delayMicroseconds(500);
   digitalWrite(stepper[0],LOW); 
   delayMicroseconds(500);
  }
}

// DISCLAMER: The following code for the serial monitor control has been written by Claude. 
// This is because this isn't a vital part of the code for the functioning of our machine, and is just for convience. 
// Please do not use Claude (or any ai) to write vital code.
//
// Checks if text is a valid number (digits, optional leading '-', optional '.')
// Input:
//    s: text to check (String)
// Output:
//    true if s is a number (bool)
bool isNumber(String s) {
  if (s.length() == 0) return false;
  bool dot = false;
  for (unsigned int i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '-' && i == 0 && s.length() > 1) continue;
    if (c == '.' && !dot) { dot = true; continue; }
    if (c < '0' || c > '9') return false;
  }
  return true;
}

// Reads one line from the Serial Monitor and runs the matching command.
// Call this every loop. It does nothing when no line was typed.
// Commands:
//    start               -> main_function()
//    transport <mm>      -> transportStrip(mm)
//    bend <deg>          -> bendPlate(deg)
//    torsion <deg>       -> torsionPlate(deg)
void handleSerialCommands() {
  if (Serial.available() == 0) return;

  String line = Serial.readStringUntil('\n');
  line.trim();
  line.toLowerCase();
  if (line.length() == 0) return;

  // split "transport 50" into cmd = "transport", arg = "50"
  String cmd = line;
  String arg = "";
  int space = line.indexOf(' ');
  if (space != -1) {
    cmd = line.substring(0, space);
    arg = line.substring(space + 1);
    arg.trim();
  }

  if (cmd == "start") {
    main_function();
    Serial.println("Done.");
  }
  else if (cmd == "transport") {
    if (!isNumber(arg)) { Serial.println("Usage: transport <mm>"); return; }
    transportStrip(arg.toInt());
    Serial.println("Done.");
  }
  else if (cmd == "bend") {
    if (!isNumber(arg)) { Serial.println("Usage: bend <deg>"); return; }
    bendPlate(arg.toFloat());
    Serial.println("Done.");
  }
  else if (cmd == "torsion") {
    if (!isNumber(arg)) { Serial.println("Usage: torsion <deg>"); return; }
    torsionPlate(arg.toFloat());
    Serial.println("Done.");
  }
    else if (cmd == "stage") {
    if (!isNumber(arg)) { Serial.println("Usage: stage <mm>"); return; }
    moveStage(arg.toFloat());
    Serial.println("Done.");
  }
  else {
    Serial.println("Unknown command. Use: start, transport <mm>, bend <deg>, torsion <deg>, stage <mm>");
  }
}