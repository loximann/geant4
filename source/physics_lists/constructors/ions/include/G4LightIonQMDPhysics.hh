//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//---------------------------------------------------------------------------
//
// ClassName:    G4LightIonQMDPhysics
// 
// Author:       Yoshihide Sato - November 2023
//
// Description:  Variant of G4IonQMDPhysics in which G4LightIonQMDReaction
//               is used in the interval [30 MeV, 510 MeV]/nucleon
//               (G4BinaryLightIonReaction is used below 40 MeV/nucleon,
//                and G4QMDReaction is used in the interval
//                [500 MeV, 6 GeV]/nucleon).
//
// Modified:
//
//----------------------------------------------------------------------------
//

#ifndef G4LightIonQMDPhysics_h
#define G4LightIonQMDPhysics_h 1

#include "globals.hh"
#include "G4VPhysicsConstructor.hh"

#include <vector>

class G4HadronicInteraction;
class G4BinaryLightIonReaction;
class G4QMDReaction;
class G4LightIonQMDReaction;
class G4VCrossSectionDataSet;

class G4LightIonQMDPhysics : public G4VPhysicsConstructor
{
public:
  G4LightIonQMDPhysics(G4int verb = 0);
  G4LightIonQMDPhysics(const G4String& name, G4int ver = 0);
  ~G4LightIonQMDPhysics() override;

  // This method will be invoked in the Construct() method.
  // each particle type will be instantiated
  void ConstructParticle() override;

  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type
  void ConstructProcess() override;

private:

  void AddProcess(const G4String&,
		  G4ParticleDefinition*, 
		  G4BinaryLightIonReaction*,
          G4QMDReaction*,
		  G4LightIonQMDReaction*,
		  G4HadronicInteraction*,
		  G4VCrossSectionDataSet*);

  G4double eminQMD;
  G4double emaxQMD;
  G4double eminLIQMD;
  G4double emaxLIQMD;
  G4double overlap;
   
  G4int  verbose;
};


#endif

