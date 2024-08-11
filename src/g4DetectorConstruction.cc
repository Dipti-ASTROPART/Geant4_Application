//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    "g4DetectorConstruction.hh"

//////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////////////
MyDetectorConstruction::MyDetectorConstruction()
{

    DefineMaterial();

}   //  ::MyDetectorConstruction


//////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////////////
MyDetectorConstruction::~MyDetectorConstruction()
{

    delete   matNaI;
    delete   matAir;
    delete   matG3SC;
    delete   matBC404;
    delete   matPb;
    delete   matWLSCore;
    delete   matWLSCladIn;
    delete   matWLSCladOut;
    delete   matTyvek;

}   //  ::MyDetectorConstruction


//////////////////////////////////////////////////////////////////////////////////
/// Construct the detector
//////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume   *MyDetectorConstruction::Construct()
{
    return  ConstructG3CylindricalDetector();
    return  ConstructTrapezoidalDetector();
}


//////////////////////////////////////////////////////////////////////////////////
/// Construct cylindrical detector for the GRAPES-3 test set up
//////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume   *MyDetectorConstruction::ConstructG3CylindricalDetector()
{
    G4bool      checkOverlaps = true;

    // World volume coordinates
    G4double    world_x = 0.4*m;
    G4double    world_z = 0.4*m;
    G4double    world_y = 0.4*m;

    // Define solid world / world VOLUME
    solidWorld = new G4Box("solidWorld", world_x/2.0, world_y/2.0, world_z/2.0);

    // Logic volume inside the world VOLUME
    logicWorld = new G4LogicalVolume(solidWorld, matAir, "logicWorld");

    // Physical world volume
    physWorld = new G4PVPlacement(0,                ///< No rotation
            G4ThreeVector(0.0, 0.0, 0.0),           ///< Translation position
            logicWorld,                             ///< Mother volume address
            "physWorld",                            ///< Name
            0,                                      ///< 0, if mother volume
            false,                                  ///< For boolean operation
            0,                                      ///< Copy numbers
            checkOverlaps                           ///< Check for Repeatation ?
            );

    //BuildCylindricalDetector();
    BuildCylindricalDetectorWithTyvek();
    //BuildCylindricalDetectorWithLead();

    return physWorld;
}   //  :: ConstructG3CylindricalDetector()


//////////////////////////////////////////////////////////////////////////////////
/// Build a solid cylindrical detector
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildCylindricalDetector()
{
    G4bool      checkOverlaps = true;
    G4double    detRadius  = 2.5*cm;
    G4double    detHeight  = 15.0*cm;
    G4double    startAngle = 0.0*deg;
    G4double    stopAngle  = 360.0*deg;

    solidCyl = new G4Tubs("solidCylinderSC", 0, detRadius, detHeight/2.0, startAngle, stopAngle);

    logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicCylinderSC");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* scVisAttr = new G4VisAttributes();

    scVisAttr->SetColor(G4Colour(176,196,222, 0.5));
    scVisAttr->SetVisibility(true);
    scVisAttr->SetForceSolid(true);
    logicDet->SetVisAttributes(scVisAttr);

    physDet = new G4PVPlacement(0,                      // no rotation
            G4ThreeVector(0, 0, detHeight/2.0),       // shifted position for detector
            logicDet,               // logical volume for detector
            "CylindricalSolid",     // name
            logicWorld,             // mother volume
            false,                  // no boolean operations
            0,                      // copy number
            checkOverlaps);         // checking overlaps

}   //  ::BuildCylindricalDetector()


