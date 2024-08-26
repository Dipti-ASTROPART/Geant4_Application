//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef     G4SENSITIVEDETECTOR_HH
#define     G4SENSITIVEDETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4StepPoint.hh"
#include "G4TouchableHandle.hh"
#include "G4VPhysicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class   MySensitiveDetector : public G4VSensitiveDetector
{
    public:
        MySensitiveDetector(const G4String& name);
        virtual ~MySensitiveDetector();

        virtual void        Initialize(G4HCofThisEvent*) override;
        virtual G4bool      ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) override;
        virtual void        EndOfEvent(G4HCofThisEvent*) override;

        G4int               GetPhotonHits() const {return fPhotonHits;}

    private:
        G4int fPhotonHits;  // Counter for photons
};

#endif
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
