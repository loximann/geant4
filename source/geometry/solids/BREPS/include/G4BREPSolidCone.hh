// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4BREPSolidCone.hh,v 2.1 1998/10/20 16:31:05 broglia Exp $
// GEANT4 tag $Name: geant4-00 $
//
#ifndef __G4BREPSolidCone
#define __G4BREPSolidCone
#include "G4BREPSolid.hh"
//#include "G4Axis2Placement3D.hh"


class G4BREPSolidCone : public G4BREPSolid
{
 public:
  G4BREPSolidCone(G4String,
		  const G4ThreeVector&, 
		  const G4ThreeVector&, 
		  const G4ThreeVector&,		   
		  const G4double,
		  const G4double,
		  const G4double);
  void Initialize();

  EInside Inside(register const G4ThreeVector&) const;
  G4ThreeVector SurfaceNormal(const G4ThreeVector&) const;

  G4double DistanceToIn(const G4ThreeVector&) const;
  G4double DistanceToIn(register const G4ThreeVector&, 
			register const G4ThreeVector&) const;

  G4double DistanceToOut(register const G4ThreeVector&, 
			 register const G4ThreeVector&, 
			 const G4bool calcNorm=false, 
			 G4bool *validNorm=0, G4ThreeVector *n=0) const;
  G4double DistanceToOut(const G4ThreeVector&) const;
};

#endif
















