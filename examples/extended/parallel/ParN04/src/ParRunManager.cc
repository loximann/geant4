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
#ifdef G4USE_TOPC

#include "G4Timer.hh"
#include "G4RunManager.hh"
#include "Randomize.hh"
#include "G4Run.hh"
#include "G4RunMessenger.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4UserRunAction.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4TransportationManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ApplicationState.hh"
#include "G4StateManager.hh"
#include "G4VPersistencyManager.hh"
#include "G4UImanager.hh"
#include "G4ParticleTable.hh"
#include "G4ProcessTable.hh"
#include "G4UnitsTable.hh"
#include "G4VVisManager.hh"

#include "ParRunManager.hh"
//vietha, include file for marshaling structures
#include "MarshaledG4HCofThisEvent.h"

#include "G4ios.hh"
#include <strstream>


/*
 * The TopC call backs are implemented by using the 5 static data members to store
 * the data and using the 3 static member functions to point to the corresponding
 * functions of the singleton class instance.
 */
G4StateManager*     ParRunManager::stateManager;
G4int               ParRunManager::n_event;
G4int               ParRunManager::n_select;
G4String            ParRunManager::msg;
ParRunManager*      ParRunManager::myRunManager = 0;

//cross-context variables
static long* g_Seeds;

// TOP-C callbacks functions
TOPC_BUF ParRunManager::MyDoEvent( void *input_buf ) { 
   return myRunManager->DoEvent(input_buf); 
}
TOPC_ACTION ParRunManager::MyCheckEventResult( void * input_buf, void *buf ) { 
   return myRunManager->CheckEventResult(input_buf, buf); 
}


static void trace_event_input( void *input ) { 
  //G4cout << "Event " << *(G4int *)input << G4endl; 
}

/*
 * DoEventLoop is the most important piece in RunManager class to control the program runs,
 * this is where we embed the TopC parallelism callbacks.
 */
void ParRunManager::DoEventLoop( G4int n_event, const char* macroFile, G4int n_select )
{
  TOPC_OPT_trace_input = trace_event_input;

  G4StateManager* stateManager = G4StateManager::GetStateManager();

#ifdef G4_ORIGINAL
  cout << "ParRunManager::DoEventLoop" << endl;
  G4RunManager::DoEventLoop(n_event, macroFile, n_select);
  return;
#endif

  if ( verboseLevel > 0 ) { timer->Start(); }
 

  G4String msg;
  if ( macroFile != 0 ) { 
    if ( n_select < 0 ) n_select = n_event;
    msg = "/control/execute ";
    msg += macroFile;
  } else { 
    n_select = -1; 
  }

  // BeginOfEventAction() and EndOfEventAction() would normally be
  // called inside G4EventManager::ProcessOneEvent() in ParRunManager::DoEvent
  // on slave.  Since this is often where hits are collected and where
  // histogram data is first stored, must do it
  // on master instead, to process hits.  So, set user event action to NULL.
  // Keep private copy, and execute it in CheckTaskResult().
  // If user later does:  SetUserAction( (G4UserEventAction *)NULL );
  // we won't notice and copy it to origUserEventAction.
  if ( eventManager->GetUserEventAction() ) {
    origUserEventAction = eventManager->GetUserEventAction();
    SetUserAction( (G4UserEventAction *)0 );
  }

  // Make these variables accessible to TOP-C callback functions
  ImportDoEventLoopLocals( stateManager, n_event, n_select, msg );


  // Setup random seeds for each slave
  g_Seeds = (long*)calloc(n_event, sizeof(long));

  G4int i_event;
  for( i_event=0; i_event<n_event; i_event++ )
    {
      g_Seeds[i_event] = (long) (100000000L * HepRandom::getTheGenerator()->flat());
    }

  // This is where all the parallelism occurs
  TOPC_raw_begin_master_slave(MyDoEvent, MyCheckEventResult, NULL);
  if(TOPC_is_master()){
    G4int i_event;
    for( i_event=0; i_event<n_event; i_event++ )
      {
		TOPC_raw_submit_task_input(TOPC_MSG( &i_event, sizeof(G4int)));
		if (runAborted) break;
      }	
  }	
  TOPC_raw_end_master_slave();

  free(g_Seeds);

  if ( verboseLevel > 0 ) {
    timer->Stop();
    G4cout << "Run terminated." << G4endl;
    G4cout << "Run Summary" << G4endl;
    if ( runAborted ) { 
	  G4cout << "  Run Aborted." << G4endl; 
	} else { 
	  G4cout << "  Number of events processed : " << n_event << G4endl; 
	}
    G4cout << "  "  << *timer << G4endl;
  }
}

