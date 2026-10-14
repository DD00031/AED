# CNC Bend Machine Code
Final code for arduino.

## List of files + Description
| File | Module / axis | Description |
| ---- | ------------- | ----------- |
| `cnc_bend.ino` | Main | Main code file, used for bringing the separate module code files together. Contains `setup()` and `loop()`. |
| `config.h` | Config | Contains all global constant and state variables. Also used for configuring order of operations. |
| `main.ino` | Main sequence | Contains `main_function()`, the full automatic sequence that runs when you type `start`. |
| `doorvoer.ino` | Transport (X) | `initTransport()` sets up the pins. `transportStrip(d)` feeds the strip d mm through the machine using the wheel radius (negative = backwards). |
| `torsie.ino` | Torsion (Y) | `initTorsion()` sets up the pins. `torsionPlate(angle)` twists the plate by an angle in degrees, corrected for the gearbox (negative = other direction). `standardTorsion()` does one full torsion: feed the plate, move the stage in, twist 90°, move the stage back and return the torsion module. |
| `buiging.ino` | Bending (Z) | `initBending()` sets up the pins. `bendPlate(angle)` bends the plate by an angle in degrees, corrected for the gearbox (negative = other direction). `standardBend()` does one full bend: feed the plate, move the stage in, bend 90°, move the stage back and return the bender. |
| `stage.ino` | Stage (A) | `initStage()` sets up the pins. `moveStage(dist)` moves the stage dist mm using the screw pitch (negative = backwards). |
| `helpers.ino` | Calculations | `angle(r, d)` gives the angle needed to move a distance over a wheel. `n_steps(angle)` gives the steps needed for an angle. `angleGearReduction(angle, reduction)` gives the motor angle needed for an angle after the gearbox. `d_circ(r)` gives the circumference. `getStatus()` prints whether the drivers are armed. Also contains `waitForCommand()`, which is currently unused. |
| `lib.ino` | Shared functions | `moveStepper()` sends the step pulses (only works when armed). `arm()`, `disarm()` and `initArm()` enable or disable all drivers through the enable pin. `checkArmed()` checks whether the drivers are armed. `handleSerialCommands()` and `isNumber()` handle the serial monitor control. |

## Usage
Upload the sketch, open the Serial Monitor (baud rate 9600, line ending "Newline" or "Both NL & CR") and type a command followed by Enter. Commands are not case sensitive. Numbers can be negative to move in the other direction. Decimals are allowed (for example `bend 22.5` or `transport 12.5`).

The drivers start **disarmed**, so type `arm` before anything else. After a reset or re-upload, the board is disarmed again.

| Command | Description |
| ------- | ----------- |
| `arm` | Enable the drivers (motors hold position and can move) |
| `disarm` | Disable the drivers (motors are free to turn by hand and no longer hold position) |
| `status` | Show whether the drivers are armed or disarmed |
| `start` | Run the full automatic sequence (`main_function()`) |
| `transport <mm>` | Feed the strip by the given distance in mm |
| `stage <mm>` | Move the stage by the given distance in mm |
| `bend <deg>` | Bend the plate by the given angle in degrees |
| `torsion <deg>` | Twist the plate by the given angle in degrees |

Examples:
```
arm
transport 50
bend -90
disarm
```

Notes:
- A move cannot be interrupted once it has started. Disarm after it finishes, or cut the 12-36V power for an emergency stop.
- Opening the Serial Monitor resets the board, so it is always disarmed after connecting.
- To add a command, add an `else if` block in `handleSerialCommands()` in `lib.ino` and add it to this table.

## Variables
(Keep in mind that these are prone to changing.)

### Constants (config.h)
| Variable in code | Value | Unit | Description |
| ---------------- | ----- | ---- | ----------- |
| `rWheel` | 43 | mm | Radius of the transport wheel |
| `bendStageDist` | 50 | mm | Distance the stage must move to get the plate into the bending module |
| `bendTransportDist` | 50 | mm | Distance the plate must be fed through for one bend |
| - | 90 | deg | Angle the wipe bender makes to bend the plate (hardcoded in `standardBend()`) |
| `bendingGearReduction` | 5 | - | Gearbox reduction of the bending module (1 to 5) |
| `torsionStageDist` | 50 | mm | Distance the stage must move to get the plate into the holder |
| `torsionTransportDist` | 50 | mm | Distance the plate must be fed through for torsion |
| - | 90 | deg | Angle the torsion module makes to twist the plate (hardcoded in `standardTorsion()`) |
| `torsionGearReduction` | 8 | - | Gearbox reduction of the torsion module (1 to 8) |
| `pitch` | 8 | mm | Distance the stage travels per revolution of the lead screw |

