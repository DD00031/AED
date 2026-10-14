#pragma once

// --- Config --- 

// --- Pin Declarations ---
// Doorvoer = X
// Torsie = Y
// Buiging = Z
// Stage = A
// (CHANGE THE PINS TO MATCH THE CORRECT ONES FROM THE README!)
const int transportStep = 2;
const int transportDir = 5;
const int torsionStep = 3;
const int torsionDir = 6;
const int bendingStep = 4;
const int bendingDir = 7;
const int stageStep = A4;
const int stageDir = A3;

const int transport[] = {transportStep, transportDir};
const int torsion[] = {torsionStep, torsionDir};
const int bending[] = {bendingStep, bendingDir};
const int stage[] = {stageStep, stageDir};

// --- Dimension Declarations ---
// Transport
const int rWheel = 43; //mm
// Bending
const int bendingGearReduction = 25;
const int bendTransportDist = 50; //mm
const int bendStageDist = 50; //mm
// Torsion
const int torsionGearReduction = 25;
const int torsionTransportDist = 50; //mm
const int torsionStageDist = 50; //mm
