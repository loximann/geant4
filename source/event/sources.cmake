#------------------------------------------------------------------------------
# Module : G4event
# Package: Geant4.src.G4event
#------------------------------------------------------------------------------

#
# Define the Geant4 Module.
#
geant4_define_module(NAME G4event
  HEADERS
    G4AdjointPosOnPhysVolGenerator.hh
    G4AdjointPrimaryGenerator.hh
    G4AdjointStackingAction.hh
    G4ClassificationOfNewTrack.hh
    G4EvManMessenger.hh
    G4Event.hh
    G4EventException.hh
    G4EventManager.hh
    G4GeneralParticleSource.hh
    G4GeneralParticleSourceData.hh
    G4GeneralParticleSourceMessenger.hh
    G4HEPEvtInterface.hh
    G4HEPEvtParticle.hh
    G4ParticleGun.hh
    G4ParticleGunMessenger.hh
    G4PrimaryTransformer.hh
    G4RayShooter.hh
    G4SPSAngDistribution.hh
    G4SPSEneDistribution.hh
    G4SPSPosDistribution.hh
    G4SPSRandomGenerator.hh
    G4SingleParticleSource.hh
    G4SmartTrackStack.hh
    G4StackChecker.hh
    G4StackManager.hh
    G4StackedTrack.hh
    G4StackingMessenger.hh
    G4TrackStack.hh
    G4TrajectoryContainer.hh
    G4UserEventAction.hh
    G4MultiEventAction.hh
    G4UserStackingAction.hh
    G4VPrimaryGenerator.hh
    G4VUserEventInformation.hh
    evtdefs.hh
  SOURCES
    G4AdjointPosOnPhysVolGenerator.cc
    G4AdjointPrimaryGenerator.cc
    G4AdjointStackingAction.cc
    G4EvManMessenger.cc
    G4Event.cc
    G4EventException.cc
    G4EventManager.cc
    G4GeneralParticleSource.cc
    G4GeneralParticleSourceData.cc
    G4GeneralParticleSourceMessenger.cc
    G4HEPEvtInterface.cc
    G4HEPEvtParticle.cc
    G4ParticleGun.cc
    G4ParticleGunMessenger.cc
    G4PrimaryTransformer.cc
    G4RayShooter.cc
    G4SPSAngDistribution.cc
    G4SPSEneDistribution.cc
    G4SPSPosDistribution.cc
    G4SPSRandomGenerator.cc
    G4SingleParticleSource.cc
    G4SmartTrackStack.cc
    G4StackChecker.cc
    G4StackManager.cc
    G4StackingMessenger.cc
    G4TrackStack.cc
    G4TrajectoryContainer.cc
    G4UserEventAction.cc
    G4MultiEventAction.cc
    G4UserStackingAction.cc
    G4VPrimaryGenerator.cc
  GRANULAR_DEPENDENCIES
    G4baryons
    G4bosons
    G4detector
    G4digits
    G4emutils
    G4geombias
    G4geometrymng
    G4globman
    G4graphics_reps
    G4hepnumerics
    G4hits
    G4intercoms
    G4ions
    G4leptons
    G4materials
    G4mesons
    G4navigation
    G4partman
    G4procman
    G4track
    G4tracking
    G4volumes
  GLOBAL_DEPENDENCIES
    G4digits_hits
    G4geometry
    G4global
    G4graphics_reps
    G4intercoms
    G4materials
    G4particles
    G4processes
    G4track
    G4tracking
  LINK_LIBRARIES
    ${timemory_LIBRARIES}
)

# List any source specific properties here
# Enable smart stack
if(GEANT4_USE_SMARTSTACK)
  geant4_module_compile_definitions(G4event PUBLIC G4_USESMARTSTACK)
endif()

