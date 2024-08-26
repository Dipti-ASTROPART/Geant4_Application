/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#include    "g4UserMaterial.hh"

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
MyMaterials::MyMaterials()
{
    ResetMaterialPointers();

    G3SC        = GRAPES_Scintillator_Material();
    WLSCORE     = WLSFiberCore_Material();
    WLSCLADIN   = WLSFiberCladIn_Material();
    WLSCLADOUT  = WLSFiberCladOut_Material();

    MiscellaneousMaterials();
    SetOpticalSurfaces();

}   //  ::MyMaterials


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void    MyMaterials::ResetMaterialPointers()
{
    AIR         = nullptr;
    TYVEK       = nullptr;
    VACCUUM     = nullptr;
    G3SC        = nullptr;
    WLSCORE     = nullptr;
    WLSCLADIN   = nullptr;
    WLSCLADOUT  = nullptr;
    ALUMINUM    = nullptr;

    TYVEK_SURFACE       = nullptr;
    CORE_CLAD_SURFACE   = nullptr;
    CORE_ENV_SURFACE    = nullptr;
    CLAD_CLAD_SURFACE   = nullptr;
    CLAD_ENV_SURFACE    = nullptr;
    PHOTON_DET_SURFACE  = nullptr;
    MIRROR_SURFACE      = nullptr;

}   //  ::ResetMaterialPointers()


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void    MyMaterials::MiscellaneousMaterials()
{
    G4NistManager *nist = G4NistManager::Instance();

    std::vector<G4double> photonEnergy  = {fOpticalPhotonMinEnergy, fOpticalPhotonMaxEnergy};

    //------------------------ VACCUUM -------------------------//
    VACCUUM = nist->FindOrBuildMaterial("G4_Galactic");
    //----------------------------------------------------------//

    //--------------------------- AIR --------------------------//
    AIR  = nist->FindOrBuildMaterial("G4_AIR");
                                                                                                                                                        
    std::vector<G4double> absLAir       = {1.90 *km, 3.47 *km};
    std::vector<G4double> rIndexAir     = {1.0, 1.0};
    
    G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();                                                                                
    airMPT->AddProperty("RINDEX", photonEnergy, rIndexAir);                                                                                       
    airMPT->AddProperty("ABSLENGTH", photonEnergy, absLAir);                                                                                          
    
    AIR->SetMaterialPropertiesTable(airMPT);
    //----------------------------------------------------------//

    //------------------------- TYVEK --------------------------//
    TYVEK = new G4Material("TYVEK", 0.92 * g/cm3, 2);
    TYVEK->AddElement(nist->FindOrBuildElement("C"), 2);
    TYVEK->AddElement(nist->FindOrBuildElement("H"), 4);

    TYVEK_SURFACE = new G4OpticalSurface("TYVEK_SURFACE");
    TYVEK_SURFACE->SetType(dielectric_metal);       //  Dielectric
    TYVEK_SURFACE->SetModel(glisur);
    TYVEK_SURFACE->SetFinish(polished);                  //  Surface finishing type

    // Initiate material properties table
    G4MaterialPropertiesTable *tyvekMPT = new G4MaterialPropertiesTable();

    std::vector<G4double> tyvekReflectivity = {0.90, 0.90};      //  Reflectiity of the tyvek surface
    //std::vector<G4double> tyvekEfficiency   = {1.00, 1.00};      //  Reflectiity of the tyvek surface

    tyvekMPT->AddProperty("REFLECTIVITY", photonEnergy, tyvekReflectivity);

    TYVEK->SetMaterialPropertiesTable(tyvekMPT);
    TYVEK_SURFACE->SetMaterialPropertiesTable(tyvekMPT);
    //----------------------------------------------------------//

    //---------------------- ALUMINUM --------------------------//
    ALUMINUM = nist->FindOrBuildMaterial("G4_Al");
    //----------------------------------------------------------//


}   //   ::MiscellaneousMaterials()


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
G4Material *MyMaterials::WLSFiberCladIn_Material()
{
    G4NistManager *nist = G4NistManager::Instance();

    std::vector<G4double> energySmall     = {fOpticalPhotonMinEnergy, fOpticalPhotonMaxEnergy};
    std::vector<G4double> cladin_rIndex   = {1.49, 1.49};
    std::vector<G4double> clad_absL       = {20.0 *m, 20.0 *m};

    // Build PMMA(C5H8O2) for inner clad
    G4Material *matWLSCLADIN = new G4Material("matWLSCLADIN", 1.19*g/cm3, 3);

    matWLSCLADIN->AddElement(nist->FindOrBuildElement("C"), 5);
    matWLSCLADIN->AddElement(nist->FindOrBuildElement("H"), 8);
    matWLSCLADIN->AddElement(nist->FindOrBuildElement("O"), 2);

    // Setting up inner clad (PMMA)
    G4MaterialPropertiesTable *mptCladIn = new G4MaterialPropertiesTable();
    mptCladIn->AddProperty("RINDEX",        energySmall,    cladin_rIndex);
    mptCladIn ->AddProperty("ABSLENGTH",    energySmall,    clad_absL);
    matWLSCLADIN->SetMaterialPropertiesTable(mptCladIn);

    return matWLSCLADIN;

}   //  ::WLSFiberCladIn_Material


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
G4Material *MyMaterials::WLSFiberCladOut_Material()
{
    G4NistManager *nist = G4NistManager::Instance();

    std::vector<G4double> energySmall     = {fOpticalPhotonMinEnergy, fOpticalPhotonMaxEnergy};
    std::vector<G4double> cladout_rIndex  = {1.42, 1.42};
    std::vector<G4double> clad_absL       = {20.0 *m, 20.0 *m};

    // Build F-PEthylene for outer clad
    G4Material *matWLSCLADOUT = new G4Material("matWLSCLADOUT", 1.43*g/cm3, 2);
    matWLSCLADOUT->AddElement(nist->FindOrBuildElement("C"), 2);
    matWLSCLADOUT->AddElement(nist->FindOrBuildElement("H"), 4);
    // setting up outer layer: F-Polystyrene
    G4MaterialPropertiesTable *mptCladOut = new G4MaterialPropertiesTable();
    mptCladOut->AddProperty("RINDEX",       energySmall,    cladout_rIndex);
    mptCladOut->AddProperty("ABSLENGTH",    energySmall,    clad_absL);
    matWLSCLADOUT->SetMaterialPropertiesTable(mptCladOut);

    return matWLSCLADOUT;

}   //  ::WLSFiberCladOut_Material()


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
G4Material *MyMaterials::WLSFiberCore_Material()
{
    G4NistManager *nist = G4NistManager::Instance();

    G4double optPhotMinE_ = fOpticalPhotonMinEnergy;
    G4double optPhotMaxE_ = fOpticalPhotonMaxEnergy;
    G4double noAbsLength_ = 1.e8  * m;
    G4double core_decayTimeConstant = 2.7 * ns;

    // Build the WLS fiber core made up of Polystyrene (C8H8)
    //matWLSCORE = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    G4Material *matWLSCORE = new G4Material("matWLSCore", 1.05 * g/cm3, 2);
    matWLSCORE->AddElement(nist->FindOrBuildElement("C"), 8);
    matWLSCORE->AddElement(nist->FindOrBuildElement("H"), 8);
    G4MaterialPropertiesTable *mptCore = new G4MaterialPropertiesTable();
    // REFRACTIVE INDEX
    std::vector<G4double> ri_energy = {optPhotMinE_,  optPhotMaxE_};
    std::vector<G4double> rIndex = {1.59,  1.59};
    mptCore->AddProperty("RINDEX", ri_energy, rIndex);

    //---------------- ABSORPTION LENGTH -----------------//
    //std::vector<G4double> abs_energy = {optPhotMinE_, hc_ / (750. * nm), hc_ / (740. * nm), hc_ / (380. * nm), hc_ / (370. * nm), optPhotMaxE_ };
    //std::vector<G4double> absLength = {noAbsLength_, noAbsLength_, 3.5 * m, 3.5 * m, noAbsLength_, noAbsLength_};
    std::vector<G4double> abs_energy = {optPhotMinE_, optPhotMaxE_ };
    std::vector<G4double> absLength = {3.5 * m, 3.5 * m};
    mptCore->AddProperty("ABSLENGTH", abs_energy, absLength);

    //----------------WLS ABSORPTION LENGTH---------------//
    std::vector<G4double> WLS_abs_energy = {
        optPhotMinE_,
        hc_ / (490. * nm), hc_ / (485. * nm), hc_ / (475. * nm), hc_ / (454. * nm),
        hc_ / (443. * nm), hc_ / (430. * nm), hc_ / (410. * nm), hc_ / (405. * nm),
        hc_ / (359. * nm), hc_ / (350. * nm), hc_ / (345. * nm),
        optPhotMaxE_
    };
    std::vector<G4double> WLS_absLength = {
        noAbsLength_,
        noAbsLength_, 44.2 * mm, 5.39 * mm, 0.395 * mm, // 490, 485, 475, 454 nm
        0.462 * mm, 0.354 * mm, 0.571 * mm, 0.612 * mm, // 443, 430, 410, 405 nm
        4.51 * mm,  4.81  * mm, noAbsLength_,           // 359, 350, 345  nm
        noAbsLength_
    };
    mptCore->AddProperty("WLSABSLENGTH", WLS_abs_energy, WLS_absLength);

    //---------------WLS EMISSION SPECTRUM---------------//
    std::vector<G4double> WLS_emi_energy = {
        1.908 * eV, 1.928 * eV, 1.958 * eV, 1.979 * eV, 2.013 * eV, 2.038 * eV, 2.072 * eV, 2.107 * eV, 2.148 * eV, 2.197 * eV,
        2.232 * eV, 2.258 * eV, 2.282 * eV, 2.297 * eV, 2.317 * eV, 2.337 * eV, 2.351 * eV, 2.380 * eV, 2.399 * eV, 2.417 * eV,
        2.438 * eV, 2.451 * eV, 2.468 * eV, 2.485 * eV, 2.498 * eV, 2.513 * eV, 2.524 * eV, 2.536 * eV, 2.551 * eV, 2.573 * eV,
        2.588 * eV, 2.600 * eV, 2.611 * eV, 2.621 * eV, 2.634 * eV, 2.643 * eV, 2.648 * eV, 2.665 * eV, 2.683 * eV, 2.700 * eV,
        2.717 * eV, 2.734 * eV, 2.754 * eV, 2.779 * eV, 2.812 * eV, 2.847 * eV, 2.878 * eV
    };

    std::vector<G4double> WLS_emiSpectrum = {
        0.008, 0.008, 0.013, 0.013, 0.016, 0.022, 0.030, 0.048, 0.068, 0.097,
        0.133, 0.178, 0.236, 0.284, 0.330, 0.370, 0.387, 0.408, 0.451, 0.516,
        0.622, 0.700, 0.817, 0.920, 0.974, 1.000, 0.983, 0.935, 0.860, 0.757,
        0.730, 0.738, 0.757, 0.774, 0.780, 0.773, 0.759, 0.674, 0.523, 0.371,
        0.254, 0.160, 0.090, 0.041, 0.019, 0.012, 0.015
    };
    mptCore->AddProperty("WLSCOMPONENT",  WLS_emi_energy, WLS_emiSpectrum);

    // WLS Delay
    mptCore->AddConstProperty("WLSTIMECONSTANT", core_decayTimeConstant);

    // WLS Quantum Efficiency
    mptCore->AddConstProperty("WLSMEANNUMBERPHOTONS", 0.87);

    matWLSCORE->SetMaterialPropertiesTable(mptCore);

    return matWLSCORE;

}   //  ::WLSFiberCore_Material()