For now `bendTransportDist` = `torsionTransportDist`.

### Pins (config.h)
| Variable | Pin | Module (axis) | Description |
| -------- | --- | ------------- | ----------- |
| `transportStep` | 2 | Transport (X) | Step pin |
| `transportDir` | 5 | Transport (X) | Direction pin |
| `torsionStep` | 3 | Torsion (Y) | Step pin |
| `torsionDir` | 6 | Torsion (Y) | Direction pin |
| `bendingStep` | 4 | Bending (Z) | Step pin |
| `bendingDir` | 7 | Bending (Z) | Direction pin |
| `stageStep` | A4 | Stage (A) | Step pin |
| `stageDir` | A3 | Stage (A) | Direction pin |
| `enablePin` | 8 | All drivers | Enable pin shared by all drivers (LOW = armed, HIGH = disarmed) |
| `transport`, `torsion`, `bending`, `stage` |  |  | Arrays `{stepPin, dirPin}` per module, passed to `moveStepper()` |

### Values hardcoded in the code (these do not change)
| Value | Where | Description |
| ----- | ----- | ----------- |
| 1.8 deg | `n_steps()` in helpers.ino | Angle of one motor step (full-step mode, no microstepping) |
| 500 + 500 µs | `moveStepper()` in lib.ino | Duration of the HIGH and LOW part of one step pulse, so 1000 steps per second |
| 9600 | `setup()` in cnc_bend.ino | Baud rate of the Serial Monitor |
| 500 ms / 200 ms | `standardBend()` and `standardTorsion()` | Pauses between the steps of a sequence |

### State variables
| Name in notes | Variable in code | Description |
| ------------- | ---------------- | ----------- |
|  | `armed` | Whether the drivers are enabled (`true`) or disabled (`false`). Starts as `false`. |
| x |  | Movement of the stage from 0 (not tracked in the code yet) |
| α |  | Angle of the torsion module (not tracked in the code yet) |

## Functions
| Function | File | Inputs | Output | Description |
| -------- | ---- | ------ | ------ | ----------- |
| `setup()` | cnc_bend.ino |  |  | Runs once at power-up or reset. Starts the Serial Monitor, initializes the enable pin and all modules. The drivers start disarmed. |
| `loop()` | cnc_bend.ino |  |  | Runs continuously. Calls `handleSerialCommands()` to check for typed commands. |
| `main_function()` | main.ino |  |  | Runs the full automatic sequence (`start` command). Currently one `standardBend()`. |
| `initTransport()` | doorvoer.ino |  |  | Sets the step and direction pin of the transport module to OUTPUT. |
| `transportStrip(d)` | doorvoer.ino | `d`: distance (mm) (float), negative = backwards |  | Feeds the strip d mm through the machine. Converts the distance to a wheel angle and steps, then moves the stepper. Does nothing (and prints a message) when disarmed. |
| `initTorsion()` | torsie.ino |  |  | Sets the step and direction pin of the torsion module to OUTPUT. |
| `torsionPlate(torsionAngle)` | torsie.ino | `torsionAngle`: angle (deg) (float), negative = other direction |  | Twists the plate by the given angle, corrected for the gearbox. Does nothing (and prints a message) when disarmed. |
| `standardTorsion()` | torsie.ino |  |  | One full torsion: feed the plate, move the stage in, twist 90°, move the stage back, return the torsion module (-90°). |
| `initBending()` | buiging.ino |  |  | Sets the step and direction pin of the bending module to OUTPUT. |
| `bendPlate(bendAngle)` | buiging.ino | `bendAngle`: angle (deg) (float), negative = other direction |  | Bends the plate by the given angle, corrected for the gearbox. Does nothing (and prints a message) when disarmed. |
| `standardBend()` | buiging.ino |  |  | One full bend: feed the plate, move the stage in, bend 90°, move the stage back, return the bender (-90°). |
| `initStage()` | stage.ino |  |  | Sets the step and direction pin of the stage to OUTPUT. |
| `moveStage(stageDist)` | stage.ino | `stageDist`: distance (mm) (float), negative = backwards |  | Moves the stage the given distance using the screw pitch. Does nothing (and prints a message) when disarmed. |
| `angle(r, d)` | helpers.ino | `r`: radius (mm) (int), `d`: distance (mm) (float) | angle (deg) (float) | Angle a wheel with radius r must turn to move a distance d. |
| `n_steps(angle)` | helpers.ino | `angle`: angle (deg) (float) | steps (-) (int) | Number of steps needed for the given motor angle (1.8° per step, truncated). |
| `angleGearReduction(outAngle, reduction)` | helpers.ino | `outAngle`: angle after the gearbox (deg) (float), `reduction`: gearbox reduction 1 to x (-) (int) | angle before the gearbox (deg) (float) | Motor angle needed to get a given angle after the gearbox. |
| `d_circ(r)` | helpers.ino | `r`: radius (mm) (int) | circumference (mm) (float) | Circumference of a circle. |
| `getStatus()` | helpers.ino |  |  | Prints whether the drivers are armed or disarmed (`status` command). |
| `waitForCommand(word)` | helpers.ino | `word`: text to wait for (String) |  | Blocks until the given word is typed in the Serial Monitor. Currently unused. |
| `moveStepper(steps, stepper, dir)` | lib.ino | `steps`: number of steps (-) (int), `stepper`: array `{stepPin, dirPin}` (int[]), `dir`: true = forward, false = backward (bool) |  | Sets the direction and sends the step pulses. Does nothing (and prints a message) when disarmed. |
| `arm()` | lib.ino |  |  | Enables all drivers through the enable pin. |
| `disarm()` | lib.ino |  |  | Disables all drivers through the enable pin. Motors are free to turn by hand. |
| `checkArmed()` | lib.ino |  | true if armed (bool) | Prints "Not armed!" and returns false when the drivers are disarmed. Used at the start of the module functions. |
| `initArm()` | lib.ino |  |  | Sets up the enable pin and starts disarmed. Call first in `setup()`. |
| `handleSerialCommands()` | lib.ino |  |  | Reads a typed line from the Serial Monitor and runs the matching command. Call every loop. |
| `isNumber(s)` | lib.ino | `s`: text (String) | true if s is a number (bool) | Checks whether the text is a valid number (digits, optional leading `-`, optional `.`). |