//////////////////////////////////////////////////////////////////////////////////
/// Build cylindrical detector with tyvek coating
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildCylindricalDetectorWithTyvek()
{
    G4bool      checkOverlaps = true;
    G4double    detRadius  = 2.5*cm;
    G4double    detHeight  = 3*cm;
    G4double    startAngle = 0.0*deg;
    G4double    stopAngle  = 360.0*deg;

    solidCyl = new G4Tubs("solidCylinderSC", 0, detRadius, detHeight/2.0, startAngle, stopAngle);

    logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicCylinderSC");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* scVisAttr = new G4VisAttributes();
    G4VisAttributes* tyvekVisAttr = new G4VisAttributes();

    scVisAttr->SetColor(G4Colour(176,196,222, 0.5));
    scVisAttr->SetVisibility(true);
    scVisAttr->SetForceSolid(true);
    logicDet->SetVisAttributes(scVisAttr);

    const G4int num = 2;

    G4double photonEnergy[num] = {2.0*eV, 3.0*eV};  //  visible photon energy range
    G4double reflectivity[num] = {0.90, 0.90};      //  Reflectiity of the tyvek surface
    G4double refIndex[num] = {1.0003, 1.0003};      //  Reflectiity of the tyvek surface

    // Initiate an optical surface
    G4OpticalSurface *tyvekSurface = new G4OpticalSurface("TyvekSurface");
    tyvekSurface->SetType(dielectric_metal);       //  Dielectric 
    tyvekSurface->SetModel(unified);
    tyvekSurface->SetFinish(polished);                  //  Surface finishing type

    // Initiate material properties table
    G4MaterialPropertiesTable *tyvekMPT = new G4MaterialPropertiesTable();
    tyvekMPT->AddProperty("REFLECTIVITY", photonEnergy, reflectivity, num);
    tyvekSurface->SetMaterialPropertiesTable(tyvekMPT);

    /*  
        physDet = new G4PVPlacement(0,                      // no rotation
        G4ThreeVector(0, 0, detHeight/2.0),       // shifted position for detector
        logicDet,               // logical volume for detector
        "CylindricalSolid",     // name
        logicWorld,             // mother volume
        false,                  // no boolean operations
        0,                      // copy number
        checkOverlaps);         // checking overlaps
        */
    for (int i = 0; i < 5; ++i) 
    {
        // Calculate z position for each detector
        G4double zPositionDetector = i*detHeight + detHeight/2.0;
        G4ThreeVector positionDetector = G4ThreeVector(0, 0, zPositionDetector);
        G4VPhysicalVolume *physDet = new G4PVPlacement(0,                      // no rotation
                positionDetector,       // shifted position for detector
                logicDet,               // logical volume for detector
                "CylindricalSolid",     // name
                logicWorld,             // mother volume
                false,                  // no boolean operations
                i,                      // copy number
                checkOverlaps);         // checking overlaps

    }

    //new G4LogicalSkinSurface("TyvekSurface", logicDet, tyvekSurface);

}   //  ::BuildCylindricalDetectorWithTyvek()


//////////////////////////////////////////////////////////////////////////////////
/// Build a solid cylindrical detector with lead in between
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildCylindricalDetectorWithLead()
{
    G4bool      checkOverlaps = true;
    G4double    detRadius  = 2.5*cm;
    G4double    detHeight  = 3.0*cm;
    G4double    startAngle = 0.0*deg;
    G4double    stopAngle  = 360.0*deg;
    G4double    leadThick  = 2.0*mm;

    solidCyl = new G4Tubs("solidCylinderSC", 0, detRadius, detHeight/2.0, startAngle, stopAngle);

    // Create cylindrical solid for lead sheets
    G4Tubs* solidLead = new G4Tubs("LeadSheet", 0, detRadius, leadThick/2, startAngle, stopAngle);

    logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicCylinderSC");

    // Logical volume for lead sheets
    G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, matPb, "LeadSheet");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* scVisAttr = new G4VisAttributes();

    scVisAttr->SetColor(G4Colour(176,196,222, 0.5));
    scVisAttr->SetVisibility(true);
    scVisAttr->SetForceSolid(true);
    logicDet->SetVisAttributes(scVisAttr);

    G4VisAttributes *leadVisAttr = new G4VisAttributes();
    leadVisAttr->SetColor(G4Colour(0, 255, 255, 0.5));
    leadVisAttr->SetVisibility(true);
    leadVisAttr->SetForceSolid(true);
    logicLead->SetVisAttributes(leadVisAttr);

    for (int i = 0; i < 5; ++i) 
    {
        // Calculate z position for each detector
        G4double zPositionDetector = i * (detHeight + leadThick) + detHeight/2.0;
        G4ThreeVector positionDetector = G4ThreeVector(0, 0, zPositionDetector);
        new G4PVPlacement(0,                      // no rotation
                positionDetector,       // shifted position for detector
                logicDet,               // logical volume for detector
                "CylindricalSolid",     // name
                logicWorld,             // mother volume
                false,                  // no boolean operations
                i,                      // copy number
                checkOverlaps);         // checking overlaps

        // Calculate z position for each lead sheet (except after the last detector)
        if (i < 4) 
        {
            G4double zPositionLead = (i+1) * detHeight + (i+0.5) * leadThick;
            G4ThreeVector positionLead = G4ThreeVector(0, 0, zPositionLead);
            new G4PVPlacement(0,                  // no rotation
                    positionLead,       // shifted position for lead sheet
                    logicLead,          // logical volume for lead sheet
                    "LeadSheet",        // name
                    logicWorld,         // mother volume
                    false,              // no boolean operations
                    i,                  // copy number
                    checkOverlaps);     // checking overlaps
        }
    }

}   //  ::BuildCylindricalDetector()


