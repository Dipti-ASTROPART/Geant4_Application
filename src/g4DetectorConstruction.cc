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
    //delete   matWLSCore;
    //delete   matWLSCladIn;
    //delete   matWLSCladOut;
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
    G4double    world_x = 0.2*m;
    G4double    world_z = 0.5*m;
    G4double    world_y = 0.2*m;

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

    //BuildWLSFiber();
    //BuildCylindricalDetector();
    BuildCylindricalDetectorWithTyvek();
    //BuildCylindricalDetectorWithLead();

    return physWorld;
}   //  :: ConstructG3CylindricalDetector()


//////////////////////////////////////////////////////////////////////////////////
/// Construct the WLS fiber
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildWLSFiber()
{
    G4bool      checkOverlaps    = true;
    G4double    fiberHeight      = 40.0 *cm;
    G4double    fiberRadius      = 1.0 *cm;
    G4double    fiberCoreRadius  = 0.96 * fiberRadius; 
    G4double    fiberCladRadius1 = fiberCoreRadius + 0.02*fiberRadius; 
    G4double    fiberCladRadius2 = fiberCladRadius1 + 0.02*fiberRadius; 
    G4double    startAngle       = 0.0  *deg;
    G4double    stopAngle        = 360.0*deg;

    // Convert the units into cm
    //fiberRadius      /= cm;
    //fiberCoreRadius  /= cm;
    //fiberCladRadius1 /= cm;
    //fiberCladRadius2 /= cm;


    // Define the solid volume
    G4Tubs* solidFiberCore  = new G4Tubs("solidFiberCore",  0.0,              fiberCoreRadius,  fiberHeight/2.0, startAngle, stopAngle);
    G4Tubs* solidFiberClad1 = new G4Tubs("solidFiberClad1", fiberCoreRadius,  fiberCladRadius1, fiberHeight/2.0, startAngle, stopAngle);
    G4Tubs* solidFiberClad2 = new G4Tubs("solidFiberClad2", fiberCladRadius1, fiberCladRadius2, fiberHeight/2.0, startAngle, stopAngle);


    // Define logical volumes
    G4LogicalVolume* logicFiberCore = new G4LogicalVolume(solidFiberCore,   matWLSCore,     "LogicalFiberCore");
    G4LogicalVolume* logicFiberClad1= new G4LogicalVolume(solidFiberClad1,  matWLSCladIn,   "LogicalFiberCladIn");
    G4LogicalVolume* logicFiberClad2= new G4LogicalVolume(solidFiberClad2,  matWLSCladOut,  "LogicalFiberCladOut");

    //  Assign the scoring volume 
    fScoringVolume = logicFiberCore;

    // Set visualization attributes (optional)
    SetVisualAttributes(logicFiberCore,  "lightblue", 0.10);
    SetVisualAttributes(logicFiberClad1, "lightblue", 0.10);
    SetVisualAttributes(logicFiberClad2, "lightblue", 0.10);

    // Place each fiber around the detector surface
    G4ThreeVector fiberPosVector = G4ThreeVector(0, 0, 0); // Adjust z as needed

    // Place the fiber in the mother volume
    new G4PVPlacement(
            0,                          // No rotation
            fiberPosVector,             // Position of the fiber
            logicFiberCore,             // Logical volume for the fiber
            "CylindricalFiber",         // Name of the physical volume
            logicWorld,                 // Mother volume
            false,                      // No boolean operations
            0,                          // Copy number
            checkOverlaps               // Check for overlaps
            );
    new G4PVPlacement(
            0,                          // No rotation
            fiberPosVector,             // Position of the fiber
            logicFiberClad1,            // Logical volume for the fiber
            "CylindricalFiber",         // Name of the physical volume
            logicWorld,                 // Mother volume
            false,                      // No boolean operations
            0,                          // Copy number
            checkOverlaps               // Check for overlaps
            );
    new G4PVPlacement(
            0,                          // No rotation
            fiberPosVector,             // Position of the fiber
            logicFiberClad2,            // Logical volume for the fiber
            "CylindricalFiber",         // Name of the physical volume
            logicWorld,                 // Mother volume
            false,                      // No boolean operations
            0,                          // Copy number
            checkOverlaps               // Check for overlaps
            );

}   //  ::ConstructWLSFiber