## Operation conventions (Dutch)
- Elke bewerking begint op x = 0 en α = 0
- Na elke bewerking beweegt de stage naar x = 0 en draait de torsie module naar α = 0
- Voor elke bewerking wordt de plaat eerst doorgevoerd

## Code conventions
- Keep the code clean and separated. Different modules go in different files and are linked to the main file.
- Provide enough comments so others can understand what your code does without having to read the full code. 
- If functions have inputs list those inputs (function and type) just below the definiton of the function.
- Try to provide useful information by printing to the serial monitor. (use `Serial.Println()`)

## Notes 
### Pin layout of the CNC-shield
The CNC-shield has 4 different axes that we can control: X, Y, Z and A. Below is a short overview of the pin layout corresponding with each axis.
| Axis | Step pin | Dir pin |
| ---- | -------- | ------- |
| X    | 2        | 5       |
| Y    | 3        | 6       |
| Z    | 4        | 7       |
| A    | A4       | A3      |

All the pins should be set to OUTPUT using pinMode. The direction of which the plug with blue, green, red and black wire does not matter, as long as it is consistent for all the axes. 

### About the pins of the Stepper driver
The test code defines the following variables:
```c++
const int StepX = 2;
const int DirX = 5;
const int StepY = 3;
const int DirY = 6;
const int StepZ = 4;
const int DirZ = 7;
```

The difference between StepX and DirX is as follows (written by Claude):
- STEP says when to move. Every time the pin goes from LOW to HIGH, the motor takes one step. The motor doesn't know how far to go, so you send a pulse for every step you want. 200 pulses is typically one full revolution (for a 1.8° motor at full-step mode).
- DIR says which way to move. Set it HIGH and the motor turns one way, set it LOW and it turns the other way. The driver reads this pin when the step pulse arrives, so you set it before stepping.

So in short:
- Step is for how much steps.
- Dir is for which direction.

The direction can be set as follows:
```c++
digitalWrite(DirX, HIGH);
// or 
digitalWrite(DirX, LOW);
```

Also make sure to set the pinMode of the Dir and Step variables to OUTPUT, not INPUT.
### About the usage of the 4th axis (A)
The cnc-shield uses the analog pins to allow power users to use it as a separate 4th axis in their code. We can control them by declaring the variables as follows:
```c++
const int StepA = A4;
const int DirA  = A3;

void setup() {
	  pinMode(StepA, OUTPUT);
	  pinMode(DirA, OUTPUT);
}
```