//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#include    "g4SensitiveDetector.hh"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MySensitiveDetector::MySensitiveDetector(const G4String &volName) 
    : G4VSensitiveDetector(volName), fPhotonHits(0)
{

    // Constructor

}   //  ::MySensitiveDetector()


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
MySensitiveDetector::~MySensitiveDetector()
{

    // Destructor

}   //  ::MySensitiveDetector()



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void MySensitiveDetector::Initialize(G4HCofThisEvent* hce) 
{
    fPhotonHits = 0;
}   //::Initialize()


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
G4bool MySensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory* history) 
{
    G4Track* track = step->GetTrack();

    // Only process optical photons
    //G4cout << "Track definition   : " <<track->GetDefinition() << G4endl;
    if (track->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition()) {
        return false;
    }
    fPhotonHits++;

    // Get the energy of the photon
    G4double photonEnergy = track->GetTotalEnergy();

    // Get the position of the hit
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4ThreeVector position = preStepPoint->GetPosition();

    // Get the volume where the hit occurred
    G4TouchableHandle touchable = preStepPoint->GetTouchableHandle();
    G4VPhysicalVolume* volume = touchable->GetVolume();

    // Log or store the hit information
    //G4cout << "Photon detected at : " << volume->GetName() << ", Energy: " << photonEnergy / eV << " eV, Position: " << position<<"  ["<<fPhotonHits <<"]" << G4endl;

    // Here, you would add the code to store this data in a hit collection or process it further
    // ...

    return true;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* hce) 
{
    // Perform any end-of-event actions here
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
