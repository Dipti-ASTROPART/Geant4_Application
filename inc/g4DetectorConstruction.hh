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
<<<<<<< HEAD
#include    "G4LogicalBorderSurface.hh"
=======
>>>>>>> 671b135 (Minor update to the detector construction class)


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
        G4Trap                              *solidDet;
        G4Tubs                              *solidCyl;
        G4LogicalVolume                     *logicWorld,
                                            *logicDet;
        G4VPhysicalVolume                   *physWorld,
                                            *physDet;
<<<<<<< HEAD
        G4Material                          *detMat,
                                            *worldMat;
=======
>>>>>>> 671b135 (Minor update to the detector construction class)
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

        G4VPhysicalVolume                   *ConstructTrapezoidalDetector(),
                                            *ConstructG3CylindricalDetector();

<<<<<<< HEAD
        G4LogicalSkinSurface                *tyvekSurface;

=======
>>>>>>> 671b135 (Minor update to the detector construction class)
        void                                DefineMaterial(),
                                            BuildCylindricalDetector(),
                                            BuildCylindricalDetectorWithTyvek(),
                                            BuildCylindricalDetectorWithLead(),
                                            BuildOpticalSurface(),
                                            BuildMaterial(),
                                            ConstructFiberMaterial();
};


/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////

#endif
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
