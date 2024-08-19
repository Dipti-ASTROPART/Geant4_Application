//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#ifndef     g4EventAction_HH
#define     g4EventAction_HH

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "G4UserEventAction.hh"
#include    "G4Event.hh"    
#include    "g4root_defs.hh"
#include    "g4RunAction.hh"
#include    "G4SystemOfUnits.hh"
#include    "G4GeneralParticleSource.hh"

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
class MyEventAction : public G4UserEventAction
{
    public:
                                    MyEventAction(MyRunAction *);
                                    ~MyEventAction();
        virtual void                BeginOfEventAction(const G4Event *);
        virtual void                EndOfEventAction(const G4Event *);
        void                        AddEdep(G4double edep) {fEdep += edep;}
        void                        AddOpticalPhotonEnergy(G4double edep){fOptPhotonE += edep; nOpticalPhotons ++;}
        void                        CountPhotonsInWLS(){nWLSPhotons++;}

    private:
        G4double                    fEdep;
        G4double                    fOptPhotonE;
        G4int                       nOpticalPhotons;
        G4int                       nWLSPhotons;
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#endif