//////////////////////////////////////////////////////////////////////////////////
/// Construct a trapezoidal plastic scintillator detector
//////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume   *MyDetectorConstruction::ConstructTrapezoidalDetector()
{
    // Construct the WORLD VOLUME which is a solid
    G4double    world_x = 2.0*m;
    G4double    world_y = 2.0*m;
    G4double    world_z = 2.0*m;

    solidWorld = new G4Box("solidWorld", 0.5*world_x, 0.5*world_y, 0.5*world_z);

    // Construct a logic VOLUME inside solid world volume and provide material
    logicWorld = new G4LogicalVolume(solidWorld, matAir, "logicWorld");

    // Construct the physical world volume
    physWorld = new G4PVPlacement(
            0,                          ///< Rotation
            G4ThreeVector(0., 0., 0.0), ///< Translation position
            logicWorld,                 ///< Mother volume name
            "physWorld",                ///< Name 
            0,                          ///< 0, if mother volume
            false,                      ///< For boolean operation
            0,                          ///< Copy number
            true                        ///< Repeatation check
            );

    //----- Build Trapezoidal Detector -------- //

    G4double pDz    = 1.0  * cm;    ///< Half-length along z-axis
    G4double pTheta = 0.0  * rad;   ///< Polar angle of the line joining the centres of the faces parallel to the z plane
    G4double pPhi   = 0.0  * rad;   ///< Azimuthal angle of the line joining the centres of the faces parallel to the z plane
    G4double pDy1   = 50.0 * cm;    ///< Half-length along y-axis at -pDz
    G4double pDx1   = 50.0 * cm;    ///< Half-length along x-axis at -pDz, -pDy1
    G4double pDx2   = 50.0 * cm;    ///< Half-length along x-axis at -pDz, +pDy1
    G4double pDy2   = 60.0 * cm;    ///< Half-length along y-axis at +pDz
    G4double pDx3   = 60.0 * cm;    ///< Half-length along x-axis at +pDz, -pDy2
    G4double pDx4   = 60.0 * cm;    ///< Half-length along x-axis at +pDz, +pDy2
    G4double pAlph  = 0.0  * rad;   ///< Angle with respect to the y-axis from the centre of the lower side to the centre of the upper side


    // Build the solid detector of trapezoidal shape
    solidDet = new G4Trap("TrapezoidDetector", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlph, pDy2, pDx3, pDx4, pAlph);

    // Build the Logical volume using the solid detector
    logicDet = new G4LogicalVolume(solidDet, matBC404, "LogicalDetector");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour::Red());
    logicDet->SetVisAttributes(visAttributes);

    // Building the physical detector
    physDet = new G4PVPlacement(0, 
            G4ThreeVector(0., 0., 0.5*m),
            logicDet,
            "physDet",
            logicWorld,
            false,
            0,
            true
            );


    /*  
        G4double    det_x = 0.5*m, 
        det_y = 0.5*m,
        det_z = 0.01*m;
    // Building the world detector
    G4Box *worldDet = new G4Box("worldDet", det_x, det_y, det_z);

    // Building the logical detector
    G4LogicalVolume *logicDet = new G4LogicalVolume(worldDet, NaI, "logicDet");

    // Building the physical detector
    G4VPhysicalVolume *physDet = new G4PVPlacement(0, 
    G4ThreeVector(0., 0., 0.75*m),
    logicDet,
    "physDet",
    logicWorld,
    false,
    0,
    true
    );

    //  Reutn the physical world volume
    */
    return physWorld;
}


//////////////////////////////////////////////////////////////////////////////////
/// Return the detector material
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::DefineMaterial()
{
    matG3SC  = G3ScintillatorMaterial();
    matBC404 = PSD_BC404_Material();
    matNaI   = NaI_Material();

    ///< Define Tyvek material made up of polyethylene

    // Instanciate the G4NistManager
    G4NistManager *nist   =   G4NistManager::Instance();

    matAir  = nist->FindOrBuildMaterial("G4_AIR");

    G4int   nEntries = 2;
    // Define energies
    G4double photonEnergy[nEntries] = {2.00 *eV, 3.47 *eV};
    // Define refractive index
    G4double refractiveIndex[nEntries] = {1.0003, 1.0003};

    G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
    airMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
    matAir->SetMaterialPropertiesTable(airMPT);

    matTyvek = new G4Material("matTyvek", 0.92 * g/cm3, 2);
    matTyvek->AddElement(nist->FindOrBuildElement("C"), 2);
    matTyvek->AddElement(nist->FindOrBuildElement("H"), 4);


    // Define Leab Material
    matPb = nist->FindOrBuildMaterial("G4_Pb");

}   //  ::DefineMaterial()


