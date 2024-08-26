//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    "g4PrimaryGenerator.hh"


//////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////////////
MyPrimaryGenerator::MyPrimaryGenerator(ControlFile cnt)
{

    //  Address the Contro file structures
    Control = cnt;

    fParticleGun = new G4ParticleGun(1);

}   //  ::MyPrimaryGenerator()


//////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////////////
MyPrimaryGenerator::~MyPrimaryGenerator()
{

    delete fParticleGun;

}   //  ::~MyPrimaryGenerator()


//////////////////////////////////////////////////////////////////////////////////////////
/// Define and generate primaries here
//////////////////////////////////////////////////////////////////////////////////////////
void    MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    // Access the Geant4 particle table 
    //G4ParticleTable *partTable = G4ParticleTable::GetParticleTable();

    G4double partEne  = GenerateRandomEnergy();
    
    // Define position and momentum of the primary particle
    G4ThreeVector pos(fParticlePosX, fParticlePosY, fParticlePosZ);
    G4ThreeVector mom(0., 0., 1.);

    fParticleDef = GetUserParticle(Control.ParticleName);

    // Initiate the particle gun
    fParticleGun->SetParticlePosition(pos);
    fParticleGun->SetParticleMomentumDirection(mom);
    //fParticleGun->SetParticleMomentum(partEne * MeV);
    fParticleGun->SetParticleEnergy(partEne * MeV);
    fParticleGun->SetParticleDefinition(fParticleDef);

    // Shoot the particle from the assigned position and designated momentum direction
    fParticleGun->GeneratePrimaryVertex(anEvent);

}   //  ::GeneratePrimaries()


//////////////////////////////////////////////////////////////////////////////////////////
/// Get random energy following a power law distribution 
//////////////////////////////////////////////////////////////////////////////////////////
G4double    MyPrimaryGenerator::GenerateRandomEnergy()
{
    // Initialize the TRandom3 class from the ROOT
    TRandom3 randGen;

    // Set the seed for the random number
    randGen.SetSeed(0);

    // Generate Uniform random number
    G4double    U = randGen.Uniform(0.0, 1.0);
    G4double    exponent = 1.0 / (1.0 - E_SLOPE);
    G4double    randEne = std::pow((1 - U) * (std::pow(Control.PrimaryEnergy2, 1 - E_SLOPE) - std::pow(Control.PrimaryEnergy1, 1 - E_SLOPE)) + std::pow(Control.PrimaryEnergy1, 1 - E_SLOPE), exponent);

    // Return the energy
    return randEne;

}   //  ::GetRandomEnergy()


//////////////////////////////////////////////////////////////////////////////////////////
/// Get primary particle definition
//////////////////////////////////////////////////////////////////////////////////////////
G4ParticleDefinition    *MyPrimaryGenerator::GetUserParticle(G4String particleName)
{
    if (particleName == "electron" || particleName == "e-") {
        return G4Electron::ElectronDefinition();
    }
    if (particleName == "positron" || particleName == "e+") {
        return G4Positron::PositronDefinition();
    }
    if (particleName == "muon+" || particleName == "mu+") {
        return G4MuonPlus::MuonPlusDefinition();
    }
    if (particleName == "muon-" || particleName == "mu-") {
        return G4MuonMinus::MuonMinusDefinition();
    }
    if (particleName == "gamma" || particleName == "photon") {
        return G4Gamma::GammaDefinition();
    }
    if (particleName == "proton") {
        return G4Proton::ProtonDefinition();
    }
    if (particleName == "alpha") {
        return G4Alpha::AlphaDefinition();
    }
    if (particleName == "Li6") {
        return G4IonTable::GetIonTable()->GetIon(3, 6, 0.0);
    }
    if (particleName == "Li7") {
        return G4IonTable::GetIonTable()->GetIon(3, 7, 0.0);
    }
    if (particleName == "Be9") {
        return G4IonTable::GetIonTable()->GetIon(4, 9, 0.0);
    }
    if (particleName == "B10") {
        return G4IonTable::GetIonTable()->GetIon(5, 10, 0.0);
    }
    if (particleName == "B11") {
        return G4IonTable::GetIonTable()->GetIon(5, 11, 0.0);
    }
    if (particleName == "C12") {
        return G4IonTable::GetIonTable()->GetIon(6, 12, 0.0);
    }
    if (particleName == "C13") {
        return G4IonTable::GetIonTable()->GetIon(6, 13, 0.0);
    }
    if (particleName == "N14") {
        return G4IonTable::GetIonTable()->GetIon(7, 14, 0.0);
    }
    if (particleName == "N15") {
        return G4IonTable::GetIonTable()->GetIon(7, 15, 0.0);
    }
    if (particleName == "O16") {
        return G4IonTable::GetIonTable()->GetIon(8, 16, 0.0);
    }
    if (particleName == "O17") {
        return G4IonTable::GetIonTable()->GetIon(8, 17, 0.0);
    }
    if (particleName == "O18") {
        return G4IonTable::GetIonTable()->GetIon(8, 18, 0.0);
    }
    if (particleName == "F19") {
        return G4IonTable::GetIonTable()->GetIon(9, 19, 0.0);
    }
    if (particleName == "Ne20") {
        return G4IonTable::GetIonTable()->GetIon(10, 20, 0.0);
    }
    if (particleName == "Ne21") {
        return G4IonTable::GetIonTable()->GetIon(10, 21, 0.0);
    }
    if (particleName == "Ne22") {
        return G4IonTable::GetIonTable()->GetIon(10, 22, 0.0);
    }
    if (particleName == "Na23") {
        return G4IonTable::GetIonTable()->GetIon(11, 23, 0.0);
    }
    if (particleName == "Mg24") {
        return G4IonTable::GetIonTable()->GetIon(12, 24, 0.0);
    }
    if (particleName == "Mg25") {
        return G4IonTable::GetIonTable()->GetIon(12, 25, 0.0);
    }
    if (particleName == "Mg26") {
        return G4IonTable::GetIonTable()->GetIon(12, 26, 0.0);
    }
    if (particleName == "Al27") {
        return G4IonTable::GetIonTable()->GetIon(13, 27, 0.0);
    }
    if (particleName == "Si28") {
        return G4IonTable::GetIonTable()->GetIon(14, 28, 0.0);
    }
    if (particleName == "Si29") {
        return G4IonTable::GetIonTable()->GetIon(14, 29, 0.0);
    }
    if (particleName == "Si30") {
        return G4IonTable::GetIonTable()->GetIon(14, 30, 0.0);
    }

    // List of available particle names
    std::vector<std::string> availableParticles = {
        "proton", "alpha", "Li6", "Li7", "Be9", "B10", "B11",
        "C12", "C13", "N14", "N15", "O16", "O17", "O18",
        "F19", "Ne20", "Ne21", "Ne22", "Na23", "Mg24",
        "Mg25", "Mg26", "Al27", "Si28", "Si29", "Si30"
    };

    // Print error message with available particle names
    std::cerr << "Error: Unknown particle name '" << particleName << "'." << std::endl;
    std::cerr << "Available particles are: ";
    for (const auto& name : availableParticles) {
        std::cerr << name << " ";
    }
    std::cerr << std::endl;

    return nullptr;

}   //  ::GetUserParticle


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
