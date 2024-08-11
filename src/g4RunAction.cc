//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///
/// ->  Controls the run of the Geant4 simulation
/// ->  Create ROOT files to store the output
///
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "g4RunAction.hh"

//////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////
MyRunAction::MyRunAction(ControlFile cnt)
{

    Control = cnt;

}   //  ::MyRunAction


//////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////
MyRunAction::~MyRunAction()
{

    //  DO NOTHING

}   //  ::~MyRunAction


//////////////////////////////////////////////////////////////////////////////////
/// Begin of run action (Be careful with the nomenclature)
//////////////////////////////////////////////////////////////////////////////////
void    MyRunAction::BeginOfRunAction(const G4Run *run)
{
    // Invoke G4AnalysisManager
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->SetVerboseLevel(1);

    // Extract the ID of the RUN
    G4int runID = run->GetRunID();

    //  Create a ROOT file
    sprintf(filename, "%s/G4OUT_%s_%03d.root", Control.Output, Control.ParticleName, runID);
    man->OpenFile((G4String) filename);

    // Create TUPLE (equivalent to branch)
    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("PrimaryE");
    man->CreateNtupleDColumn("ParticleMass");
    man->CreateNtupleDColumn("fEdep");

    G4int id1 = man->CreateH1("hPhotonEn", "Optical photon energy", 150, 0, 15);
    G4int id2 = man->CreateH1("hPhotonWL", "Optical photon wavelength", 500, 200, 700);
    // Finish the TUPLE (branch)
    man->FinishNtuple();

}   //  ::BeginRunAction()


//////////////////////////////////////////////////////////////////////////////////
/// End of run action (Be careful with the nomenclature)
//////////////////////////////////////////////////////////////////////////////////
void    MyRunAction::EndOfRunAction(const G4Run *)
{
    // Instantiate the G4AnalysisManager
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Write and close the ROOT files
    man->Write();
    man->CloseFile();

}   //  ::BeginRunAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

