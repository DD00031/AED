#pragma once

#include <SoftwareSerial.h>

// --- Config --- 

int speed = 800;

// --- Pin Declarations ---
// Doorvoer = X
// Torsie = Y
// Buiging = Z
// Stage = second Arduino (see README, "Stage Arduino")
const int transportStep = 2;
const int transportDir = 5;
const int torsionStep = 3;
const int torsionDir = 6;
const int bendingStep = 4;
const int bendingDir = 7;

const int enablePin = 8;

const int transport[] = {transportStep, transportDir};
const int torsion[] = {torsionStep, torsionDir};
const int bending[] = {bendingStep, bendingDir};

// --- Link to the stage Arduino (SoftwareSerial) ---
// Cross the wires: main TX -> stage RX and main RX <- stage TX. Also connect GND to GND.
const int stageLinkRx = 9;    // main RX <- TX of the stage Arduino
const int stageLinkTx = 10;   // main TX -> RX of the stage Arduino
const long stageLinkBaud = 9600;
const unsigned long stageReplyTimeout = 2000; // ms, wait for a reply to arm, disarm and ping
const unsigned long stageMoveMargin = 3000;   // ms, extra time on top of the move time (1 ms per step)
SoftwareSerial stageLink(stageLinkRx, stageLinkTx);

// --- Dimension Declarations ---
// Transport
const int rWheel = 19; //mm
// Bending
const int bendingGearReduction = 57;
const int bendTransportDist = 50; //mm
const int bendStageDist = 50; //mm
// Torsion
const int torsionGearReduction = 25;
const int torsionTransportDist = 65; //mm
const int torsionStageDist = 40; //mm
// Stage
const int pitch = 8; //mm

// Other
bool armed = false;
