#ifndef     g4ActionInitialize_HH
#define     g4ActionInitialize_HH
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
/*
 *       Filename:  g4ActionInitialize.hh
 *    Description:  Action initialization for the Geatnt4. Build the class inheriting 
 *                  from "G4VUserActionInitialization" class
 *        Version:  1.0
 *        Created:  07/05/24 04:29:33 PM IST
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  Diptiranjan 
 *   Organization:  INDEPENDET 
 */
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

#include    "G4VUserActionInitialization.hh"
#include    "g4PrimaryGenerator.hh"
#include    "g4RunAction.hh"
#include    "g4EventAction.hh"
#include    "g4SteppingAction.hh"
#include    "g4Structures.hh"


class MyActionInitialization : public G4VUserActionInitialization
{
    public:
                                MyActionInitialization(ControlFile cnt);           ///<    Constructor
                                ~MyActionInitialization();          ///<    Destructor
        virtual void            BuildForMaster() const;
        virtual void            Build() const;
    private:
        ControlFile             Control;
};


#endif
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
