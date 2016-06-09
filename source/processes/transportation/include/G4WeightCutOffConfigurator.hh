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
// $Id: G4WeightCutOffConfigurator.hh,v 1.4 2003/11/26 14:51:49 gcosmo Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
// ----------------------------------------------------------------------
// Class G4WeightCutOffConfigurator
//
// Class description:
// This class builds and places the G4WeightCutOffProcess.
// If the object is deleted the process is removed from the 
// process list.

// Author: Michael Dressel (Michael.Dressel@cern.ch)
// ----------------------------------------------------------------------
#ifndef G4WeightCutOffConfigurator_hh
#define G4WeightCutOffConfigurator_hh G4WeightCutOffConfigurator_hh

#include "G4Types.hh"
#include "G4VSamplerConfigurator.hh"
#include "G4ProcessPlacer.hh"

class G4WeightCutOffProcess;
class G4VGCellFinder;
class G4VIStore;

class G4WeightCutOffConfigurator : public G4VSamplerConfigurator
{

public:  // with description

  G4WeightCutOffConfigurator(const G4String &particlename,
                             G4double wsurvival,
                             G4double wlimit,
                             G4double isource,
                             G4VIStore *istore,
                             const G4VGCellFinder &aGCellFinder);

  virtual ~G4WeightCutOffConfigurator();
  virtual void Configure(G4VSamplerConfigurator *preConf);
  virtual const G4VTrackTerminator *GetTrackTerminator() const ;
  
private:

  G4WeightCutOffConfigurator(const G4WeightCutOffConfigurator&);
  G4WeightCutOffConfigurator &
  operator=(const G4WeightCutOffConfigurator&);
  G4ProcessPlacer fPlacer;
  G4WeightCutOffProcess *fWeightCutOffProcess;
  G4bool fPlaced;
};

#endif