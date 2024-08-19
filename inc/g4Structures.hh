#ifndef     g4Structures_HH
#define     g4Structures_HH

#include    "G4LogicalVolume.hh"

typedef struct  ControlFile
{
    char            Path[20][300],
                    Output[300],
                    MacroPath[300],
                    Task[50],
                    ParticleName[50];
    bool            GraphicalMode;
    int             NPath,
                    NBeam;
    double          PrimaryEnergy1,
                    PrimaryEnergy2;
    
} ControlFile;

typedef struct  ScoringVolumes_Struct
{
    G4LogicalVolume* primaryDetector;
    G4LogicalVolume* fiberCore;
    G4LogicalVolume* fiberCladIn;
    G4LogicalVolume* fiberCladOut;

}   ScoringVolumes_Struct;

#endif
