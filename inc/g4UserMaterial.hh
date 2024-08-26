/////////////////////////////////////////////////////////////////////////
#ifndef     G4USERMATERIAL_HH
#define     G4USERMATERIAL_HH 1

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#include    "G4Material.hh"
#include    "G4MaterialPropertiesTable.hh" 
#include    "G4NistManager.hh"
#include    "G4SystemOfUnits.hh"
#include    "G4OpticalSurface.hh"
#include    "g4ConfigDetector.hh"


/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
class   MyMaterials
{
    private:
        void                MiscellaneousMaterials();
        void                SetOpticalSurfaces();
        void                ResetMaterialPointers();
        G4Material          *GRAPES_Scintillator_Material();
        G4Material          *WLSFiberCore_Material();
        G4Material          *WLSFiberCladIn_Material();
        G4Material          *WLSFiberCladOut_Material();
    public:
        MyMaterials();
        ~MyMaterials()  {};

        G4Material          *AIR,
                            *VACCUUM,
                            *TYVEK,
                            *G3SC,
                            *WLSCORE,
                            *WLSCLADIN,
                            *WLSCLADOUT,
                            *ALUMINUM;
        G4OpticalSurface    *TYVEK_SURFACE,
                            *CORE_CLAD_SURFACE,
                            *CLAD_CLAD_SURFACE,
                            *CLAD_ENV_SURFACE,
                            *CORE_ENV_SURFACE,
                            *MIRROR_SURFACE,
                            *PHOTON_DET_SURFACE;

};

#endif
 /////////////////////////////////////////////////////////////////////////

