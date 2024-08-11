#ifndef     g4SteppingAction_HH
#define     g4SteppingAction_HH

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "G4UserSteppingAction.hh"
#include    "G4Step.hh"
#include    "G4RunManager.hh"
#include    "G4ProcessManager.hh"
#include    "G4ProcessVector.hh"
#include    "G4Scintillation.hh"

#include    "g4DetectorConstruction.hh"
#include    "g4EventAction.hh"

class   MySteppingAction : public G4UserSteppingAction
{
    public:
                                    MySteppingAction(MyEventAction *eventAction);
                                    ~MySteppingAction();
        virtual void                UserSteppingAction(const G4Step *);

    private:
        MyEventAction               *fEventAction;
};
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#endif
