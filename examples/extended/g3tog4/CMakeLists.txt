#---Adding all g3tog4 examples subdirectories explicitly 

cmake_minimum_required(VERSION 3.12...3.20)

#----------------------------------------------------------------------------
# g3tog4 examples require Geant4 build with g3tog4
#
find_package(Geant4)
if(Geant4_g3tog4_FOUND)
  add_subdirectory(clGeometry)
else()  
  message(STATUS "G4 Examples: G3TOG4 not installed --> g3tog4 examples disabled")  
endif()