//////////////////////////////////////////////////////////////////////////////////
/// Build cylindrical detector with tyvek coating
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildCylindricalDetectorWithTyvek()
{
    G4bool      checkOverlaps    = true;
    G4double    detRadius        = 2.5 ;
    G4double    detHeight        = 3.0 ;
    G4double    fiberHeight      = 20.0;
    G4double    fiberRadius      = 0.05 ;
    G4double    fiberCoreRadius  = 0.96 * fiberRadius; 
    G4double    fiberCladRadius1 = fiberCoreRadius + 0.02*fiberRadius; 
    G4double    fiberCladRadius2 = fiberCladRadius1 + 0.02*fiberRadius; 
    G4double    startAngle       = 0.0  *deg;
    G4double    stopAngle        = 360.0*deg;

    G4int       nFibers(20);
    G4double    *fiberPosX = nullptr;
    G4double    *fiberPosY = nullptr;

    DefineFiberCoordinates(detRadius, fiberRadius, &nFibers, &fiberPosX, &fiberPosY);

    // Define the solid volume
    G4Tubs* solidCyl        = new G4Tubs("solidCylinderSC", 0.0 *cm,              detRadius *cm,        detHeight/2.0 *cm,   startAngle, stopAngle);
    G4Tubs* solidFiberCore  = new G4Tubs("solidFiberCore",  0.0 *cm,              fiberCoreRadius *cm,  fiberHeight/2.0 *cm, startAngle, stopAngle);
    G4Tubs* solidFiberClad1 = new G4Tubs("solidFiberClad1", fiberCoreRadius *cm,  fiberCladRadius1 *cm, fiberHeight/2.0 *cm, startAngle, stopAngle);
    G4Tubs* solidFiberClad2 = new G4Tubs("solidFiberClad2", fiberCladRadius1 *cm, fiberCladRadius2 *cm, fiberHeight/2.0 *cm, startAngle, stopAngle);


    // Define logical volumes
    G4LogicalVolume* logicDet       = new G4LogicalVolume(solidCyl,         matG3SC,        "logicCylinderSC");
    G4LogicalVolume* logicFiberCore = new G4LogicalVolume(solidFiberCore,   matWLSCore,     "LogicalFiberCore");
    G4LogicalVolume* logicFiberClad1= new G4LogicalVolume(solidFiberClad1,  matWLSCladIn,   "LogicalFiberCladIn");
    G4LogicalVolume* logicFiberClad2= new G4LogicalVolume(solidFiberClad2,  matWLSCladOut,  "LogicalFiberCladOut");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    SetVisualAttributes(logicDet, "grey", 1.0);
    SetVisualAttributes(logicFiberCore,  "lightblue", 0.10);
    SetVisualAttributes(logicFiberClad1, "lightblue", 0.10);
    SetVisualAttributes(logicFiberClad2, "lightblue", 0.10);

    for (int i = 0; i < 5; ++i) 
    {
        // Calculate z position for each detector
        G4double zPositionDetector = (i*detHeight + detHeight/2.0) *cm;

        // Place the detector in the mother volume
        G4ThreeVector positionDetector = G4ThreeVector(0, 0, zPositionDetector);
        new G4PVPlacement(0,                      // no rotation
                positionDetector,       // shifted position for detector
                logicDet,               // logical volume for detector
                "CylindricalSolid",     // name
                logicWorld,             // mother volume
                false,                  // no boolean operations
                i,                      // copy number
                checkOverlaps);         // checking overlaps
    }

    // Place each fiber around the detector surface
    for (int i = 0; i < nFibers; i++)
    {
        // Compute position
        G4ThreeVector fiberPosVector = G4ThreeVector(fiberPosX[i]*cm, fiberPosY[i]*cm, fiberHeight/2.0 * cm); // Adjust z as needed

        // Place the fiber in the mother volume
        new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCore,             // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicWorld,                 // Mother volume
                false,                      // No boolean operations
                i,                          // Copy number
                checkOverlaps               // Check for overlaps
                );
        new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberClad1,            // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicWorld,                 // Mother volume
                false,                      // No boolean operations
                i,                          // Copy number
                checkOverlaps               // Check for overlaps
                );
        new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberClad2,            // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicWorld,                 // Mother volume
                false,                      // No boolean operations
                i,                          // Copy number
                checkOverlaps               // Check for overlaps
                );
    }

    G4double    tyvekThickness = 0.02;      // in cm
    G4double    air_tyvek_gap  = 0.1;       // in cm
    // Create cylindrical solid for tyvek sheets
    G4Tubs* solidTyvek = new G4Tubs("solidTyvek", (detRadius+2*fiberRadius+air_tyvek_gap)*cm, (detRadius+2*fiberRadius+tyvekThickness+air_tyvek_gap)*cm, fiberHeight/2.0 *cm, startAngle, stopAngle);
    // Logical volume for lead sheets

    G4LogicalVolume* logicTyvek = new G4LogicalVolume(solidTyvek, matTyvek, "tyvekSheet");
    SetVisualAttributes(logicTyvek, "red", 0.4);
    
    //G4ThreeVector tyvekPosVector = G4ThreeVector(0*cm, 0*cm, fiberHeight/2.0*cm); // Adjust z as needed
    G4ThreeVector tyvekPosVector = G4ThreeVector(0*cm, 0*cm, fiberHeight/2.0 * cm);
    new G4PVPlacement(0,                  // no rotation
            tyvekPosVector,       // shifted position for lead sheet
            logicTyvek,          // logical volume for lead sheet
            "TyvekSheet",        // name
            logicWorld,         // mother volume
            false,              // no boolean operations
            0,                  // copy number
            checkOverlaps);     // checking overlaps

    //new G4LogicalSkinSurface("TyvekSurface", logicTyvek, tyvekSurface);
    /*  
    */

}   //  ::BuildCylindricalDetectorWithTyvek()


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

    G4Tubs* solidCyl = new G4Tubs("solidCylinderSC", 0, detRadius, detHeight/2.0, startAngle, stopAngle);

    G4LogicalVolume* logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicCylinderSC");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* scVisAttr = new G4VisAttributes();

    scVisAttr->SetColor(G4Colour(176,196,222, 0.5));
    scVisAttr->SetVisibility(true);
    scVisAttr->SetForceSolid(true);
    logicDet->SetVisAttributes(scVisAttr);

    G4PVPlacement(0,                      // no rotation
            G4ThreeVector(0, 0, detHeight/2.0),       // shifted position for detector
            logicDet,               // logical volume for detector
            "CylindricalSolid",     // name
            logicWorld,             // mother volume
            false,                  // no boolean operations
            0,                      // copy number
            checkOverlaps);         // checking overlaps

}   //  ::BuildCylindricalDetector()


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

    G4Tubs* solidCyl = new G4Tubs("solidCylinderSC", 0, detRadius, detHeight/2.0, startAngle, stopAngle);

    // Create cylindrical solid for lead sheets
    G4Tubs* solidLead = new G4Tubs("LeadSheet", 0, detRadius, leadThick/2, startAngle, stopAngle);

    G4LogicalVolume* logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicCylinderSC");

    // Logical volume for lead sheets
    G4LogicalVolume* logicLead = new G4LogicalVolume(solidLead, matPb, "LeadSheet");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

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
    G4Trap *solidDet = new G4Trap("TrapezoidDetector", pDz, pTheta, pPhi, pDy1, pDx1, pDx2, pAlph, pDy2, pDx3, pDx4, pAlph);

    // Build the Logical volume using the solid detector
    G4LogicalVolume* logicDet = new G4LogicalVolume(solidDet, matBC404, "LogicalDetector");

    //  Assign the scoring volume 
    fScoringVolume = logicDet;

    // Set visualization attributes (optional)
    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour::Red());
    logicDet->SetVisAttributes(visAttributes);

    // Building the physical detector
    new G4PVPlacement(0, 
            G4ThreeVector(0., 0., 0.5*m),
            logicDet,
            "physDet",
            logicWorld,
            false,
            0,
            true
            );

    return physWorld;
}


