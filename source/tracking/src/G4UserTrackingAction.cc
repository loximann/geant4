// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4UserTrackingAction.cc,v 2.0 1998/07/02 17:29:34 gunter Exp $
// GEANT4 tag $Name: geant4-00 $
//
//
// ---------------------------------------------------------------
//
// G4UserTrackingAction.cc
//
// Contact:
//   Questions and comments to this code should be sent to
//     Katsuya Amako  (e-mail: Katsuya.Amako@kek.jp)
//     Takashi Sasaki (e-mail: Takashi.Sasaki@kek.jp)
//
// ---------------------------------------------------------------

#include "G4UserTrackingAction.hh"

/////////////////////////////////////////////////////////
void G4UserTrackingAction::
     SetTrackingManagerPointer(G4TrackingManager* pValue)
/////////////////////////////////////////////////////////
{
  fpTrackingManager = pValue;
}  







