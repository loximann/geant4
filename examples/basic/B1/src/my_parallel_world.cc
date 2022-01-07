#include "my_parallel_world.hh"

#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include <ostream>
#include <string>
#include <thread>
#include <utility>

MyParallelWorld::MyParallelWorld() :
    G4VUserParallelWorld{"MyParallelWorld"}
{
}

void MyParallelWorld::Construct()
{
    auto* ghost_world = GetWorld()->GetLogicalVolume();

    const double shift = 0;
  G4ThreeVector pos2 = G4ThreeVector(0+shift, -1*cm+shift, 7*cm+shift);

  // Trapezoid shape
  G4double shape2_dxa = 12*cm, shape2_dxb = 12*cm;
//  G4double shape2_dya = 10*cm, shape2_dyb = 16*cm;
  G4double shape2_dz  = 6*cm;
  G4Box* solidShape2 =
      new G4Box("Shape2_ghost",                //its name
              0.5*shape2_dxa, 0.5*shape2_dxb, 0.5*shape2_dz);


  G4LogicalVolume* logicShape2 =
    new G4LogicalVolume(solidShape2,         //its solid
                        nullptr,             //its material
                        "Shape2_ghost");     //its name

  new G4PVPlacement(0,                       //no rotation
                    pos2,                    //at position
                    logicShape2,             //its logical volume
                    "Shape2_ghost",          //its name
                    ghost_world,             //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    false);                  //overlaps checking

}

void MyParallelWorld::ConstructSD()
{
}
