//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/// 
/// All the parameters at each step are calculated here.
///
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "g4SteppingAction.hh"

//////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////
MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{

    fEventAction = eventAction;             ///< Object to the event action

}   //  ::MySteppingAction()


//////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////
MySteppingAction::~MySteppingAction()
{

}   //  ::MySteppingAction()


//////////////////////////////////////////////////////////////////////////////////
/// User defined stepping action
//////////////////////////////////////////////////////////////////////////////////
void    MySteppingAction::UserSteppingAction(const G4Step *step)
{
    // Access the actual logical volume of the detector
    G4LogicalVolume *volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // Extract the scoring volume under investigation through the G4RunManager
    const MyDetectorConstruction *detConstruction = static_cast <const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4LogicalVolume *fScoringVolume = detConstruction->GetScoringVolume();

    // Compare if the scoring volume and detector logical volumes are same
    if(volume != fScoringVolume)
        return;

    // Extract the energy deposited at each step 
    G4double edep = step->GetTotalEnergyDeposit();

    // Extract each track
    G4Track *track = step->GetTrack();

    // Get particle definition
    G4ParticleDefinition *partDef = track->GetDefinition();


    /*  
        auto proc_man = track->GetDynamicParticle()->GetParticleDefinition()->GetProcessManager();
        G4ProcessVector *proc_vec = proc_man->GetPostStepProcessVector(typeDoIt);

        G4int    n_proc = proc_vec->entries();
        for(G4int i  = 0; i < n_proc; i++)
        {
        G4cout << (*proc_vec)[i]->GetProcessName() << G4endl;
        if((*proc_vec)[i]->GetProcessName() == "Scintillation")
        {
        auto scint = (G4Scintillation*) (*proc_vec)[i];
        G4cout <<"Scint. photons: "<< scint->GetNumPhotons() <<G4endl;
        }
        }
        */

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    if(partDef->GetParticleName() == "opticalphoton")
    {
        fEventAction->AddOpticalPhotonEnergy(track->GetKineticEnergy());
        man->FillH1(0, track->GetKineticEnergy()/CLHEP::eV);
        man->FillH1(1, 1240.0/(track->GetKineticEnergy()/CLHEP::eV));

        //printf("Secondary particle : %s    %.14f (eV) \n", partDef->GetParticleName().c_str(), track->GetKineticEnergy()/CLHEP::eV);
        //printf("Energy in step : %f\n", edep/CLHEP::MeV);

    }
    else
    {
        //printf("Secondary particle : %s    %.14f (MeV) \n", partDef->GetParticleName().c_str(), track->GetKineticEnergy()/CLHEP::MeV);
        //printf("Energy in step : %.15f\n", edep);
    }

    // Accumulate the energy
    fEventAction->AddEdep(edep);
}   //  ::UserSteppingAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
