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
/// \file exampleB1.cc
/// \brief Main program of the B1 example

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

using namespace B1;
#include "G4SystemOfUnits.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Event.hh"
#include "G4StepException.hh"
#include "G4TrackingException.hh"
#include "G4EventException.hh"

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void process_exception(const std::exception_ptr& p)
{
    try{
        std::rethrow_exception(p);
    }
    catch(G4StepException& ee) {
        const auto& g4_step = *ee.step;
        std::ostringstream os;
        os << "Step exception detected! \n";
        os << "Kinetic energy (MeV): " << (g4_step.GetPreStepPoint()->GetKineticEnergy() / MeV) << "\n";
        os << "Position X (mm): " << g4_step.GetPostStepPoint()->GetPosition()[0] / mm << "\n";
        os << "Position Y (mm): " << g4_step.GetPostStepPoint()->GetPosition()[1] / mm << "\n";
        os << "Position Z (mm): " << g4_step.GetPostStepPoint()->GetPosition()[2] / mm << "\n";
        std::cout << os.str() << std::endl;
    }
    catch(G4EventException& ee) {
        std::ostringstream os;
        os << "Event exception detected! \n";
        const G4Event& event = *ee.event;
        os << "Seed: \n" << event.GetRandomNumberStatus();
        std::cout << os.str() << std::endl;
    }
    catch(G4TrackException& ee) {
        std::ostringstream os;
        os << "Track exception detected! \n";
        const G4Track& track = *ee.track;
        os << "TrackId: " << track.GetTrackID();
        std::cout << os.str() << std::endl;
    }
    catch(std::exception& e) {
        std::ostringstream os;
        os << "Exception: " << e.what();
        std::cout << os.str() << std::endl;
    }

    try{
        try{
            std::rethrow_exception(p);
        }
        catch(const std::exception& e)
        {
            std::rethrow_if_nested(e);
        }
    }
    catch(...) {
        process_exception(std::current_exception());
    }
}


int main(int argc,char** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //
  G4UIExecutive* ui = nullptr;
  if ( argc == 1 ) { ui = new G4UIExecutive(argc, argv); }

  // Optionally: choose a different Random engine...
  // G4Random::setTheEngine(new CLHEP::MTwistEngine);

  //use G4SteppingVerboseWithUnits
  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

  // Construct the default run manager
  //
  auto* runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

  // Set mandatory initialization classes
  //
  // Detector construction
  runManager->SetUserInitialization(new DetectorConstruction());

  // Physics list
  G4VModularPhysicsList* physicsList = new QBBC;
  physicsList->SetVerboseLevel(1);
  runManager->SetUserInitialization(physicsList);

  // User action initialization
  runManager->SetUserInitialization(new ActionInitialization());

  // Initialize visualization
  //
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  try{
  // Process macro or start UI session
  //
  if ( ! ui ) {
    // batch mode
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }
  else {
    // interactive mode
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    ui->SessionStart();
    delete ui;
  }
  }
  catch(...) {
      process_exception(std::current_exception());
      exit(42);
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

  delete visManager;
  delete runManager;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
