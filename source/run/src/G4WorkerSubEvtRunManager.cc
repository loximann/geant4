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

#include "G4WorkerSubEvtRunManager.hh"

#include "G4AutoLock.hh"
#include "G4MTRunManager.hh"
#include "G4ParallelWorldProcess.hh"
#include "G4ParallelWorldProcessStore.hh"
#include "G4RNGHelper.hh"
#include "G4Run.hh"
#include "G4SDManager.hh"
#include "G4ScoringManager.hh"
#include "G4SubEvtRunManager.hh"
#include "G4Timer.hh"
#include "G4TransportationManager.hh"
#include "G4UImanager.hh"
#include "G4UserRunAction.hh"
#include "G4UserWorkerInitialization.hh"
#include "G4UserWorkerThreadInitialization.hh"
#include "G4VScoreNtupleWriter.hh"
#include "G4VScoringMesh.hh"
#include "G4VUserActionInitialization.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VUserPhysicsList.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4VVisManager.hh"
#include "G4WorkerTaskRunManagerKernel.hh"
#include "G4WorkerThread.hh"

#include <fstream>
#include <sstream>

//============================================================================//

G4WorkerSubEvtRunManager* G4WorkerSubEvtRunManager::GetWorkerRunManager()
{
  return static_cast<G4WorkerSubEvtRunManager*>(G4RunManager::GetRunManager());
}

//============================================================================//

G4WorkerSubEvtRunManagerKernel* G4WorkerSubEvtRunManager::GetWorkerRunManagerKernel()
{
  return static_cast<G4WorkerSubEvtRunManagerKernel*>(GetWorkerRunManager()->kernel);
}

//============================================================================//

G4WorkerSubEvtRunManager::G4WorkerSubEvtRunManager(G4int seType)
: fSubEventType(seType)
{ runManagerType = subEventWorkerRM; }

void G4WorkerSubEvtRunManager::RunInitialization()
{
#ifdef G4MULTITHREADED
  if (!visIsSetUp) {
    G4VVisManager* pVVis = G4VVisManager::GetConcreteInstance();
    if (pVVis != nullptr) {
      pVVis->SetUpForAThread();
      visIsSetUp = true;
    }
  }
#endif
  runIsSeeded = false;

  if (!(kernel->RunInitialization(fakeRun))) return;

  // Signal this thread can start event loop.
  // Note this will return only when all threads reach this point
  G4MTRunManager::GetMasterRunManager()->ThisWorkerReady();
  if (fakeRun) return;

  const G4UserWorkerInitialization* uwi =
    G4MTRunManager::GetMasterRunManager()->GetUserWorkerInitialization();

  CleanUpPreviousEvents();

  delete currentRun;

  currentRun = nullptr;

  if (IfGeometryHasBeenDestroyed()) G4ParallelWorldProcessStore::GetInstance()->UpdateWorlds();

  // Call a user hook: this is guaranteed all threads are "synchronized"
  if (uwi != nullptr) uwi->WorkerRunStart();

  if (userRunAction != nullptr) currentRun = userRunAction->GenerateRun();
  if (currentRun == nullptr) currentRun = new G4Run();

  currentRun->SetRunID(runIDCounter);
  G4TaskRunManager* mrm = G4TaskRunManager::GetMasterRunManager();
  numberOfEventToBeProcessed = mrm->GetNumberOfEventsToBeProcessed();
  currentRun->SetNumberOfEventToBeProcessed(numberOfEventToBeProcessed);

  currentRun->SetDCtable(DCtable);
  G4SDManager* fSDM = G4SDManager::GetSDMpointerIfExist();
  if (fSDM != nullptr) {
    currentRun->SetHCtable(fSDM->GetHCtable());
  }

  if (G4VScoreNtupleWriter::Instance() != nullptr) {
    auto hce = (fSDM != nullptr) ? fSDM->PrepareNewEvent() : nullptr;
    isScoreNtupleWriter = G4VScoreNtupleWriter::Instance()->Book(hce);
    delete hce;
  }

  std::ostringstream oss;
  G4Random::saveFullState(oss);
  randomNumberStatusForThisRun = oss.str();
  currentRun->SetRandomNumberStatus(randomNumberStatusForThisRun);

  for (G4int i_prev = 0; i_prev < n_perviousEventsToBeStored; ++i_prev)
    previousEvents->push_back(nullptr);

  if (printModulo > 0 || verboseLevel > 0) {
    G4cout << "### Run " << currentRun->GetRunID() << " starts on worker thread "
           << G4Threading::G4GetThreadId() << "." << G4endl;
  }

  if (userRunAction != nullptr) userRunAction->BeginOfRunAction(currentRun);

  if (isScoreNtupleWriter) {
    G4VScoreNtupleWriter::Instance()->OpenFile();
  }

  if (storeRandomNumberStatus) {
    G4String fileN = "currentRun";
    if (rngStatusEventsFlag) {
      std::ostringstream os;
      os << "run" << currentRun->GetRunID();
      fileN = os.str();
    }
    StoreRNGStatus(fileN);
  }

  runAborted = false;
  numberOfEventProcessed = 0;
}

