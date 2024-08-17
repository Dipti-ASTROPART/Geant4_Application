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


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
// Inherit the class from the public class "G4VUserDetectorConstruction"
class MyDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
                                            MyDetectorConstruction();           ///< Constructor
                                            ~MyDetectorConstruction();          ///< Destructor
        virtual G4LogicalVolume             *GetScoringVolume() const { return fScoringVolume; } ///< Return the scoring volume
                                            
        virtual G4VPhysicalVolume           *Construct();
    private:                                
        G4Box                               *solidWorld;
        G4LogicalVolume                     *logicWorld;
        G4VPhysicalVolume                   *physWorld;
        G4LogicalVolume                     *fScoringVolume;
                                            
        G4Material                          *NaI_Material(),
                                            *G3ScintillatorMaterial(),
                                            *PSD_BC404_Material();
        G4Material                          *matNaI,
                                            *matAir,
                                            *matG3SC,
                                            *matBC404,
                                            *matPb,
                                            *matWLSCore,
                                            *matWLSCladIn,
                                            *matWLSCladOut,
                                            *matTyvek;
        G4OpticalSurface                    *tyvekSurface;

        G4VPhysicalVolume                   *ConstructTrapezoidalDetector(),
                                            *ConstructG3CylindricalDetector();

        void                                DefineMaterial(),
                                            BuildCylindricalDetector(),
                                            BuildWLSFiber(),
                                            BuildCylindricalDetectorWithTyvek(),
                                            BuildCylindricalDetectorWithLead(),
                                            BuildOpticalSurface(),
                                            BuildMaterial(),
                                            DefineFiberCoordinates(G4double detRadius, G4double fiberRad, G4int *nFibers, G4double** xpos, G4double **ypos),
                                            SetVisualAttributes(G4LogicalVolume *logicDet, G4String color, G4double transparency),
                                            DefineWLSFiberMaterial();
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#endif
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
