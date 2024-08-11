#ifndef     g4Structures_HH
#define     g4Structures_HH

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

#endif