//============================================================================//

void G4WorkerSubEvtRunManager::DoEventLoop(G4int n_event, const char* macroFile, G4int n_select)
{
//MAMAMAMA
G4Exception("G4WorkerSubEvtRunManager::DoEventLoop()","SuvEvtXXX001",FatalException,"We should not be here!");
//MAMAMAMA
  if (userPrimaryGeneratorAction == nullptr) {
    G4Exception("G4RunManager::GenerateEvent()", "Run0032", FatalException,
                "G4VUserPrimaryGeneratorAction is not defined!");
  }

  // This is the same as in the sequential case, just the for-loop indexes are
  // different
  InitializeEventLoop(n_event, macroFile, n_select);

  // Reset random number seeds queue
  while (!seedsQueue.empty())
    seedsQueue.pop();
  // for each run, worker should receive at least one set of random number
  // seeds.
  // runIsSeeded = false;

  // Event loop
  eventLoopOnGoing = true;
  G4int i_event = -1;
  nevModulo = -1;
  currEvID = -1;

  for (G4int evt = 0; evt < n_event; ++evt) {
    ProcessOneEvent(i_event);
    if (eventLoopOnGoing) {
      TerminateOneEvent();
      if (runAborted) eventLoopOnGoing = false;
    }
    if (!eventLoopOnGoing) break;
  }
}

//============================================================================//

void G4WorkerSubEvtRunManager::ProcessOneEvent(G4int i_event)
{
//MAMAMAMA
G4Exception("G4WorkerSubEvtRunManager::ProcessOneEvent()","SuvEvtXXX002",FatalException,"We should not be here!");
//MAMAMAMA
  currentEvent = GenerateEvent(i_event);
  if (eventLoopOnGoing) {
    eventManager->ProcessOneEvent(currentEvent);
    AnalyzeEvent(currentEvent);
    UpdateScoring();
    if (currentEvent->GetEventID() < n_select_msg) {
      G4cout << "Applying command \"" << msgText << "\" @ " << __FUNCTION__ << ":" << __LINE__
             << G4endl;
      G4UImanager::GetUIpointer()->ApplyCommand(msgText);
    }
  }
}

//============================================================================//