/////////////////////////////////////////////////////////////////////////
G4Material *MyMaterials::GRAPES_Scintillator_Material()
{
    // Define the elements for the materials
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* H  = nist->FindOrBuildElement("H");
    G4Element* C  = nist->FindOrBuildElement("C");
    G4Element* O  = nist->FindOrBuildElement("O");
    G4Element* N  = nist->FindOrBuildElement("N");
    G4Element* S  = nist->FindOrBuildElement("S");
    G4Element* Cd = nist->FindOrBuildElement("Cd");

    // Define Polystyrene (Styrene C8H8)
    G4Material* polystyrene = new G4Material("Polystyrene", 1.050 * g / cm3, 2);
    polystyrene->AddElement(C, 8);
    polystyrene->AddElement(H, 8);

    // Define para-Terphenyl (c18H14)
    G4Material* ppo = new G4Material("PPO", 1.24 * g / cm3, 2);
    ppo->AddElement(C, 18);
    ppo->AddElement(H, 14);

    // Define POPOP (C_25 H_16 N_2 O_2)
    G4Material* popop = new G4Material("POPOP", 1.30 * g / cm3, 4);
    popop->AddElement(C, 25);
    popop->AddElement(H, 16);
    popop->AddElement(O, 2);
    popop->AddElement(N, 2);

    // Define Impurity CdS
    G4Material *cds = new G4Material("CdS", 4.82 * g/cm3, 2);
    cds->AddElement(Cd, 1);
    cds->AddElement(S, 1);

    // Define the scintillator material
    G4Material* scintillator = new G4Material("Scintillator", 1.032 * g / cm3, 3);
    scintillator->AddMaterial(polystyrene, 0.987); // 99.4% Polystyrene
    scintillator->AddMaterial(ppo, 0.010);       // 0.4% PPO
    scintillator->AddMaterial(popop, 0.0030);     // 0.01% POPOP

    std::vector<G4double> energyrange = {fOpticalPhotonMinEnergy, fOpticalPhotonMaxEnergy };
    std::vector<G4double> refractiveIndexPS = { 1.58, 1.58 };
    std::vector<G4double> absPS = {1.0* m, 1.0* m }; 
    std::vector<G4double> absWLSPS = {3.5* m, 3.5* m }; 

    // Measured emission spectrum
    std::vector<G4double> abs_energy = {
        2.254 * eV, 2.283 * eV, 2.340 * eV, 2.370 * eV, 2.400 * eV, 2.462 * eV, 2.505 * eV, 2.543 * eV, 2.568 * eV, 2.607 * eV,
        2.661 * eV, 2.686 * eV, 2.712 * eV, 2.738 * eV, 2.767 * eV, 2.788 * eV, 2.817 * eV, 2.839 * eV, 2.867 * eV, 2.888 * eV,
        2.901 * eV, 2.913 * eV, 2.925 * eV, 2.932 * eV, 2.944 * eV, 2.962 * eV, 2.976 * eV, 2.983 * eV, 2.997 * eV, 3.009 * eV,
        3.019 * eV, 3.035 * eV, 3.045 * eV, 3.055 * eV, 3.067 * eV, 3.075 * eV, 3.082 * eV, 3.090 * eV, 3.097 * eV, 3.105 * eV,
        3.110 * eV, 3.118 * eV, 3.126 * eV, 3.133 * eV, 3.141 * eV, 3.149 * eV, 3.157 * eV, 3.165 * eV, 3.175 * eV, 3.189 * eV,
        3.203 * eV, 3.219 * eV, 3.244 * eV, 3.276 * eV, 3.311 * eV, 3.343 * eV, 3.377 * eV, 3.408 * eV, 3.439 * eV, 3.474 * eV,
        3.507 * eV, 3.534 * eV, 3.554 * eV, 3.578 * eV, 3.606 * eV, 3.637 * eV, 3.659 * eV, 3.684 * eV, 3.717 * eV, 3.747 * eV,
        3.777 * eV, 3.800 * eV, 3.823 * eV, 3.855 * eV
    };

    std::vector<G4double> scintilFast = {
        0.015, 0.018, 0.026, 0.035, 0.047, 0.069, 0.098, 0.139, 0.173, 0.218,
        0.255, 0.296, 0.346, 0.430, 0.501, 0.537, 0.556, 0.556, 0.587, 0.660,
        0.727, 0.809, 0.870, 0.923, 0.974, 1.000, 0.976, 0.947, 0.870, 0.788,
        0.723, 0.624, 0.573, 0.529, 0.486, 0.455, 0.433, 0.407, 0.378, 0.354,
        0.329, 0.307, 0.281, 0.257, 0.233, 0.214, 0.190, 0.170, 0.148, 0.127,
        0.108, 0.093, 0.083, 0.083, 0.088, 0.100, 0.115, 0.132, 0.143, 0.146,
        0.155, 0.168, 0.182, 0.197, 0.201, 0.192, 0.177, 0.163, 0.163, 0.168,
        0.158, 0.131, 0.096, 0.061
    };

    auto mptG3SC = new G4MaterialPropertiesTable();
    mptG3SC->AddProperty("RINDEX", energyrange, refractiveIndexPS);
    mptG3SC->AddProperty("WLSABSLENGTH", energyrange, absWLSPS);
    mptG3SC->AddProperty("ABSLENGTH", energyrange, absPS);
    mptG3SC->AddProperty("SCINTILLATIONCOMPONENT1", abs_energy, scintilFast);
    //mptG3SC->AddProperty("WLSCOMPONENT", abs_energy, scintilFast);
    mptG3SC->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
    mptG3SC->AddConstProperty("RESOLUTIONSCALE", 1.0); 
    mptG3SC->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1 * ns);
    mptG3SC->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 14.2 * ns);

    // Set the material properties to the material
    scintillator->SetMaterialPropertiesTable(mptG3SC);

    // Set the Birks Constant for the Polystyrene scintillator
    scintillator->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

    return scintillator;

}   //  ::GRAPES_Scintillator_Material()


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
void    MyMaterials::SetOpticalSurfaces()
{

    std::vector<G4double> photonEnergies= {fOpticalPhotonMinEnergy, fOpticalPhotonMaxEnergy};
    std::vector<G4double> reflectivity  = {0.9,  0.9};
    std::vector<G4double> transmittance = {1.0,  1.0};

    // 1. Core-to-Inner Cladding Surface--------------------------//
    CORE_CLAD_SURFACE = new G4OpticalSurface("CoreToInnerCladSurface");
    CORE_CLAD_SURFACE->SetType(dielectric_dielectric);
    CORE_CLAD_SURFACE->SetFinish(ground);  // or rough, depending on your requirement
    CORE_CLAD_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* coreToInnerMPT = new G4MaterialPropertiesTable();
    //coreToInnerMPT->AddProperty("REFLECTIVITY", photonEnergies, reflectivity, nEntries);
    //coreToInnerMPT->AddProperty("TRANSMITTANCE", photonEnergies, transmittance, nEntries);
    CORE_CLAD_SURFACE->SetMaterialPropertiesTable(coreToInnerMPT);
    //------------------------------------------------------------//

    // 2. Inner Cladding-to-Outer Cladding Surface----------------//
    G4OpticalSurface* CLAD_CLAD_SURFACE = new G4OpticalSurface("InnerToOuterCladSurface");
    CLAD_CLAD_SURFACE->SetType(dielectric_dielectric);
    CLAD_CLAD_SURFACE->SetFinish(ground);  // or rough
    CLAD_CLAD_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* innerToOuterMPT = new G4MaterialPropertiesTable();
    //innerToOuterMPT->AddProperty("REFLECTIVITY", photonEnergies, reflectivity, nEntries);
    //innerToOuterMPT->AddProperty("TRANSMITTANCE", photonEnergies, transmittance, nEntries);
    CLAD_CLAD_SURFACE->SetMaterialPropertiesTable(innerToOuterMPT);
    //------------------------------------------------------------//

    // 3. Outer Cladding-to-Air Surface --------------------------//
    G4OpticalSurface* CLAD_ENV_SURFACE = new G4OpticalSurface("OuterCladToAirSurface");
    CLAD_ENV_SURFACE->SetType(dielectric_dielectric);
    CLAD_ENV_SURFACE->SetFinish(ground);  // or rough
    CLAD_ENV_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* outerCladToAirMPT = new G4MaterialPropertiesTable();
    //outerCladToAirMPT->AddProperty("REFLECTIVITY", photonEnergies, reflectivity, nEntries);
    //outerCladToAirMPT->AddProperty("TRANSMITTANCE", photonEnergies, transmittance, nEntries);
    CLAD_ENV_SURFACE->SetMaterialPropertiesTable(outerCladToAirMPT);
    //------------------------------------------------------------//

    // Define the Optical Surface for the Core-to-Air Interface---//
    CORE_ENV_SURFACE = new G4OpticalSurface("CoreToAirSurface");
    CORE_ENV_SURFACE->SetType(dielectric_dielectric);
    CORE_ENV_SURFACE->SetFinish(ground);  // or rough, depending on your requirement
    CORE_ENV_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* coreToAirMPT = new G4MaterialPropertiesTable();
    //coreToAirMPT->AddProperty("REFLECTIVITY", photonEnergies, reflectivity);  // Set appropriate reflectivity
    //coreToAirMPT->AddProperty("TRANSMITTANCE", photonEnergies, transmittance);  // Set appropriate transmittance
    CORE_ENV_SURFACE->SetMaterialPropertiesTable(coreToAirMPT);
    //------------------------------------------------------------//

    //----------- sensitive detector surface properties ----------//
    PHOTON_DET_SURFACE = new G4OpticalSurface("CoreToAirSurface");

    PHOTON_DET_SURFACE->SetType(dielectric_metal);
    PHOTON_DET_SURFACE->SetFinish(ground);  // or rough, depending on your requirement
    PHOTON_DET_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* photonDetSurfaceProperty = new G4MaterialPropertiesTable();
    std::vector<G4double> refl_mppc = { 0.0, 0.0 };
    std::vector<G4double> effi_mppc = { 1., 1. };
    photonDetSurfaceProperty->AddProperty("REFLECTIVITY", photonEnergies, refl_mppc);
    photonDetSurfaceProperty->AddProperty("EFFICIENCY", photonEnergies, effi_mppc);
    
    PHOTON_DET_SURFACE->SetMaterialPropertiesTable(photonDetSurfaceProperty);
    //------------------------------------------------------------//

    //---------------------- Mirror surface ----------------------//
    MIRROR_SURFACE = new G4OpticalSurface("CoreToAirSurface");

    MIRROR_SURFACE->SetType(dielectric_metal);
    MIRROR_SURFACE->SetFinish(ground);  // or rough, depending on your requirement
    MIRROR_SURFACE->SetModel(glisur);

    G4MaterialPropertiesTable* mirrorSurfaceProperty = new G4MaterialPropertiesTable();
    std::vector<G4double> refl_mirror = { 1.0, 1.0 };
    std::vector<G4double> effi_mirror = { 0.0, 0.0 };
    mirrorSurfaceProperty->AddProperty("REFLECTIVITY", photonEnergies, refl_mirror);
    //mirrorSurfaceProperty->AddProperty("EFFICIENCY", p_mppc, effi_mppc);
    
    MIRROR_SURFACE->SetMaterialPropertiesTable(mirrorSurfaceProperty);
    //------------------------------------------------------------//

}   //  ::SetOpticalSurfaces()


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
