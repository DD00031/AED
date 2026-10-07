# CNC Bend Machine Code
Final code for arduino.

## List of files + Description
- config.h
Contains all global constant and state variables. Also used for configuring order of operations.
- cnc_bend.ino
Main code file, used for bringing the separate module code files together.

## Variables (Dutch)
(Keep in mind that these are prone to changing)
### Constant Variables
b1: Afstand die de stage moet bewegen om de plaat in de buigmodule te krijgen
b2: Afstand die de plaat doorgevoerd moet worden voor een buiging
b3: Hoek die de wipe bender maakt om de plaat te buigen.

t1: Afstand die de stage moet bewegen om de plaat in de houder te krijgen
t2: Afstand die de plaat doorgevoerd moet worden voor tordering
t3: Hoek die torsiemodule maakt om de plaat te torderen

b2 = t2 (vooralsnog)

### State Variabels:
x: Beweging stage vanaf 0
α: Hoek torsiemodule

## Operation conventions (Dutch)
- Elke bewerking begint op x = 0 en α = 0
- Na elke bewerking beweegt de stage naar x = 0 en draait de torsie module naar α = 0
- Voor elke bewerking wordt de plaat eerst doorgevoerd

## Code conventions
- Keep the code clean and separated. Diffent modules go in different files and are linked to the main file.
- Provide enough comments so others can understand what your code does without having to read the full code. 
- If functions have inputs list those inputs (function and type) just below the definiton of the function.

## Notes 
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