G4Event* G4WorkerSubEvtRunManager::GenerateEvent(G4int i_event)
{
//MAMAMAMA
G4Exception("G4WorkerSubEvtRunManager::GenerateEvent()","SuvEvtXXX003",FatalException,"We should not be here!");
//MAMAMAMA
  auto anEvent = new G4Event(i_event);
  G4long s1 = 0;
  G4long s2 = 0;
  G4long s3 = 0;
  G4bool eventHasToBeSeeded = true;
  if (G4MTRunManager::SeedOncePerCommunication() == 1 && runIsSeeded) eventHasToBeSeeded = false;

  if (i_event < 0) {
    G4int nevM = G4MTRunManager::GetMasterRunManager()->GetEventModulo();
    if (nevM == 1) {
      eventLoopOnGoing = G4MTRunManager::GetMasterRunManager()->SetUpAnEvent(anEvent, s1, s2, s3,
                                                                             eventHasToBeSeeded);
      runIsSeeded = true;
    }
    else {
      if (nevModulo <= 0) {
        G4int nevToDo = G4MTRunManager::GetMasterRunManager()->SetUpNEvents(anEvent, &seedsQueue,
                                                                            eventHasToBeSeeded);
        if (nevToDo == 0)
          eventLoopOnGoing = false;
        else {
          currEvID = anEvent->GetEventID();
          nevModulo = nevToDo - 1;
        }
      }
      else {
        if (G4MTRunManager::SeedOncePerCommunication() > 0) eventHasToBeSeeded = false;
        anEvent->SetEventID(++currEvID);
        nevModulo--;
      }
      if (eventLoopOnGoing && eventHasToBeSeeded) {
        s1 = seedsQueue.front();
        seedsQueue.pop();
        s2 = seedsQueue.front();
        seedsQueue.pop();
      }
    }

    if (!eventLoopOnGoing) {
      delete anEvent;
      return nullptr;
    }
  }
  else if (eventHasToBeSeeded) {
    // Need to reseed random number generator
    G4RNGHelper* helper = G4RNGHelper::GetInstance();
    s1 = helper->GetSeed(i_event * 2);
    s2 = helper->GetSeed(i_event * 2 + 1);
  }

  if (eventHasToBeSeeded) {
    G4long seeds[3] = {s1, s2, 0};
    G4Random::setTheSeeds(seeds, -1);
    runIsSeeded = true;
  }

  // Read from file seed.
  // Andrea Dotti 4 November 2015
  // This is required for strong-reproducibility, in MT mode we have that each
  // thread produces, for each event a status file, we want to do that.
  // Search a random file with the format run{%d}evt{%d}.rndm

  // This is the filename base constructed from run and event
  const auto filename = [&] {
    std::ostringstream os;
    os << "run" << currentRun->GetRunID() << "evt" << anEvent->GetEventID();
    return os.str();
  };

  G4bool RNGstatusReadFromFile = false;
  if (readStatusFromFile) {
    // Build full path of RNG status file for this event
    std::ostringstream os;
    os << filename() << ".rndm";
    const G4String& randomStatusFile = os.str();
    std::ifstream ifile(randomStatusFile.c_str());
    if (ifile) {
      // File valid and readable
      RNGstatusReadFromFile = true;
      G4Random::restoreEngineStatus(randomStatusFile.c_str());
    }
  }

  if (storeRandomNumberStatusToG4Event == 1 || storeRandomNumberStatusToG4Event == 3) {
    std::ostringstream oss;
    G4Random::saveFullState(oss);
    randomNumberStatusForThisEvent = oss.str();
    anEvent->SetRandomNumberStatus(randomNumberStatusForThisEvent);
  }

  if (storeRandomNumberStatus && !RNGstatusReadFromFile) {
    // If reading from file, avoid to rewrite the same
    G4String fileN = "currentEvent";
    if (rngStatusEventsFlag) fileN = filename();
    StoreRNGStatus(fileN);
  }

  if (printModulo > 0 && anEvent->GetEventID() % printModulo == 0) {
    G4cout << "--> Event " << anEvent->GetEventID() << " starts";
    if (eventHasToBeSeeded) G4cout << " with initial seeds (" << s1 << "," << s2 << ")";
    G4cout << "." << G4endl;
  }
  userPrimaryGeneratorAction->GeneratePrimaries(anEvent);
  return anEvent;
}

//============================================================================//

void G4WorkerSubEvtRunManager::RunTermination()
{
  if (!fakeRun && (currentRun != nullptr)) {
    MergePartialResults(true);

    // Call a user hook: note this is before the next barrier
    // so threads execute this method asyncrhonouzly
    //(TerminateRun allows for synch via G4RunAction::EndOfRun)
    const G4UserWorkerInitialization* uwi =
      G4MTRunManager::GetMasterRunManager()->GetUserWorkerInitialization();
    if (uwi != nullptr) uwi->WorkerRunEnd();
  }

  if (currentRun != nullptr) {
    G4RunManager::RunTermination();
  }
  // Signal this thread has finished envent-loop.
  // Note this will return only whan all threads reach this point
  G4MTRunManager::GetMasterRunManager()->ThisWorkerEndEventLoop();
}

