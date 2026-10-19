#include "config.h"

// The stage is driven by a second Arduino. This tab only talks to it over SoftwareSerial.
// Commands we send (one line each, ending with a newline):
//    arm / disarm      -> reply "ok"
//    ping              -> reply "ok"
//    move <steps>      -> reply "done" when finished (negative steps = backwards)
// Errors are answered with a line that starts with "err".

void initStage(){
  stageLink.begin(stageLinkBaud);
}

// Reads one line from the stage Arduino.
// Input:
//    line: the received text is stored here (String)
//    timeoutMs: how long to wait (ms) (unsigned long)
// Output:
//    true if a line was received, false on timeout (bool)
bool stageReadLine(String &line, unsigned long timeoutMs){
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    if (stageLink.available() > 0) {
      line = stageLink.readStringUntil('\n');
      line.trim();
      if (line.length() > 0) {
        return true;
      }
    }
  }
  return false;
}

// Sends one command to the stage Arduino and waits for the expected reply.
// Input:
//    command: text to send, for example "arm" (String)
//    expected: reply that means success, for example "ok" or "done" (String)
//    timeoutMs: how long to wait for the reply (ms) (unsigned long)
// Output:
//    true if the expected reply was received (bool)
bool stageSend(String command, String expected, unsigned long timeoutMs){
  // throw away old messages (for example "ready" after the stage Arduino booted)
  while (stageLink.available() > 0) {
    stageLink.read();
  }

  stageLink.println(command);

  String reply;
  unsigned long start = millis();
  while (millis() - start < timeoutMs) {
    unsigned long left = timeoutMs - (millis() - start);
    if (!stageReadLine(reply, left)) {
      break;
    }
    if (reply == expected) {
      return true;
    }
    if (reply.startsWith("err")) {
      Serial.print("Stage Arduino: ");
      Serial.println(reply);
      return false;
    }
    // any other line is ignored
  }

  Serial.println("Stage Arduino did not answer (timeout). Check the cables and that it is powered.");
  return false;
}

// Moves the stage.
// Input:
//    stageDist: distance (mm) (float), negative = backwards
// Output:
//    true if the stage reported that the move is finished (bool)
bool moveStage(float stageDist){
  if (!checkArmed()) return false;

  int steps;
  float stageAngle = (abs(stageDist) * 360) / pitch;
  steps = n_steps(stageAngle);
  if (stageDist < 0) {
    steps = -steps;
  }

  // moving takes about 1 ms per step, so wait that long plus a margin
  unsigned long timeoutMs = (unsigned long)abs(steps) + stageMoveMargin;
  if (!stageSend("move " + String(steps), "done", timeoutMs)) {
    Serial.println("Stage move failed.");
    return false;
  }

  Serial.print("Moved stage ");
  Serial.print(stageDist);
  Serial.print("mm, over ");
  Serial.print(steps);
  Serial.println("steps");
  return true;
}