//////////////////////////////////////////////////////////////////////////////////
/// Determine the fiber position along the cylindrical surface
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::DefineFiberCoordinates(G4double cylRadius, G4double fiberRadius, G4int *nFiber, G4double **coordx, G4double **coordy)
{
    // Calculate radius and circumference
    G4double largeRadius = cylRadius + fiberRadius;
    G4double circumference = 2 * CLHEP::pi * largeRadius;

    // Calculate the number of fibers and the opening angle
    *nFiber = static_cast<int>(circumference / (2 * fiberRadius));
    G4double openingAngle = 2 * CLHEP::pi / (*nFiber);

    // Allocate memory for coordinates
    *coordx = new G4double[*nFiber];
    *coordy = new G4double[*nFiber];

    // Compute coordinates
    for (int i = 0; i < *nFiber; ++i)
    {
        (*coordx)[i] = largeRadius * std::cos(openingAngle * i);
        (*coordy)[i] = largeRadius * std::sin(openingAngle * i);

        // Output for debugging
        //G4cout << i << "   " << (*coordx)[i] << "   " << (*coordy)[i] << G4endl;
    }

}   //  ::DefineFiberCoordinates()


//////////////////////////////////////////////////////////////////////////////////
/// Return the detector material
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::DefineMaterial()
{
    matG3SC  = G3ScintillatorMaterial();
    matBC404 = PSD_BC404_Material();
    matNaI   = NaI_Material();

    // Define the elements for the materials
    G4NistManager* nist = G4NistManager::Instance();


    matAir  = nist->FindOrBuildMaterial("G4_AIR");

    std::vector<G4double> photonEnergy = {1.90 *eV, 3.47 *eV};
    std::vector<G4double> refractiveIndex = {1.0, 1.0};

    G4MaterialPropertiesTable *airMPT = new G4MaterialPropertiesTable();
    airMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex);
    matAir->SetMaterialPropertiesTable(airMPT);

    DefineWLSFiberMaterial();


    //-------------------------------------------------------------------------------------------//
    // Initiate Tyvek surface
    matTyvek = new G4Material("matTyvek", 0.92 * g/cm3, 2);
    matTyvek->AddElement(nist->FindOrBuildElement("C"), 2);
    matTyvek->AddElement(nist->FindOrBuildElement("H"), 4);

    tyvekSurface = new G4OpticalSurface("TyvekSurface");
    tyvekSurface->SetType(dielectric_metal);       //  Dielectric 
    tyvekSurface->SetModel(unified);
    tyvekSurface->SetFinish(ground);                  //  Surface finishing type

    // Initiate material properties table
    G4MaterialPropertiesTable *tyvekMPT = new G4MaterialPropertiesTable();

    std::vector<G4double> tyvekPhotonEnergy = {1.9*eV, 3.7*eV};  //  visible photon energy range
    std::vector<G4double> tyvekReflectivity = {0.90, 0.90};      //  Reflectiity of the tyvek surface

    tyvekMPT->AddProperty("REFLECTIVITY", tyvekPhotonEnergy, tyvekReflectivity);
    matTyvek->SetMaterialPropertiesTable(tyvekMPT);
    tyvekSurface->SetMaterialPropertiesTable(tyvekMPT);
    //-------------------------------------------------------------------------------------------//


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

    std::vector<G4double> energySmall = { 2.0 * eV, 3.47 * eV };
    std::vector<G4double> refractiveIndexPS = { 1.58, 1.58 };
    std::vector<G4double> absPS = {2. * m, 2. * m }; 

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
    mptG3SC->AddProperty("RINDEX", energySmall, refractiveIndexPS);
    mptG3SC->AddProperty("ABSLENGTH", energySmall, absPS);
    mptG3SC->AddProperty("SCINTILLATIONCOMPONENT1", abs_energy, scintilFast);
    mptG3SC->AddConstProperty("SCINTILLATIONYIELD", 10. / keV);
    mptG3SC->AddConstProperty("RESOLUTIONSCALE", 1.0); 
    mptG3SC->AddConstProperty("SCINTILLATIONTIMECONSTANT1", 2.1 * ns);
    mptG3SC->AddConstProperty("SCINTILLATIONTIMECONSTANT2", 14.2 * ns);

    // Set the material properties to the material
    scintillator->SetMaterialPropertiesTable(mptG3SC);

    // Set the Birks Constant for the Polystyrene scintillator
    scintillator->GetIonisation()->SetBirksConstant(0.126 * mm / MeV);

    return scintillator;
}   //  ::G3ScintillatorMaterial()


