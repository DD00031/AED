// Stage Arduino: runs on the SECOND Arduino and only moves the stage stepper.
// The main Arduino (cnc_bend) sends it commands over a SoftwareSerial link.
// You can also type the same commands in the Serial Monitor of this Arduino to test it.
//
// Commands (one line each, ending with a newline):
//    ping              -> reply "ok"
//    arm               -> enable the driver, reply "ok"
//    disarm            -> disable the driver, reply "ok"
//    status            -> reply "ok armed" or "ok disarmed"
//    move <steps>      -> move that many steps (negative = backwards), reply "done" when finished
// Errors are answered with a line that starts with "err".

#include <SoftwareSerial.h>

// --- Pins on THIS Arduino (change to match your wiring) ---
const int stepPin = 3;
const int dirPin = 6;
const int enablePin = 8;

// --- Link to the main Arduino ---
// Cross the wires: this TX -> main RX and this RX <- main TX. Also connect GND to GND.
const int linkRx = 9;      // this RX <- TX of the main Arduino
const int linkTx = 10;     // this TX -> RX of the main Arduino
const long linkBaud = 9600;

// --- Other ---
const long maxSteps = 6000
;   // bigger moves are refused

SoftwareSerial link(linkRx, linkTx);
bool armed = false;

void setup() {
  Serial.begin(9600);          // USB, for testing from the computer
  link.begin(linkBaud);

  digitalWrite(enablePin, HIGH);   // start disarmed (HIGH = driver off)
  pinMode(enablePin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);

  Serial.println("Stage Arduino ready. Driver disarmed.");
  link.println("ready");
}

void loop() {
  handleLine(Serial);   // commands typed in the Serial Monitor
  handleLine(link);     // commands from the main Arduino
}

// Checks if text is a whole number (digits, optional leading '-')
bool isInteger(String s) {
  if (s.length() == 0) return false;
  for (unsigned int i = 0; i < s.length(); i++) {
    char c = s[i];
    if (c == '-' && i == 0 && s.length() > 1) continue;
    if (c < '0' || c > '9') return false;
  }
  return true;
}

// Sends the reply to the sender and also shows it in the Serial Monitor
void reply(Stream &to, const char *message) {
  to.println(message);
  if (&to != &Serial) {
    Serial.print("-> ");
    Serial.println(message);
  }
}

// Moves the stepper.
// Input:
//    steps: number of steps (-) (long), negative = backwards
void moveSteps(long steps) {
  digitalWrite(dirPin, steps >= 0 ? HIGH : LOW);
  delayMicroseconds(5);

  long n = abs(steps);
  for (long i = 0; i < n; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(500);
  }
}

// Reads one line from the given stream (if there is one) and runs the command
void handleLine(Stream &in) {
  if (in.available() == 0) return;

  String line = in.readStringUntil('\n');
  line.trim();
  line.toLowerCase();
  if (line.length() == 0) return;

  if (&in != &Serial) {
    Serial.print("<- ");
    Serial.println(line);
  }

  // split "move 200" into cmd = "move", arg = "200"
  String cmd = line;
  String arg = "";
  int space = line.indexOf(' ');
  if (space != -1) {
    cmd = line.substring(0, space);
    arg = line.substring(space + 1);
    arg.trim();
  }

  if (cmd == "ping") {
    reply(in, "ok");
  }
  else if (cmd == "arm") {
    digitalWrite(enablePin, LOW);
    armed = true;
    reply(in, "ok");
  }
  else if (cmd == "disarm") {
    digitalWrite(enablePin, HIGH);
    armed = false;
    reply(in, "ok");
  }
  else if (cmd == "status") {
    reply(in, armed ? "ok armed" : "ok disarmed");
  }
  else if (cmd == "move") {
    if (!isInteger(arg)) {
      reply(in, "err usage: move <steps>");
      return;
    }
    if (!armed) {
      reply(in, "err not armed");
      return;
    }
    long steps = arg.toInt();
    if (abs(steps) > maxSteps) {
      reply(in, "err too large");
      return;
    }
    moveSteps(steps);
    reply(in, "done");
  }
  else {
    reply(in, "err unknown command");
  }
}