//////////////////////////////////////////////////////////////////////////////////
/// Build the NaI Material
//////////////////////////////////////////////////////////////////////////////////
G4Material  *MyDetectorConstruction::NaI_Material()
{
    // Instanciate the G4NistManager
    G4NistManager *nist   =   G4NistManager::Instance();

    // Define NaI(Tl) material
    G4Material *NaI = new G4Material("NaI", 
            3.67*g/cm3,
            2);
    NaI->AddElement(nist->FindOrBuildElement("Na"), 1);
    NaI->AddElement(nist->FindOrBuildElement("I"), 1);

    return NaI;

}   //  ::DefineMaterial()


//////////////////////////////////////////////////////////////////////////////////
/// Build the scintillator detector material built from BC-404
//////////////////////////////////////////////////////////////////////////////////
G4Material  *MyDetectorConstruction::G3ScintillatorMaterial()
{
    // Define the elements for the materials
    G4NistManager* nist = G4NistManager::Instance();
    G4Element* H = nist->FindOrBuildElement("H");
    G4Element* C = nist->FindOrBuildElement("C");
    G4Element* O = nist->FindOrBuildElement("O");
    G4Element* N = nist->FindOrBuildElement("N");
    G4Element* S = nist->FindOrBuildElement("S");
    G4Element* Cd = nist->FindOrBuildElement("Cd");

    // Define Polystyrene (Styrene C8H8)
    G4Material* polystyrene = new G4Material("Polystyrene", 1.050 * g / cm3, 2);
    polystyrene->AddElement(C, 9);
    polystyrene->AddElement(H, 10);

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
                                                  //scintillator->AddMaterial(cds, 0.02);        // 0.2% CdS

    std::vector<G4double> wls_Energy = { 2.00 * eV, 2.87 * eV, 2.90 * eV, 3.47 * eV };
    std::vector<G4double> rIndexPstyrene = { 0.1, 0.1, 0.1, 0.1 };
    std::vector<G4double> reflectivityPstyrene = { 0.90, 0.90, 0.90, 0.90 };
    std::vector<G4double> absorption1    = { 2. * cm, 2. * cm, 2. * cm, 2. * cm };
    std::vector<G4double> scintilFast    = { 0.0, 0.0, 1.0, 1.0 };

    auto mptG3SC = new G4MaterialPropertiesTable();
    mptG3SC->AddProperty("RINDEX", wls_Energy, rIndexPstyrene);
    //mptG3SC->AddProperty("REFLECTIVITY", wls_Energy, reflectivityPstyrene);
    mptG3SC->AddProperty("ABSLENGTH", wls_Energy, absorption1);
    mptG3SC->AddProperty("SCINTILLATIONCOMPONENT1", wls_Energy, scintilFast);
    mptG3SC->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
    mptG3SC->AddConstProperty("RESOLUTIONSCALE", 1.0); 
    mptG3SC->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 10. * ns);

    // Set the material properties to the material
    scintillator->SetMaterialPropertiesTable(mptG3SC);

    // Set the Birks Constant for the Polystyrene scintillator
    scintillator->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

    return scintillator;
}   //  ::G3ScintillatorMaterial()