//============================================================================//

void G4WorkerSubEvtRunManager::TerminateEventLoop()
{
  if (verboseLevel > 0 && !fakeRun) {
    timer->Stop();
    // prefix with thread # info due to how TBB calls this function
    G4String prefix = "[thread " + std::to_string(workerContext->GetThreadId()) + "] ";
    G4cout << prefix << "Thread-local run terminated." << G4endl;
    G4cout << prefix << "Run Summary" << G4endl;
    if (runAborted)
      G4cout << prefix << "  Run Aborted after " << numberOfEventProcessed << " events processed."
             << G4endl;
    else
      G4cout << prefix << "  Number of events processed : " << numberOfEventProcessed << G4endl;
    G4cout << prefix << "  " << *timer << G4endl;
  }
}

//============================================================================//

void G4WorkerSubEvtRunManager::SetupDefaultRNGEngine()
{
  const CLHEP::HepRandomEngine* mrnge =
    G4MTRunManager::GetMasterRunManager()->getMasterRandomEngine();
  assert(mrnge);  // Master has created RNG
  const G4UserWorkerThreadInitialization* uwti =
    G4MTRunManager::GetMasterRunManager()->GetUserWorkerThreadInitialization();
  uwti->SetupRNGEngine(mrnge);
}

//============================================================================//

void G4WorkerSubEvtRunManager::StoreRNGStatus(const G4String& fn)
{
  std::ostringstream os;
  os << randomNumberStatusDir << "G4Worker" << workerContext->GetThreadId() << "_" << fn << ".rndm";
  G4Random::saveEngineStatus(os.str().c_str());
}

//============================================================================//

void G4WorkerSubEvtRunManager::ProcessUI()
{
  G4TaskRunManager* mrm = G4TaskRunManager::GetMasterRunManager();
  if (mrm == nullptr) return;

  //------------------------------------------------------------------------//
  // Check UI commands not already processed
  auto command_stack = mrm->GetCommandStack();
  bool matching = (command_stack.size() == processedCommandStack.size());
  if (matching) {
    for (uintmax_t i = 0; i < command_stack.size(); ++i)
      if (processedCommandStack.at(i) != command_stack.at(i)) {
        matching = false;
        break;
      }
  }

  //------------------------------------------------------------------------//
  // Execute UI commands stored in the master UI manager
  if (!matching) {
    for (const auto& itr : command_stack)
      G4UImanager::GetUIpointer()->ApplyCommand(itr);
    processedCommandStack = command_stack;
  }
}

//============================================================================//

void G4WorkerSubEvtRunManager::DoCleanup()
{
  // Nothing to do for a run 

  //CleanUpPreviousEvents();
  //
  //delete currentRun;
  //currentRun = nullptr;
}

//============================================================================//

