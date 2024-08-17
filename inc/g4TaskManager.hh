//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef     g4TaskManager_HH
#define     g4TaskManager_HH

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include        <iostream>
#include        "g4ControlFileHandler.hh"
#include        "G4RunManager.hh"
#include        "G4MTRunManager.hh"
#include        "G4UIExecutive.hh"
#include        "G4VisManager.hh"
#include        "G4UImanager.hh"
#include        "G4VisExecutive.hh"
#include        "G4VModularPhysicsList.hh"
#include        "QBBC.hh"
#include        "FTFP_BERT.hh"
#include        "G4OpticalParameters.hh"
#include        "G4EmStandardPhysics_option4.hh"
#include        "G4OpticalPhysics.hh"
#include        "G4Scintillation.hh"
#include        "G4OpticalPhoton.hh"

#include        "g4DetectorConstruction.hh"
#include        "g4PhysicsList.hh"
#include        "g4ActionInitialize.hh"
#include        "g4Constants.hh"
#include        "g4Structures.hh"
#include        "g4ControlFileHandler.hh"
#include        "g4CPP_ROOT_Headers.hh"


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class       MyTaskManager
{
    public:
                                    MyTaskManager(int argc, char *argv[]);
                                    ~MyTaskManager();
        void                        AssignTask();
    private:
        int                         p_argc;
        char**                      p_argv;
        ControlFile                 Control;
        ControlFileHandler          *cCF;
        void                        CreateRunMacro(G4String filename);
        bool                        ValidateDirectory(G4String dir_name);
        bool                        ValidateFile(G4String filename);
};

#endif
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