//////////////////////////////////////////////////////////////////////////////////
/// Construct Kuraray double clad WLS fiber
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::ConstructFiberMaterial()
{
    constexpr G4int NUMENTRIES = 26;
    G4double core_photonEnergy[NUMENTRIES] = { 2.32*eV, 2.35*eV, 2.38*eV, 2.41*eV, 2.44*eV, 2.47*eV,
        2.50*eV, 2.53*eV, 2.56*eV, 2.59*eV, 2.62*eV,
        2.65*eV, 2.68*eV, 2.71*eV, 2.74*eV, 2.77*eV,
        2.80*eV, 2.83*eV, 2.86*eV, 2.89*eV, 2.92*eV,
        2.95*eV, 2.98*eV, 3.01*eV, 3.04*eV, 3.07*eV};

    G4double core_scintilationSpectra[NUMENTRIES] = { 0.007, 0.021, 0.035, 0.048, 0.065, 0.093,
        0.120, 0.148, 0.176, 0.233, 0.318,
        0.408, 0.504, 0.568, 0.625, 0.682,
        0.772, 0.896, 0.985, 0.895, 0.785,
        0.638, 0.326, 0.127, 0.042, 0.000};

    G4double core_pathLenght[NUMENTRIES] = { 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
        2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
        2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
        2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m,
        2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m, 2.2 * m};

    G4double rEnergies[2] = {2*eV, 3.5*eV};

    //const G4double core_RefractiveIndex = 1.6;
    G4double core_rIndex[2] = {1.59, 1.59};

    //const G4double clad1_RefractiveIndex = 1.49;
    G4double clad1_rIndex[2] = {1.49, 1.49};

    //const G4double clad2_RefractiveIndex = 1.42;
    G4double clad2_rIndex[2] = {1.42, 1.42};


    const G4double core_scintilationYield = 8000 / MeV;

    const G4double core_YieldRatio = 1.0;

    const G4double core_decayTimeConstant = 2.7 * ns;

    const G4double core_resolutionScale = 1.0;

    constexpr G4int NUMENTRIESCLAD = 50;

    G4double clad_photonEnergy[NUMENTRIESCLAD] = {
        2.00 * eV, 2.03 * eV, 2.06 * eV, 2.09 * eV, 2.12 * eV, 2.15 * eV, 2.18 * eV,
        2.21 * eV, 2.24 * eV, 2.27 * eV, 2.30 * eV, 2.33 * eV, 2.36 * eV, 2.39 * eV,
        2.42 * eV, 2.45 * eV, 2.48 * eV, 2.51 * eV, 2.54 * eV, 2.57 * eV, 2.60 * eV,
        2.63 * eV, 2.66 * eV, 2.69 * eV, 2.72 * eV, 2.75 * eV, 2.78 * eV, 2.81 * eV,
        2.84 * eV, 2.87 * eV, 2.90 * eV, 2.93 * eV, 2.96 * eV, 2.99 * eV, 3.02 * eV,
        3.05 * eV, 3.08 * eV, 3.11 * eV, 3.14 * eV, 3.17 * eV, 3.20 * eV, 3.23 * eV,
        3.26 * eV, 3.29 * eV, 3.32 * eV, 3.35 * eV, 3.38 * eV, 3.41 * eV, 3.44 * eV,
        3.47 * eV
    };

    G4double clad_pathLenght[NUMENTRIESCLAD] = {
        5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
        5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
        5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
        5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m, 5.40 * m,
        5.40 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m, 1.10 * m,
        1.10 * m, 1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
        1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,  1. * mm,
        1. * mm
    };


}   //  ::WLSFiberMaterial()


//////////////////////////////////////////////////////////////////////////////////
/// Build the scintillator detector material built from BC-404
//////////////////////////////////////////////////////////////////////////////////
G4Material  *MyDetectorConstruction::PSD_BC404_Material()
{
    // Instanciate the G4NistManager
    G4NistManager *nist   =   G4NistManager::Instance();

    // Define BC-404 made from PolyVinylTolune (C9H10)
    G4Material  *PVT = new G4Material("PVT", 1.032*g/cm3, 2);

    // Add material elements
    PVT->AddElement(nist->FindOrBuildElement("C"), 9);
    PVT->AddElement(nist->FindOrBuildElement("H"), 10);

    // Instantiate material propoerties table
    G4MaterialPropertiesTable *mptPVT = new G4MaterialPropertiesTable();

    const   G4int nEntries = 2;

    // Define energies
    G4double photonEnergy[nEntries] = {2.00 *eV, 3.47 *eV};

    // Define refractive index
    G4double refractiveIndex[nEntries] = {1.58, 1.58};

    // Define absorption length
    G4double absorptionLength[nEntries] = {3.8*m, 3.8*m};

    // Define scintillation yeild
    G4double scintillationYield = 10000./MeV;

    // Define resolution scale
    G4double resolutionScale = 1.0;

    // Define fast time constant
    G4double fastTimeConstant = 1.8*ns;

    // Add the properties to the material propoerties table
    mptPVT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
    mptPVT->AddProperty("ABSLENGTH", photonEnergy, absorptionLength, nEntries);
    mptPVT->AddConstProperty("SCINTILLATIONYIELD", scintillationYield);
    mptPVT->AddConstProperty("RESOLUTIONSCALE", resolutionScale);
    mptPVT->AddConstProperty("SCINTILLATIONTIMECONSTANT1", fastTimeConstant);

    // Set the material properties to the material
    PVT->SetMaterialPropertiesTable(mptPVT);

    return PVT;
}   //  ::PSD_BC404_Material()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
