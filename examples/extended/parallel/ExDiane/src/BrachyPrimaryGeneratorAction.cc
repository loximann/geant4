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
// --------------------------------------------------------------
//                 GEANT 4 - Brachytherapy example
// --------------------------------------------------------------
//
// Code developed by:
// S.Guatelli
//
//    ********************************************
//    *                                          *
//    *    BrachyPrimaryGeneratorAction.cc       *
//    *                                          *
//    ********************************************
//
// $Id: BrachyPrimaryGeneratorAction.cc,v 1.1 2004/05/25 07:32:37 guatelli Exp $
// GEANT4 tag $Name: geant4-07-00-cand-01 $
//
#include "globals.hh"
#include "BrachyPrimaryGeneratorAction.hh"
#include "BrachyAnalysisManager.hh"
#include "G4ParticleTable.hh"
#include "Randomize.hh"  
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4UImanager.hh"
#include "G4RunManager.hh"

BrachyPrimaryGeneratorAction::BrachyPrimaryGeneratorAction()
{;}

BrachyPrimaryGeneratorAction::~BrachyPrimaryGeneratorAction()
{;}

void BrachyPrimaryGeneratorAction::GeneratePrimaries(G4Event*)
{;}