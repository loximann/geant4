//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// $Id: F01FieldMessenger.hh,v 1.6 2003/11/25 18:06:22 japost Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
// 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifndef F01FieldMessenger_h
#define F01FieldMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class F01FieldSetup;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;


class F01FieldMessenger: public G4UImessenger
{
  public:
    F01FieldMessenger(F01FieldSetup* );
   ~F01FieldMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    void SetNewValue(G4UIcommand*, G4int);
    
  private:

    F01FieldSetup*             fEMfieldSetup;
    
    G4UIdirectory*             F01detDir;

    G4UIcmdWithAnInteger*      StepperCmd;
    G4UIcmdWithADoubleAndUnit* MagFieldCmd;
    G4UIcmdWithADoubleAndUnit* MinStepCmd;
    G4UIcmdWithoutParameter*   UpdateCmd;

    G4UIcmdWithAString*        AbsMaterCmd;

};

#endif