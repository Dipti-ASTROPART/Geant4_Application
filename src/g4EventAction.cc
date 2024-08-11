//////////////////////////////////////////////////////////////////////////////////
///
/// Event action class where each event gets analyzed and the resultant 
/// outputs are store in the tree
///
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "g4EventAction.hh"

//////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////
MyEventAction::MyEventAction(MyRunAction *run)
{

    fEdep = 0;              ///< Initialize the energy deposit values
    fOptPhotonE = 0;
    nOpticalPhotons = 0;
}   //  ::MyEventAction


//////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////
MyEventAction::~MyEventAction()
{

}   //  ::MyEventAction
    

//////////////////////////////////////////////////////////////////////////////////
/// Begin the event action process
//////////////////////////////////////////////////////////////////////////////////
void    MyEventAction::BeginOfEventAction(const G4Event *)
{

    fEdep = 0;              ///< Reset the values
    fOptPhotonE = 0;
    nOpticalPhotons = 0;

}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
/// End the event action
//////////////////////////////////////////////////////////////////////////////////
void    MyEventAction::EndOfEventAction(const G4Event *event)
{

    G4PrimaryVertex     *primaryVertex = event->GetPrimaryVertex();
    G4PrimaryParticle   *primaryParticle = primaryVertex->GetPrimary();
    //
    // Invoke the G4AnalysisManager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    //G4cout << "Total Energy : " << primaryEnergy/GeV << G4endl;
    G4cout << "Total Momentum (MeV/C) : " << primaryParticle->GetTotalMomentum()/MeV << G4endl;
    G4cout << "Total K.E (MeV) : " << primaryParticle->GetKineticEnergy()/MeV << G4endl;
    G4cout << "Particle Mass (MeV/C^2): " << primaryParticle->GetMass()/MeV << G4endl;
    G4cout << "Energy Deposition (MeV) : " << fEdep/MeV << G4endl;
    G4cout << "Total number of optical photons : " << nOpticalPhotons << G4endl;
    G4cout << "Total E for optical photons (MeV) : " << fOptPhotonE/MeV <<"  |  " << fOptPhotonE/eV << "eV" << G4endl;

    // Fill the parameter
    man->FillNtupleDColumn(0, primaryParticle->GetTotalEnergy()/GeV);
    man->FillNtupleDColumn(1, primaryParticle->GetMass()/GeV);
    man->FillNtupleDColumn(2, fEdep);

    //  Write the branch
    man->AddNtupleRow();
    
}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
