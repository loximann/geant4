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
// $Id: G4LinInterpolation.hh,v 1.2 2002/05/28 09:15:26 pia Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
// Author: Maria Grazia Pia (Maria.Grazia.Pia@cern.ch)
//
// History:
// -----------
// 31 Jul 2001   MGP        Created
//
// -------------------------------------------------------------------
// Class description:
// Log-Log interpolation of a data set
// Part of a strategy pattern to encapsulate algorithms for interpolation of data sets
// Further documentation available from http://www.ge.infn.it/geant4/lowE/index.html

// -------------------------------------------------------------------

#ifndef G4LININTERPOLATION_HH
#define G4LININTERPOLATION_HH 1

#include "globals.hh"
#include "G4VDataSetAlgorithm.hh"
#include "G4DataVector.hh"

class G4LinInterpolation : public G4VDataSetAlgorithm {
 
public:

  G4LinInterpolation();

  ~G4LinInterpolation();
 
  G4double Calculate(G4double point, G4int bin, 
		     const G4DataVector& energies, 
		     const G4DataVector& data) const;

  G4VDataSetAlgorithm* Clone() const;

private:
  
  // Hide copy constructor and assignment operator

};
 
#endif
 









