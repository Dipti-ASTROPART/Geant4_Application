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

    nParentPartEl   = 0;
    nParentPartGa   = 0;
    gammaDetected = FALSE;
    electronDetected = FALSE;
}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
/// End the event action
//////////////////////////////////////////////////////////////////////////////////
void    MyEventAction::EndOfEventAction(const G4Event *event)
{
    // Get photon hits
    G4int photonsSCSD= 0;
    G4int photonsSiPM1x1 = 0;
    G4int photonsSiPM3x3 = 0;
    G4int inpartID   = -1;

    const   MyRunAction* runAction = static_cast<const MyRunAction*>(G4RunManager::GetRunManager()->GetUserRunAction());
    static  G4int eventCount = 0;
    eventCount++;

    G4PrimaryVertex     *primaryVertex   = event->GetPrimaryVertex();
    G4PrimaryParticle   *primaryParticle = primaryVertex->GetPrimary();
    G4SDManager         *sdManager       = G4SDManager::GetSDMpointer();

    if(fSETSENSITIVEDETECTOR)
    {
        // Invoke the sensitive detectors
        MySensitiveDetector* sd3x3 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("SiPM3SD"));
        MySensitiveDetector* sd1x1 = static_cast<MySensitiveDetector*>(sdManager->FindSensitiveDetector("SiPM1SD"));

        photonsSiPM1x1= sd1x1->GetPhotonHits();
        photonsSiPM3x3= sd3x3->GetPhotonHits();
    }
    G4int totalphotons = photonsSiPM1x1 + photonsSiPM3x3;

    // Invoke the G4AnalysisManager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Fill the parameter
    man->FillNtupleDColumn(0, primaryParticle->GetTotalEnergy()/MeV);
    man->FillNtupleDColumn(1, primaryParticle->GetMass()/MeV);
    man->FillNtupleDColumn(2, fEdep);
    man->FillNtupleDColumn(3, photonsSiPM1x1);
    man->FillNtupleDColumn(4, photonsSiPM3x3);
 
    if(gammaDetected == TRUE && electronDetected == TRUE) inpartID = 2;         ///< Both e and gamma detected
    else if(gammaDetected == TRUE  && electronDetected == FALSE) inpartID = 0;  ///< Gamma detected
    else if(gammaDetected == FALSE && electronDetected == TRUE)  inpartID = 1;  ///< Electron detected
    else inpartID = -1;


    man->FillH1(0, fEdep);
    man->FillH1(3, photonsSiPM1x1);
    man->FillH1(4, photonsSiPM3x3);
    man->FillH1(5, totalphotons);
    man->FillH1(6, fEdep/keV);
    if(photonsSCSD!=0)
        man->FillH1(7, photonsSCSD);

    //  Write the branch
    //if(inpartID > -1)
    man->AddNtupleRow();

    if(runAction->IsVisualModeEnabled() == true)
    {
        G4cout << COLOR_GREEN << "\n............................................................................" << COLOR_RESET <<G4endl;
        G4cout << COLOR_CYAN <<"Primary particle in the detector...............: " << COLOR_YELLOW << "Gamma : " << gammaDetected << "   e- : " << electronDetected <<"   " << inpartID << G4endl;
        G4cout << COLOR_CYAN << "Energy Deposition .............................: " << COLOR_YELLOW << fEdep/keV <<" keV" << G4endl;
        G4cout << COLOR_CYAN << "Total number of optical photons in Detector ...: " << COLOR_YELLOW << nOpticalPhotons << G4endl;
        //G4cout << COLOR_CYAN << "Total number of optical photons in Fiber ......: " << COLOR_YELLOW << nWLSPhotons << G4endl;
        G4cout << COLOR_CYAN << "Total Energy for optical photons ..............: " << COLOR_YELLOW << fOptPhotonE/keV <<" keV" << G4endl;
        G4cout << COLOR_CYAN << "Total photons collected by 1x1 SiPM ..: " << COLOR_YELLOW << photonsSiPM1x1<< G4endl;
        G4cout << COLOR_CYAN << "Total photons collected by 3x3 SiPM ..: " << COLOR_YELLOW << photonsSiPM3x3<< G4endl;
        G4cout << COLOR_CYAN << "Total number of Collected photons by the PMT   : " << COLOR_YELLOW << photonsSCSD << G4endl;
        G4cout << COLOR_GREEN << "............................................................................" << COLOR_RESET <<G4endl;
        G4cout<<G4endl;

    }
    else
        PrintProgressBar(eventCount, runAction->GetTotalSimulatedEvents());

}   //  ::BeginOfEventAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void    MyEventAction::PrintProgressBar(G4int currentEvent, G4int totalEvents) {
    G4int       barWidth = 70; // Width of the progress bar
    G4double    progress = (G4double)currentEvent / totalEvents;

    G4cout << COLOR_MAGENTA << "[";
    int pos = barWidth * progress;
    for (G4int i = 0; i < barWidth; ++i) {
        if (i < pos) G4cout<< COLOR_GREEN << "=";
        else if (i == pos) G4cout << ">";
        else G4cout << " ";
    }
    G4cout << COLOR_MAGENTA <<"]" << int(progress * 100.0) << " %\r"<<COLOR_RESET;
    G4cout.flush();

    if(currentEvent == totalEvents)
        G4cout <<COLOR_RESET <<"\n\n";
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
