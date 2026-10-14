# CNC Bend Machine Code
Final code for arduino.

## List of files + Description
| File | Module / axis | Description |
| ---- | ------------- | ----------- |
| `cnc_bend.ino` | Main | Main code file, used for bringing the separate module code files together. Contains `setup()` and `loop()`. |
| `config.h` | Config | Contains all global constant and state variables. Also used for configuring order of operations. |
| `main.ino` | Main sequence | Contains `main_function()`, the full automatic sequence that runs when you type `start`. |
| `doorvoer.ino` | Transport (X) | `initTransport()` sets up the pins. `transportStrip(d)` feeds the strip d mm through the machine using the wheel radius (negative = backwards). |
| `torsie.ino` | Torsion (Y) | `initTorsion()` sets up the pins. `torsionPlate(angle)` twists the plate by an angle in degrees, corrected for the gearbox (negative = other direction). |
| `buiging.ino` | Bending (Z) | `initBending()` sets up the pins. `bendPlate(angle)` bends the plate by an angle in degrees, corrected for the gearbox (negative = other direction). `standardBend()` does one full bend: feed the plate, move the stage in, bend 90°, move the stage back and return the bender. |
| `stage.ino` | Stage (A) | `initStage()` sets up the pins. `moveStage(dist)` moves the stage dist mm using the screw pitch (negative = backwards). |
| `helpers.ino` | Calculations | `angle(r, d)` gives the angle needed to move a distance over a wheel. `n_steps(angle)` gives the steps needed for an angle. `angleGearReduction(angle, reduction)` gives the motor angle needed for an angle after the gearbox. `d_circ(r)` gives the circumference. Also contains `waitForCommand()`, which is currently unused. |
| `lib.ino` | Shared functions | `moveStepper()` sends the step pulses (only works when armed). `arm()`, `disarm()` and `initArm()` enable or disable all drivers through the enable pin. `handleSerialCommands()` and `isNumber()` handle the serial monitor control. |

## Usage
Upload the sketch, open the Serial Monitor (baud rate 9600, line ending "Newline" or "Both NL & CR") and type a command followed by Enter. Commands are not case sensitive. Numbers can be negative to move in the other direction.

The drivers start **disarmed**, so type `arm` before anything else. After a reset or re-upload, the board is disarmed again.

| Command | Description |
| ------- | ----------- |
| `arm` | Enable the drivers (motors hold position and can move) |
| `disarm` | Disable the drivers (motors are free to turn by hand and no longer hold position) |
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
| - | 90 | deg | Angle the wipe bender makes to bend the plate (currently hardcoded in `standardBend()`) |
| `bendingGearReduction` | 5 | - | Gearbox reduction of the bending module (1 to 5) |
| `torsionStageDist` | 50 | mm | Distance the stage must move to get the plate into the holder |
| `torsionTransportDist` | 50 | mm | Distance the plate must be fed through for torsion |
| - |  | deg | Angle the torsion module makes to twist the plate (not in the code yet) |
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

### State variables
| Name in notes | Variable in code | Description |
| ------------- | ---------------- | ----------- |
|  | `armed` | Whether the drivers are enabled (`true`) or disabled (`false`). Starts as `false`. |
| x |  | Movement of the stage from 0 (not tracked in the code yet) |
| α |  | Angle of the torsion module (not tracked in the code yet) |

## Operation conventions (Dutch)
- Elke bewerking begint op x = 0 en α = 0
- Na elke bewerking beweegt de stage naar x = 0 en draait de torsie module naar α = 0
- Voor elke bewerking wordt de plaat eerst doorgevoerd

## Code conventions
- Keep the code clean and separated. Diffent modules go in different files and are linked to the main file.
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
