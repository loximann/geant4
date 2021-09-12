# - G4hadronic_mgt module build definition

# Define the Geant4 Module.
geant4_add_module(G4hadronic_mgt
  PUBLIC_HEADERS
    G4EnergyRangeManager.hh
    G4HadLeadBias.hh
    G4HadronicEPTestMessenger.hh
    G4HadronicInteraction.hh
    G4HadronicInteractionRegistry.hh
    G4HadronicProcess.hh
    G4HadronicProcessStore.hh
    G4HadronicProcessType.hh
    G4NoModelFound.hh
    G4VHighEnergyGenerator.hh
    G4VIntraNuclearTransportModel.hh
    G4VLeadingParticleBiasing.hh
    G4VPreCompoundModel.hh
  SOURCES
    G4EnergyRangeManager.cc
    G4HadLeadBias.cc
    G4HadronicEPTestMessenger.cc
    G4HadronicInteraction.cc
    G4HadronicInteractionRegistry.cc
    G4HadronicProcess.cc
    G4HadronicProcessStore.cc
    G4VHighEnergyGenerator.cc
    G4VIntraNuclearTransportModel.cc
    G4VPreCompoundModel.cc)

geant4_module_link_libraries(G4hadronic_mgt
  PUBLIC
    G4globman
    G4hadronic_util
    G4hadronic_xsect
    G4intercoms
    G4materials
    G4partman
    G4procman
    G4track
  PRIVATE
    G4baryons
    G4bosons
    G4heprandom
    G4leptons
    G4mesons)
