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
