//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#include    "g4DetectorConstruction.hh"

//////////////////////////////////////////////////////////////////////////////////////////
/// Constructor
//////////////////////////////////////////////////////////////////////////////////////////
MyDetectorConstruction::MyDetectorConstruction()
{

}   //  ::MyDetectorConstruction


//////////////////////////////////////////////////////////////////////////////////////////
/// Destructor
//////////////////////////////////////////////////////////////////////////////////////////
MyDetectorConstruction::~MyDetectorConstruction()
{

}   //  ::MyDetectorConstruction


//////////////////////////////////////////////////////////////////////////////////
/// Construct the detector
//////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume   *MyDetectorConstruction::Construct()
{

    return  ConstructG3CylindricalDetector();

}   //  ::Construct


//////////////////////////////////////////////////////////////////////////////////
/// Construct cylindrical detector for the GRAPES-3 test set up
//////////////////////////////////////////////////////////////////////////////////
G4VPhysicalVolume   *MyDetectorConstruction::ConstructG3CylindricalDetector()
{
    G4bool      checkOverlaps = true;

    // Define solid world / world VOLUME
    solidWorld = new G4Box("solidWorld", fWorldXY/2.0, fWorldXY/2.0, fWorldZ/2.0);

    // Logic volume inside the world VOLUME
    logicWorld = new G4LogicalVolume(solidWorld, cMaterial.VACCUUM, "logicWorld");

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

    // Define logical volumes
    solidEnv  = new G4Tubs("solidEnv",  0.0, fCylEnvRadius, fCylEnvHeight/2.0, 0. *deg, 360.0*deg);
    logicEnv = new G4LogicalVolume(solidEnv, cMaterial.AIR,     "logicEnv");
    new G4PVPlacement(
            0,                          // No rotation
            G4ThreeVector(0,0,0),             // Position of the fiber
            logicEnv,             // Logical volume for the fiber
            "CylindricalEnv",         // Name of the physical volume
            logicWorld,                 // Mother volume
            false,                      // No boolean operations
            0,                          // Copy number
            checkOverlaps               // Check for overlaps
            );
    SetVisualAttributes(logicEnv, "white", 0.001);

    BuildSolidCylindricalDetector();
    //BuildCylindricalDetectorWithTyvek();

    return physWorld;
}   //  :: ConstructG3CylindricalDetector()