void G4WorkerSubEvtRunManager::DoWork()
{
  if(verboseLevel>1) {
    G4cout << "G4WorkerSubEvtRunManager::DoWork() starts.........." << G4endl;
  }

  //G4TaskRunManager* mrm = G4TaskRunManager::GetMasterRunManager();
  G4SubEvtRunManager* mrm = G4SubEvtRunManager::GetMasterRunManager();
  G4bool newRun = false;
  const G4Run* run = mrm->GetCurrentRun();
  G4ThreadLocalStatic G4int runId = -1;
  if ((run != nullptr) && run->GetRunID() != runId) {
    runId = run->GetRunID();
    newRun = true;
    if (runId > 0) { ProcessUI(); }
  }

  G4bool reseedRequired = false;
  if (newRun) {
    G4bool cond = ConfirmBeamOnCondition();
    if (cond) {
      ConstructScoringWorlds();
      RunInitialization();
    }
    reseedRequired = true;
  }

  assert(workerContext != nullptr);
  workerContext->UpdateGeometryAndPhysicsVectorFromMaster();

  eventManager->UseSubEventParallelism(true);

  G4bool needMoreWork = true;
  while(needMoreWork)
  {
    G4bool notReady = false;
    G4long s1, s2, s3;
    auto subEv = mrm->GetSubEvent(fSubEventType, notReady, s1, s2, s3, reseedRequired);
    if(subEv==nullptr && notReady)
    {
      // Master is not yet ready for tasking a sub-event. 
      // Wait 1 second and retry.
      G4THREADSLEEP(1);
    }
    else if(subEv==nullptr)
    { 
      // No more sub-event to process
      needMoreWork = false;
    }
    else
    {
      // Let's work for this sub-event.
      if(reseedRequired)
      {
        G4long seeds[3] = {s1, s2, s3};
        G4Random::setTheSeeds(seeds, -1);
        reseedRequired = false;
      }

      // create a G4Event object for this sub-event. This G4Event object will contain output
      // to be merged into the master event.
      auto masterEvent = subEv->GetEvent();
      G4Event* ev = new G4Event(masterEvent->GetEventID());
      ev->FlagAsSubEvent(masterEvent,fSubEventType);

      // Create a G4TrackVector as the input
      G4TrackVector* tv = new G4TrackVector();
      for(auto& stackedTrack : *subEv)
      {
        // tracks (and trajectories) stored in G4SubEvent object belong to the master thread
        // and thus they must not be deleted by the worker thread. They must be cloned.
        G4Track* tr = new G4Track();
        tr->CopyTrackInfo(*(stackedTrack.GetTrack()),false);

        //MAMAMAMAMA cloning of trajectory object is not yet implemented.
        tv->push_back(tr);
      }

      // Process this sub-event
      currentEvent = ev;
      eventManager->ProcessOneEvent(tv,ev);

      // We don't need following two lines, as they are taken care by the master
      //////AnalyzeEvent(ev);
      //////UpdateScoring();

      // Report the results to the master
      mrm->SubEventFinished(subEv,ev);
     
      // clean up
      delete tv;
      delete ev;
    }
  }

  if(verboseLevel>1) {
    G4cout << "G4WorkerSubEvtRunManager::DoWork() completed.........." << G4endl;
  }
    
}

//============================================================================//

void G4WorkerSubEvtRunManager::SetUserInitialization(G4UserWorkerInitialization*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserInitialization(G4UserWorkerInitialization*)", "RunSE0118",
              FatalException, "This method should be used only with an instance of the master thread");
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserInitialization(G4UserWorkerThreadInitialization*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserInitialization(G4UserWorkerThreadInitialization*)", "RunSE0119",
              FatalException, "This method should be used only with an instance of the master thread");
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserInitialization(G4VUserActionInitialization*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserInitialization(G4VUserActionInitialization*)", "RunSE0120",
              FatalException, "This method should be used only with an instance of the master thread");
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserInitialization(G4VUserDetectorConstruction*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserInitialization(G4VUserDetectorConstruction*)", "RunSE0121",
              FatalException, "This method should be used only with an instance of the master thread");
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserInitialization(G4VUserPhysicsList* pl)
{
  pl->InitializeWorker();
  G4RunManager::SetUserInitialization(pl);
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4UserRunAction*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserAction(G4UserRunAction*)", "RunSE0221",
              FatalException, "This method should be used only with an instance of the master thread");
}

// Forward calls (avoid GCC compilation warnings)

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4UserEventAction* ua)
{
  G4RunManager::SetUserAction(ua);
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4VUserPrimaryGeneratorAction*)
{
  G4Exception("G4WorkerSubEvtRunManager::SetUserAction(G4VUserPrimaryGeneratorAction*)", "RunSE0223",
              FatalException, "This method should be used only with an instance of the master thread");
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4UserStackingAction* ua)
{
  G4RunManager::SetUserAction(ua);
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4UserTrackingAction* ua)
{
  G4RunManager::SetUserAction(ua);
}

// --------------------------------------------------------------------
void G4WorkerSubEvtRunManager::SetUserAction(G4UserSteppingAction* ua)
{
  G4RunManager::SetUserAction(ua);
}