//////////////////////////////////////////////////////////////////////////////////
/// Construct Kuraray double clad WLS fiber
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::DefineWLSFiberMaterial()
{
    // Define the elements for the materials
    G4NistManager* nist = G4NistManager::Instance();

    std::vector<G4double> rEnergies = {1.9*eV, 3.5*eV};

    //Refractive indicies;
    std::vector<G4double> core_rIndex  = {1.59, 1.59};
    std::vector<G4double> clad1_rIndex = {1.49, 1.49};
    std::vector<G4double> clad2_rIndex = {1.42, 1.42};

    //Absorption length
    std::vector<G4double> clad_absL = {3.5 * m, 3.5 * m};
    std::vector<G4double> core_absL = {3.5 * m, 3.5 * m};

    const G4double core_scintilationYield = 8 / keV;
    //const G4double core_YieldRatio = 1.0;
    const G4double core_decayTimeConstant = 2.7 * ns;
    const G4double core_resolutionScale = 1.0;

    /// Emission spectrum of the WLS fiber
    std::vector<G4double> energies = {
        1.908 * eV, 1.928 * eV, 1.958 * eV, 1.979 * eV, 2.013 * eV, 2.038 * eV, 2.072 * eV, 2.107 * eV, 2.148 * eV, 2.197 * eV,
        2.232 * eV, 2.258 * eV, 2.282 * eV, 2.297 * eV, 2.317 * eV, 2.337 * eV, 2.351 * eV, 2.380 * eV, 2.399 * eV, 2.417 * eV,
        2.438 * eV, 2.451 * eV, 2.468 * eV, 2.485 * eV, 2.498 * eV, 2.513 * eV, 2.524 * eV, 2.536 * eV, 2.551 * eV, 2.573 * eV,
        2.588 * eV, 2.600 * eV, 2.611 * eV, 2.621 * eV, 2.634 * eV, 2.643 * eV, 2.648 * eV, 2.665 * eV, 2.683 * eV, 2.700 * eV,
        2.717 * eV, 2.734 * eV, 2.754 * eV, 2.779 * eV, 2.812 * eV, 2.847 * eV, 2.878 * eV
    };

    std::vector<G4double> rel_intensities = {
        0.008, 0.008, 0.013, 0.013, 0.016, 0.022, 0.030, 0.048, 0.068, 0.097,
        0.133, 0.178, 0.236, 0.284, 0.330, 0.370, 0.387, 0.408, 0.451, 0.516,
        0.622, 0.700, 0.817, 0.920, 0.974, 1.000, 0.983, 0.935, 0.860, 0.757,
        0.730, 0.738, 0.757, 0.774, 0.780, 0.773, 0.759, 0.674, 0.523, 0.371,
        0.254, 0.160, 0.090, 0.041, 0.019, 0.012, 0.015
    };

    // Build the WLS fiber core made up of Polystyrene (C8H8)
    //matWLSCore = nist->FindOrBuildMaterial("G4_POLYSTYRENE");
    matWLSCore = new G4Material("matWLSCore", 1.05 * g/cm3, 2);
    matWLSCore->AddElement(nist->FindOrBuildElement("C"), 8);
    matWLSCore->AddElement(nist->FindOrBuildElement("H"), 8);

    G4MaterialPropertiesTable *mptCore   = new G4MaterialPropertiesTable();
    mptCore->AddProperty("RINDEX", rEnergies, core_rIndex);
    mptCore->AddProperty("ABSLENGTH", rEnergies, core_absL);
    mptCore->AddProperty("SCINTILLATIONCOMPONENT1", energies, rel_intensities);
    mptCore->AddConstProperty("SCINTILLATIONYIELD", core_scintilationYield);
    mptCore->AddConstProperty("RESOLUTIONSCALE", core_resolutionScale);
    mptCore->AddConstProperty("SCINTILLATIONTIMECONSTANT1", core_decayTimeConstant);
    matWLSCore->SetMaterialPropertiesTable(mptCore);

    // Build PMMA(C5H8O2) for inner clad
    matWLSCladIn = new G4Material("matWLSCladIn", 1.19*g/cm3, 3);
    matWLSCladIn->AddElement(nist->FindOrBuildElement("C"), 5);
    matWLSCladIn->AddElement(nist->FindOrBuildElement("H"), 8);
    matWLSCladIn->AddElement(nist->FindOrBuildElement("O"), 2);

    G4MaterialPropertiesTable *mptCladIn = new G4MaterialPropertiesTable();
    mptCladIn ->AddProperty("RINDEX", rEnergies, clad1_rIndex);
    mptCladIn ->AddProperty("ABSLENGTH", rEnergies, clad_absL);
    matWLSCladIn->SetMaterialPropertiesTable(mptCladIn);

    // Build F-PEthylene for outer clad
    matWLSCladOut = new G4Material("matWLSCladOut", 1.43*g/cm3, 2);
    matWLSCladOut->AddElement(nist->FindOrBuildElement("C"), 2);
    matWLSCladOut->AddElement(nist->FindOrBuildElement("H"), 4);

    G4MaterialPropertiesTable *mptCladOut= new G4MaterialPropertiesTable();
    mptCladOut->AddProperty("RINDEX", rEnergies, clad2_rIndex);
    mptCladOut->AddProperty("ABSLENGTH", rEnergies, clad_absL);
    matWLSCladOut->SetMaterialPropertiesTable(mptCladOut);

}   //  ::DefineWLSFiberMaterial()


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
    G4double scintillationYield = 10./keV;

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
/// Set visual attributes for detectors
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::SetVisualAttributes(G4LogicalVolume *volume, G4String color, G4double transparency)
{
    G4Colour visColor; // Default value

    if (color == "blue") 
        visColor = G4Colour(0.0, 0.40, 1.0, transparency); 
    else if (color == "grey") 
        visColor = G4Colour(176/255.0, 196/255.0, 222/255.0, transparency);
    else if (color == "red") 
        visColor = G4Colour(1.0, 0.0, 0.0, transparency);
    else if (color == "green") 
        visColor = G4Colour(0.0, 1.0, 0.0, transparency);
    else if (color == "lightgreen") 
        visColor = G4Colour(144/255.0, 238/255.0, 144/255.0, transparency);
    else if (color == "magenta")
        visColor = G4Colour(1.0, 0.0, 1.0, transparency);
    else if (color == "orange")
        visColor = G4Colour(1.0, 0.65, 0.0, transparency);
    else if (color == "lightblue")
        visColor = G4Colour(173/255.0, 216/255.0, 230/255.0, transparency);
    else if (color == "yellow")
        visColor = G4Colour(1.0, 1.0, 0.0, transparency);
    else
        visColor = G4Colour(1.0, 1.0, 1.0, transparency);


    G4VisAttributes* visAttr = new G4VisAttributes(visColor);
    visAttr->SetVisibility(true);
    visAttr->SetForceSolid(true);
    volume->SetVisAttributes(visAttr);

}   //  ::SetVisualAttributes()


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
