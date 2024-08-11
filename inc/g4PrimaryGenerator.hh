//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef     g4PrimaryGenerator_HH
#define     g4PrimaryGenerator_HH
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/*
 *       Filename:  g4PrimaryGenerator.hh
 *    Description:  
 *        Version:  1.0
 *        Created:  07/05/24 05:16:00 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Diptiranjan Pattanaik 
 *   Organization:  
 */
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#include    "G4VUserPrimaryGeneratorAction.hh"
#include    "G4SystemOfUnits.hh"
#include    "G4ParticleGun.hh"
#include    "G4ParticleTable.hh"
#include    "Randomize.hh"
#include    "TRandom3.h"
#include    "G4IonTable.hh"
#include    "G4Proton.hh"
#include    "G4Alpha.hh"
#include    "G4Electron.hh"
#include    "G4Positron.hh"
#include    "G4MuonPlus.hh"
#include    "G4MuonMinus.hh"
#include    "G4Gamma.hh"
#include    "g4Structures.hh"

#define     E_SLOPE 2.5

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
class MyPrimaryGenerator : public G4VUserPrimaryGeneratorAction
{
    public:
                                        MyPrimaryGenerator(ControlFile cnt);        ///< Constructor
                                        ~MyPrimaryGenerator();                      ///< Destructor
        virtual void                    GeneratePrimaries(G4Event *);

    private:
        ControlFile                     Control;                    ///< Control file parameters structures
        G4ParticleGun                   *fParticleGun;              ///< Particle gun 
        char                            particleList[10][10];
        G4double                        GenerateRandomEnergy();
        G4ParticleDefinition            *GetUserParticle(G4String particleName),
                                        *fParticleDef;

};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#endif
