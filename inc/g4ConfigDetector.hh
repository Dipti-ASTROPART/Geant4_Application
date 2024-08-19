#ifndef G4CONFIGDETECTOR_HH
#define G4CONFIGDETECTOR_HH

#include "G4Types.hh"

// Configure box shaped world volume
const G4double  fWorldXY = 30.0 *cm;
const G4double  fWorldZ  = 60.0 *cm;

// Configure cylinder shaped world volume
const G4double fCylWorldRadius = 20.0 *cm;
const G4double fCylWorldHeight = 60.0 *cm;

// Configure Box shaped detector
const   G4double fBoxDetX = 10.0 *cm;
const   G4double fBoxDetY = 10.0 *cm;
const   G4double fBoxDetZ = 10.0 *cm;

// Define parameters for a cylindrical scintillator detector
const G4bool    checkOverlaps       = true;
const G4int     fNCylinders         = 5;
const G4double  fCylRadius          = 2.5  * cm;
const G4double  fCylHeight          = 3.0  * cm;
const G4double  fCylStartAngle      = 0.0  * deg;
const G4double  fCylStopAngle       = 360.0* deg;

// Configure WLS fibers model Y-11
const G4double  fWLSFiberLength         = 25.0 * cm;
const G4double  fWLSFiberDiameter       = 0.1 * cm;
const G4double  fWLSFiberRadius         = fWLSFiberDiameter/2.0;
const G4double  fWLSFiberCoreRadius     = 0.96 * fWLSFiberRadius;
const G4double  fWLSFiberCladRadiusIn   = fWLSFiberCoreRadius + 0.02 * fWLSFiberRadius;
const G4double  fWLSFiberCladRadiusOut  = fWLSFiberCladRadiusIn + 0.02 * fWLSFiberRadius;

// configure tyvek
const G4double  fTyvekThickness     = 2.0  * mm;
const G4double  fAir_Tyvek_Gap      = 0.1  * mm;

// Configure the box shaped environment
const   G4double fBoxEnvX = 1.50 * fBoxEnvX;
const   G4double fBoxEnvY = 1.50 * fBoxEnvY;
const   G4double fBoxEnvZ = 1.50 * fBoxEnvZ;

// Configure cylinder shaped environment volume
const G4double  fCylEnvRadius = 2 * fCylRadius;
const G4double  fCylEnvHeight = fWorldZ;

#endif