static  MarshaledG4HCofThisEvent *aMarshaledObj = NULL;

TOPC_BUF ParRunManager::DoEvent( void *input_buf )
{
  static 
  G4int i_event;
  memcpy(&i_event, input_buf, sizeof(G4int));
  HepRandom::setTheSeed(g_Seeds[i_event]);

  // removed for Geant4.6
  //stateManager->SetNewState( G4State_EventProc );

  currentEvent = GenerateEvent( i_event );
  eventManager->ProcessOneEvent( currentEvent );

  G4HCofThisEvent* HCE = currentEvent->GetHCofThisEvent();

  if(aMarshaledObj) delete aMarshaledObj;

  aMarshaledObj = new MarshaledG4HCofThisEvent(HCE);
  //stateManager->SetNewState( G4State_GeomClosed );

  StackPreviousEvent( currentEvent );
  currentEvent = 0;
  return TOPC_MSG( aMarshaledObj->getBuffer(), aMarshaledObj->getBufferSize());
}



TOPC_ACTION ParRunManager::CheckEventResult( void * input_buf, void *output_buf )
{
  G4int i_event;
  memcpy(&i_event, input_buf, sizeof(G4int));

  // removed for Geant4.6
  //stateManager->SetNewState(G4State_EventProc);
  // Geant4.6 requires the state to be G4State_GeomClosed
  // before calling EventManager::ProcessOneEvent(..)
  
  if ( !userPrimaryGeneratorAction )
    G4Exception("G4RunManager::BeamOn - G4VUserPrimaryGeneratorAction is not defined.");

  //This creates a trivial event in lieu of GenerateEvent(i_event);
  currentEvent = new G4Event( i_event );

  // 
  SetUserAction( (G4UserEventAction *)0 );

  // When Geant4 4.0 sees empty event, it still calls userStackingAction.
  // On master, only trivial events exist, so we delete userStackingAction
  SetUserAction( (G4UserStackingAction*)0 );
  eventManager->ProcessOneEvent( currentEvent ); // Processing the trivial event

  // Called with output_buf and no size, creates object for unmarshaling
  // using marshalgen
  MarshaledG4HCofThisEvent marshaledObj( output_buf );
  G4HCofThisEvent* oldCE = currentEvent->GetHCofThisEvent();
  G4HCofThisEvent* HCE = new G4HCofThisEvent(oldCE->GetNumberOfCollections());
  
  marshaledObj.unmarshalTo(HCE);
  if(oldCE) delete(oldCE);

  currentEvent->SetHCofThisEvent(HCE);

  // Original UserEventAction was saved and set to NULL.  Do it now on master.
  HepRandom::setTheSeed(g_Seeds[i_event]);
  if ( origUserEventAction )
    origUserEventAction->BeginOfEventAction( currentEvent );

  if ( origUserEventAction )
    origUserEventAction->EndOfEventAction( currentEvent );

  AnalyzeEvent( currentEvent );

  if ( i_event < n_select ) G4UImanager::GetUIpointer()->ApplyCommand( msg );
  stateManager->SetNewState( G4State_GeomClosed );
  StackPreviousEvent( currentEvent );
  currentEvent = 0;
  return NO_ACTION;
}

#endif  /* G4USE_TOPC */