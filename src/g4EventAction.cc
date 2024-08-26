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
    : G4UserEventAction(), fPhotonHitsSD1(0), fPhotonHitsSD2(0), fPhotonHitsSCSD(0)
{

    fEdep = 0;              ///< Initialize the energy deposit values
    fOptPhotonE = 0;
    nOpticalPhotons = 0;
    nWLSPhotons = 0;

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
    nWLSPhotons = 0;

    fPhotonHitsSD1  = 0;
    fPhotonHitsSD2  = 0;
    fPhotonHitsSCSD = 0;
}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
/// End the event action
//////////////////////////////////////////////////////////////////////////////////
void    MyEventAction::EndOfEventAction(const G4Event *event)
{

    G4PrimaryVertex     *primaryVertex   = event->GetPrimaryVertex();
    G4PrimaryParticle   *primaryParticle = primaryVertex->GetPrimary();
    G4SDManager         *sdManager       = G4SDManager::GetSDMpointer();

    // Invoke the sensitive detectors
    MySensitiveDetector* sd1 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("mySD1"));
    MySensitiveDetector* sd2 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("mySD2"));
    MySensitiveDetector* sd0 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("mySCSD"));

    // Get photon hits
    G4int photonsSCSD= sd0->GetPhotonHits();
    G4int photonsSD1 = sd1->GetPhotonHits();
    G4int photonsSD2 = sd2->GetPhotonHits();

    G4int totalphotons = photonsSD1 + photonsSD2;

    // Invoke the G4AnalysisManager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Fill the parameter
    man->FillNtupleDColumn(0, primaryParticle->GetTotalEnergy()/MeV);
    man->FillNtupleDColumn(1, primaryParticle->GetMass()/MeV);
    man->FillNtupleDColumn(2, fEdep);

    man->FillH1(3, photonsSD1);
    man->FillH1(4, photonsSD2);
    man->FillH1(5, totalphotons);
    man->FillH1(6, fEdep/keV);
    man->FillH1(7, photonsSCSD);

    //  Write the branch
    man->AddNtupleRow();

    G4cout << COLOR_GREEN << "\n............................................................................" << COLOR_RESET <<G4endl;
    G4cout << COLOR_CYAN << "Energy Deposition (keV) .......................: " << COLOR_YELLOW << fEdep/keV << G4endl;
    G4cout << COLOR_CYAN << "Total number of optical photons in Detector ...: " << COLOR_YELLOW << nOpticalPhotons << G4endl;
    G4cout << COLOR_CYAN << "Total number of optical photons in Fiber ......: " << COLOR_YELLOW << nWLSPhotons << G4endl;
    G4cout << COLOR_CYAN << "Total Energy for optical photons (keV) ........: " << COLOR_YELLOW << fOptPhotonE/keV << G4endl;
    G4cout << COLOR_CYAN << "Total photons collected by WLS Fiber (end 1) ..: " << COLOR_YELLOW << photonsSD1<< G4endl;
    G4cout << COLOR_CYAN << "Total photons collected by WLS Fiber (end 2) ..: " << COLOR_YELLOW << photonsSD2<< G4endl;
    G4cout << COLOR_CYAN << "Total number of photons collected at the SCSD .: " << COLOR_YELLOW << photonsSCSD << G4endl;
    G4cout << COLOR_GREEN << "............................................................................\n" << COLOR_RESET <<G4endl;
    G4cout<<G4endl;

}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
