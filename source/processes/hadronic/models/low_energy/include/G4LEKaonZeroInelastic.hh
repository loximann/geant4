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
// $Id: G4LEKaonZeroInelastic.hh,v 1.4 2003/10/31 18:04:15 hpw Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
 // Hadronic Process: Low Energy KaonZeroS Inelastic Process
 // J.L. Chuma, TRIUMF, 12-Feb-1997
 // Last modified: 27-Mar-1997
 
#ifndef G4LEKaonZeroInelastic_h
#define G4LEKaonZeroInelastic_h 1
 
// Class Description
// Final state production model for KaonZeroShost inelastic scattering below 20 GeV; 
// To be used in your physics list in case you need this physics.
// In this case you want to register an object of this class with 
// the corresponding process.
// Class Description - End

#include "G4InelasticInteraction.hh"
 
 class G4LEKaonZeroInelastic : public G4InelasticInteraction
 {
 public:
    
    G4LEKaonZeroInelastic() : G4InelasticInteraction()
    {
      SetMinEnergy( 0.0 );
      SetMaxEnergy( 25.*GeV );
    }
    
    ~G4LEKaonZeroInelastic()
    { }
    
    G4HadFinalState * ApplyYourself(const G4HadProjectile &aTrack,
                                      G4Nucleus &targetNucleus );
    
 private:
    
    void Cascade(                               // derived from CASK0
      G4FastVector<G4ReactionProduct,GHADLISTSIZE> &vec,
      G4int &vecLen,
      const G4HadProjectile *originalIncident,
      G4ReactionProduct &currentParticle,
      G4ReactionProduct &targetParticle,
      G4bool &incidentHasChanged, 
      G4bool &targetHasChanged,
      G4bool &quasiElastic );
    
 };
 
#endif
 