//////////////////////////////////////////////////////////////////////////////////
/// Build cylindrical detector with tyvek coating
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildSolidCylindricalDetector()
{
    G4bool      checkOverlaps    = true;

    //  Determine the number of fibers required to lay on the surface and the coorinate
    G4int       nFibers(1);
    G4double    *fiberPosX = nullptr;
    G4double    *fiberPosY = nullptr;
    DefineFiberCoordinates(fCylRadius, fWLSFiberRadius, &nFibers, &fiberPosX, &fiberPosY);

    // Define the solid volume
    G4Tubs* solidCyl          = new G4Tubs("solidCylinderSC", 0.0, fCylRadius, fCylHeight/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicDet = new G4LogicalVolume(solidCyl, cMaterial.G3SC, "logicDet");
    sScoringVolumes.primaryDetector = logicDet;                 ///< Assign scoring volumes
    SetVisualAttributes(logicDet, "blue", 1.00);    ///< Set visualization attributes (optional)

    // Build tyvek sheet in between detectors
    G4Tubs* solidTyvekSheet          = new G4Tubs("solidTyvekSheet", 0.0, fCylRadius, fTyvekThickness/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekSheet = new G4LogicalVolume(solidTyvekSheet, cMaterial.TYVEK, "logicTyvekSheet");
    SetVisualAttributes(logicTyvekSheet, "red", 0.6);

    G4ThreeVector positionDetector = G4ThreeVector(0, 0, 0);
    new G4PVPlacement(0,                      // no rotation
            positionDetector,       // shifted position for detector
            logicDet,               // logical volume for detector
            "CylindricalSolid",     // name
            logicEnv,               // mother volume
            false,                  // no boolean operations
            0,                      // copy number
            checkOverlaps);         // checking overlaps

    //new G4LogicalSkinSurface("logicalDetSkinOpSurface", logicDet, detOpSurface);

    //---------- Build WLS fibers ----------------//
    
    // Fiber core
    G4Tubs* solidFiberCore = new G4Tubs("solidFiberCore", 0.0 , fWLSFiberCoreRadius, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCore = new G4LogicalVolume(solidFiberCore, cMaterial.WLSCORE, "logicFiberCore");

    // Inner clad
    G4Tubs* solidFiberCladIn = new G4Tubs("solidFiberCladIn", fWLSFiberCoreRadius, fWLSFiberCladRadiusIn, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCladIn  = new G4LogicalVolume(solidFiberCladIn, cMaterial.WLSCLADIN, "logicFiberCladIn");

    // Outer clad
    G4Tubs* solidFiberCladOut = new G4Tubs("solidFiberCladOut", fWLSFiberCladRadiusIn, fWLSFiberCladRadiusOut, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCladOut = new G4LogicalVolume(solidFiberCladOut, cMaterial.WLSCLADOUT, "logicFiberCladOut");

    sScoringVolumes.fiberCore    = logicFiberCore;
    sScoringVolumes.fiberCladIn  = logicFiberCladIn;
    sScoringVolumes.fiberCladOut = logicFiberCladOut;

    // Set visual attributes for the WLS fibers
    SetVisualAttributes(logicFiberCore,    "lightgreen", 0.15);
    SetVisualAttributes(logicFiberCladIn,  "lightgreen", 0.10);
    SetVisualAttributes(logicFiberCladOut, "lightgreen", 0.05);

    G4VPhysicalVolume *physWLSCore    = nullptr;
    G4VPhysicalVolume *physWLSCladIn  = nullptr;
    G4VPhysicalVolume *physWLSCladOut = nullptr;

    // Place each fiber around the detector surface
    for (int iFiber = 0; iFiber < nFibers; iFiber++)
    {
        // Compute position
        G4ThreeVector fiberPosVector = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber], -(fNCylinders*fCylHeight)/2.0 + fWLSFiberLength/2.0); // Adjust z as needed

        // Place the fiber in the logical environment
        physWLSCore =  new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCore,             // Logical volume for the fiber
                "phyFiberCore",         // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );
        physWLSCladIn = new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCladIn,           // Logical volume for the fiber
                "phyFiberCladIn",           // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );
        physWLSCladOut = new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCladOut,          // Logical volume for the fiber
                "physFiberCladOut",         // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );

        new G4LogicalBorderSurface("CoreCladSurface", physWLSCore,      physWLSCladIn,  cMaterial.CORE_CLAD_SURFACE);
        new G4LogicalBorderSurface("CladCladSurface", physWLSCladIn,    physWLSCladOut, cMaterial.CLAD_CLAD_SURFACE);
        new G4LogicalBorderSurface("CladAirSurface",  physWLSCladOut,   physEnv,        cMaterial.CLAD_ENV_SURFACE);
        new G4LogicalBorderSurface("CoreAirSurface",  physWLSCore,      physEnv,        cMaterial.CORE_ENV_SURFACE);
        new G4LogicalBorderSurface("CoreCladSurface", physWLSCladIn,    physWLSCore,    cMaterial.CORE_CLAD_SURFACE);
        new G4LogicalBorderSurface("CladCladSurface", physWLSCladOut,   physWLSCladIn,  cMaterial.CLAD_CLAD_SURFACE);
        new G4LogicalBorderSurface("CladAirSurface",  physEnv,          physWLSCladOut, cMaterial.CLAD_ENV_SURFACE);
        new G4LogicalBorderSurface("CoreAirSurface",  physEnv,          physWLSCore,    cMaterial.CORE_ENV_SURFACE);

    }   // i < nFibers

    G4double    sensDetPosZ = -(fNCylinders*fCylHeight)/2.0;

    // Create cylindrical  tyvek sheets for optical photon reflection
    G4double    tyvekInnerRadius  = fCylRadius+ 2*fWLSFiberRadius + fAir_Tyvek_Gap;
    G4double    tyvekOuterRadius  = tyvekInnerRadius + fTyvekThickness;

    G4double    tyvekCoatLeftEnd  = -(fNCylinders*fCylHeight)/2.0 + fTyvekCoatLength/2 - 2*mm;
    G4ThreeVector tyvekCoatPosVector = G4ThreeVector(0, 0, tyvekCoatLeftEnd);

    G4Tubs* solidTyvekCoat          = new G4Tubs("solidTyvek", tyvekInnerRadius, tyvekOuterRadius, fTyvekCoatLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekCoat = new G4LogicalVolume(solidTyvekCoat, cMaterial.TYVEK, "logicTyvekCoat");
    new G4PVPlacement(0, tyvekCoatPosVector, logicTyvekCoat, "physTyvekCoat", logicEnv, false, 0, checkOverlaps);             
    new G4LogicalSkinSurface("tyvekSkinSurface", logicTyvekCoat, cMaterial.TYVEK_SURFACE);
    SetVisualAttributes(logicTyvekCoat, "white", 0.05);

    // Set the tyvel refector cap at one end

    G4Tubs* solidTyvekCap           = new G4Tubs("solidTyvekCap", 0, tyvekOuterRadius, fTyvekThickness/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekCap  = new G4LogicalVolume(solidTyvekCap, cMaterial.TYVEK, "logicTyvekCap");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -fCylHeight/2-2*mm), logicTyvekCap, "TyvekCap", logicEnv, false, 0, checkOverlaps);
    new G4LogicalSkinSurface("tyvekCapSurface", logicTyvekCap, cMaterial.TYVEK_SURFACE);
    SetVisualAttributes(logicTyvekCap, "white", 0.05);

    //----------------------------------------------------------------------// 
    //--------- Attach sensitive detectors at the end of WLS fiber----------// 
    //----------------------------------------------------------------------// 
    G4Tubs* solidFiberSD1 = new G4Tubs("solidFiberSD1", 0.0 , fWLSFiberRadius, 0.5*mm, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberSD1 = new G4LogicalVolume(solidFiberSD1, cMaterial.ALUMINUM, "logicFiberSD1");

    G4Tubs* solidFiberSD2 = new G4Tubs("solidFiberSD2", 0.0 , fWLSFiberRadius, 0.5*mm, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberSD2 = new G4LogicalVolume(solidFiberSD2, cMaterial.ALUMINUM, "logicFiberSD2");

    for (int iFiber = 0; iFiber < nFibers; iFiber++)
    {
        // Compute position of the fibers
        G4ThreeVector fiberSDPos1 = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber], sensDetPosZ-0.005*mm); // Adjust z as needed
        G4ThreeVector fiberSDPos2 = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber], sensDetPosZ+fWLSFiberLength+0.005*mm); // Adjust z as needed

        new G4PVPlacement(0, fiberSDPos1, logicFiberSD1, "physFiberSD1", logicEnv, false, 0, checkOverlaps);
        new G4PVPlacement(0, fiberSDPos2, logicFiberSD2, "physFiberSD2", logicEnv, false, 0, checkOverlaps);

        // Provide surface material properties of the sensitive detector
        new G4LogicalSkinSurface("PhotonDetSurface1", logicFiberSD1, cMaterial.PHOTON_DET_SURFACE);
        new G4LogicalSkinSurface("PhotonDetSurface2", logicFiberSD2, cMaterial.PHOTON_DET_SURFACE);
    }

    // Attach a sensitive detector just below the scintillator detector
    G4double fSCSDThickness = 0.5 * mm;
    G4Tubs* solidSCSD           = new G4Tubs("solidSCSD", 0, fCylRadius, fSCSDThickness, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicSCSD  = new G4LogicalVolume(solidSCSD, cMaterial.ALUMINUM, "logicSCSD");
    new G4PVPlacement(0, G4ThreeVector(0, 0, (fNCylinders*fCylHeight)/2.0 + fSCSDThickness+0.005*mm), logicSCSD, "physSCSD", logicEnv, false, 0, checkOverlaps);
    new G4LogicalSkinSurface("PhotonDetSurface0", logicSCSD, cMaterial.PHOTON_DET_SURFACE);

    SetVisualAttributes(logicSCSD,     "magenta", 1.0);
    SetVisualAttributes(logicFiberSD1, "magenta", 1.0);
    SetVisualAttributes(logicFiberSD2, "magenta", 1.0);

}   //  ::BuildCylindricalDetectorWithTyvek()


//////////////////////////////////////////////////////////////////////////////////
/// Build cylindrical detector with tyvek coating
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::BuildCylindricalDetectorWithTyvek()
{
    /*  
    G4bool      checkOverlaps    = true;

    //  Determine the number of fibers required to lay on the surface and the coorinate
    G4int       nFibers(1);
    G4double    *fiberPosX = nullptr;
    G4double    *fiberPosY = nullptr;
    DefineFiberCoordinates(fCylRadius, fWLSFiberRadius, &nFibers, &fiberPosX, &fiberPosY);

    // Define the solid volume
    G4Tubs* solidCyl          = new G4Tubs("solidCylinderSC", 0.0, fCylRadius, fCylHeight/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicDet = new G4LogicalVolume(solidCyl, matG3SC, "logicDet");
    sScoringVolumes.primaryDetector = logicDet;                 ///< Assign scoring volumes
    SetVisualAttributes(logicDet,        "grey",      1.00);    ///< Set visualization attributes (optional)

    // Build tyvek sheet in between detectors
    G4Tubs* solidTyvekSheet          = new G4Tubs("solidTyvekSheet", 0.0, fCylRadius, fTyvekThickness/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekSheet = new G4LogicalVolume(solidTyvekSheet, matTyvek, "logicTyvekSheet");
    SetVisualAttributes(logicTyvekSheet, "red", 0.6);

    // Physical placement of the volumes
    for (int i = 0; i < 5; ++i) 
    {
        // Calculate z position for each detector
        G4double zPositionDetector = i * (fCylHeight + fTyvekThickness) + fCylHeight/2.0;
        G4ThreeVector positionDetector = G4ThreeVector(0, 0, zPositionDetector);
        new G4PVPlacement(0,                      // no rotation
                positionDetector,       // shifted position for detector
                logicDet,               // logical volume for detector
                "CylindricalSolid",     // name
                logicEnv,               // mother volume
                false,                  // no boolean operations
                i,                      // copy number
                checkOverlaps);         // checking overlaps

        // Set the reflecting coat
        G4double zPositionLead = (i+1) * fCylHeight + (i+0.5) * fTyvekThickness;
        G4ThreeVector positionLead = G4ThreeVector(0, 0, zPositionLead);

        new G4PVPlacement(0,                  // no rotation
                positionLead,       // shifted position for lead sheet
                logicTyvekSheet,          // logical volume for lead sheet
                "LeadSheet",        // name
                logicEnv,           // mother volume
                false,              // no boolean operations
                i,                  // copy number
                checkOverlaps);     // checking overlaps
    }

    //---------- Build WLS fibers ----------------//
    
    // Fiber core
    G4Tubs* solidFiberCore = new G4Tubs("solidFiberCore", 0.0 , fWLSFiberCoreRadius, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCore = new G4LogicalVolume(solidFiberCore, matWLSCore, "logicFiberCore");

    // Inner clad
    G4Tubs* solidFiberCladIn = new G4Tubs("solidFiberCladIn", fWLSFiberCoreRadius, fWLSFiberCladRadiusIn, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCladIn  = new G4LogicalVolume(solidFiberCladIn, matWLSCladIn, "logicFiberCladIn");

    // Outer clad
    G4Tubs* solidFiberCladOut = new G4Tubs("solidFiberCladOut", fWLSFiberCladRadiusIn, fWLSFiberCladRadiusOut, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberCladOut = new G4LogicalVolume(solidFiberCladOut, matWLSCladOut, "logicFiberCladOut");

    sScoringVolumes.fiberCore    = logicFiberCore;
    sScoringVolumes.fiberCladIn  = logicFiberCladIn;
    sScoringVolumes.fiberCladOut = logicFiberCladOut;

    // Set visual attributes for the WLS fibers
    SetVisualAttributes(logicFiberCore,    "lightblue", 0.40);
    SetVisualAttributes(logicFiberCladIn,  "lightblue", 0.30);
    SetVisualAttributes(logicFiberCladOut, "lightblue", 0.10);

    G4VPhysicalVolume *physWLSCore    = nullptr;
    G4VPhysicalVolume *physWLSCladIn  = nullptr;
    G4VPhysicalVolume *physWLSCladOut = nullptr;

    // Place each fiber around the detector surface
    for (int iFiber = 0; iFiber < nFibers; iFiber++)
    {
        // Compute position
        G4ThreeVector fiberPosVector = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber], fWLSFiberLength/2.0); // Adjust z as needed

        // Place the fiber in the logical environment
        physWLSCore =  new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCore,             // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );
        physWLSCladIn = new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCladIn,           // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );
        physWLSCladOut = new G4PVPlacement(
                0,                          // No rotation
                fiberPosVector,             // Position of the fiber
                logicFiberCladOut,          // Logical volume for the fiber
                "CylindricalFiber",         // Name of the physical volume
                logicEnv,                   // Mother volume
                false,                      // No boolean operations
                iFiber,                     // Copy number
                checkOverlaps               // Check for overlaps
                );

        //new G4LogicalSkinSurface("fiberEndSkinSurface", logicFiberCore, fiberEndOpSurface);
    }   // i < nFibers

    // Create cylindrical  tyvek sheets for optical photon reflection
    G4double    tyvekInnerRadius  = fCylRadius+ 2*fWLSFiberRadius + fAir_Tyvek_Gap;
    G4double    tyvekOuterRadius  = tyvekInnerRadius + fTyvekThickness;

    G4Tubs* solidTyvekCoat          = new G4Tubs("solidTyvek", tyvekInnerRadius, tyvekOuterRadius, fWLSFiberLength/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekCoat = new G4LogicalVolume(solidTyvekCoat, matTyvek, "logicTyvekCoat");
    new G4PVPlacement(0, G4ThreeVector(0, 0, fWLSFiberLength/2.0), logicTyvekCoat, "physTyvekCoat", logicEnv, false, 0, checkOverlaps);             
    SetVisualAttributes(logicTyvekCoat, "yellow", 0.05);

    // Set the tyvel refector cap at one end
    G4Tubs* solidTyvekCap           = new G4Tubs("solidTyvekCap", 0, tyvekOuterRadius, fTyvekThickness/2.0, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicTyvekCap  = new G4LogicalVolume(solidTyvekCap, matTyvek, "logicTyvekCap");
    new G4PVPlacement(0, G4ThreeVector(0, 0, -fTyvekThickness/2.0), logicTyvekCap, "TyvekCap", logicEnv, false, 0, checkOverlaps);
    SetVisualAttributes(logicTyvekCap, "yellow", 0.05);

    //--------- Attach sensitive detectors at the end of WLS fiber----------// 
    G4Tubs* solidFiberSD1 = new G4Tubs("solidFiberSD1", 0.0 , fWLSFiberRadius, 0.005*mm, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberSD1 = new G4LogicalVolume(solidFiberSD1, matWLSCore, "logicFiberSD1");

    G4Tubs* solidFiberSD2 = new G4Tubs("solidFiberSD2", 0.0 , fWLSFiberRadius, 0.005*mm, fCylStartAngle, fCylStopAngle);
    G4LogicalVolume* logicFiberSD2 = new G4LogicalVolume(solidFiberSD2, matWLSCore, "logicFiberSD2");

    for (int iFiber = 0; iFiber < nFibers; iFiber++)
    {
        // Compute position
        G4ThreeVector fiberSDPos1 = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber]-0.01*cm, 0); // Adjust z as needed
        G4ThreeVector fiberSDPos2 = G4ThreeVector(fiberPosX[iFiber], fiberPosY[iFiber], fWLSFiberLength+.01*cm); // Adjust z as needed

        new G4PVPlacement(0, fiberSDPos1, logicFiberSD1, "physFiberSD1", logicEnv, false, 0, checkOverlaps);
        new G4PVPlacement(0, fiberSDPos2, logicFiberSD2, "physFiberSD2", logicEnv, false, 0, checkOverlaps);
    }

    SetVisualAttributes(logicFiberSD1, "red", 1.0);
    SetVisualAttributes(logicFiberSD2, "red", 1.0);

    
    //new G4LogicalSkinSurface("TyvekSurface", logicTyvek, tyvekSurface);
*/
}   //  ::BuildCylindricalDetectorWithTyvek()


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
/// Construct the sensitive detectors
//////////////////////////////////////////////////////////////////////////////////
void    MyDetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    // Create and register the sensitive detector
    MySensitiveDetector *mySD1= new MySensitiveDetector("mySD1");
    sdManager->AddNewDetector(mySD1);
    // Attach sensitive detectors
    SetSensitiveDetector("logicFiberSD1", mySD1);

    MySensitiveDetector *mySD2= new MySensitiveDetector("mySD2");
    sdManager->AddNewDetector(mySD2);
    SetSensitiveDetector("logicFiberSD2", mySD2);

    MySensitiveDetector *mySCSD = new MySensitiveDetector("mySCSD");
    sdManager->AddNewDetector(mySCSD);
    SetSensitiveDetector("logicSCSD", mySCSD);

}   //  ::ConstructSDandField


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
