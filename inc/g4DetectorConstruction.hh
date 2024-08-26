/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#ifndef g4DetectorConstruction_HH
#define g4DetectorConstruction_HH

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
#include    "G4VUserDetectorConstruction.hh"
#include    "G4VPhysicalVolume.hh"
#include    "G4NistManager.hh"
#include    "G4Material.hh"
#include    "G4LogicalVolume.hh"
#include    "G4PVPlacement.hh"
#include    "G4SystemOfUnits.hh"
#include    "G4Box.hh"
#include    "G4Trap.hh"
#include    "G4Tubs.hh"
#include    "G4Colour.hh"
#include    "G4VisAttributes.hh"
#include    "G4OpticalSurface.hh"
#include    "G4LogicalSkinSurface.hh"
#include    "G4LogicalBorderSurface.hh"
#include    "G4Cons.hh"
#include    "G4Trd.hh"
#include    "G4SubtractionSolid.hh"
#include    "G4SDManager.hh"
#include    "g4Structures.hh"
#include    "g4ConfigDetector.hh"
#include    "g4SensitiveDetector.hh"
#include    "g4UserMaterial.hh"

/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// Inherit the class from the public class "G4VUserDetectorConstruction"
class MyDetectorConstruction : public G4VUserDetectorConstruction
{ 

    protected:
        ScoringVolumes_Struct               sScoringVolumes;
    public:
        MyDetectorConstruction();           ///< Constructor
        ~MyDetectorConstruction();          ///< Destructor

        virtual ScoringVolumes_Struct       GetScoringVolume() const { return sScoringVolumes; } ///< Return the scoring volume
        virtual G4VPhysicalVolume           *Construct();
        virtual void                        ConstructSDandField() override;

    private:                                
        G4Box                               *solidWorld;
        G4Tubs                              *solidEnv;
        G4LogicalVolume                     *logicWorld,
                                            *logicEnv;
        G4VPhysicalVolume                   *physWorld,
                                            *physEnv;
        MyMaterials                         cMaterial;
        G4VPhysicalVolume                   *ConstructG3CylindricalDetector();
        void                                BuildCylindricalDetectorWithTyvek(),
                                            BuildSolidCylindricalDetector(),
                                            DefineFiberCoordinates(G4double detRadius, G4double fiberRad, G4int *nFibers, G4double** xpos, G4double **ypos),
                                            SetVisualAttributes(G4LogicalVolume *logicDet, G4String color, G4double transparency);
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#endif
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
