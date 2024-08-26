//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
///
/// ->  Controls the run of the Geant4 simulation
/// ->  Create ROOT files to store the output
///
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
#include    "g4RunAction.hh"
#include    <filesystem>

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
    sprintf(pFileName, "%s/G4OUT_%s_%03d.root", Control.Output, Control.ParticleName, runID);
    man->OpenFile((G4String) pFileName);

    // Create TUPLE (equivalent to branch)
    man->CreateNtuple("Scoring", "Scoring");
    man->CreateNtupleDColumn("PrimaryE");
    man->CreateNtupleDColumn("ParticleMass");
    man->CreateNtupleDColumn("fEdep");

    G4int id0 = man->CreateH1("hPhotonEn", "Optical photon energy", 150, 0, 15);
    G4int id1 = man->CreateH1("hPhotonWL", "Optical photon wavelength", 500, 200, 700);
    G4int id2 = man->CreateH1("hPhotonWLinWLS", "Optical photon wavelength in WLS", 500, 200, 700);
    G4int id3 = man->CreateH1("hNPhotonsSD1", "#Photons at WLS fiber end-1;#Photons; ", 1000, 0, 1000);
    G4int id4 = man->CreateH1("hNPhotonsSD2", "#Photons at WLS fiber end-2;#Photons; ", 1000, 0, 1000);
    G4int id5 = man->CreateH1("hTotalPhotons", "#Photons at WLS fiber;#Photons; ", 1000, 0, 1000);
    G4int id6 = man->CreateH1("hEnergyLoss", "Energy loss in detector; Energy(keV);", 1000, 0, 1000);
    G4int id7 = man->CreateH1("hPMTPhotons", "#Photons at WLS fiber;#Photons; ", 1000, 0, 1000);

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

    //   Delete the file if its in graphical mode
    if(Control.GraphicalMode)
    {
        if (std::filesystem::exists(pFileName)) {
            if (std::filesystem::remove(pFileName)) 
                G4cout << "... delete file: " << pFileName<<" - done" << G4endl;
            else 
                G4cerr << "Failed to delete " << pFileName << G4endl;

        }   // if output file exists
    } // if graphical mode is enables

}   //  ::BeginRunAction()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

