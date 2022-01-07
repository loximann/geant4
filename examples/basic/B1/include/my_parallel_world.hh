#ifndef NUSIM_LIBNUSIM_GEANT4_SCORING_MY_PARALLEL_WORLD_H
#define NUSIM_LIBNUSIM_GEANT4_SCORING_MY_PARALLEL_WORLD_H

#include "G4LogicalVolume.hh"
#include "G4VUserParallelWorld.hh"

/// A material-less volume that does not interact with the particles but can track their passage.
class MyParallelWorld final : public G4VUserParallelWorld {
public:
    /// Constructor.
    MyParallelWorld();

    MyParallelWorld(const MyParallelWorld& other) = delete;
    /// Move constructor (to enable emplace_back).
    MyParallelWorld(MyParallelWorld&& other) = default;
    MyParallelWorld& operator=(const MyParallelWorld& other) = delete;
    MyParallelWorld& operator=(MyParallelWorld&& other) = delete;

    /// Destructor.
    ~MyParallelWorld() final = default;

    /// Callback used by Geant4 to build the detector volume.
    void Construct() final;
    /// Callback used by Geant4 to initialize and set the sensitive detector.
    void ConstructSD() final;

private:
    /// Pointer to the parallel world logical volume.
    G4LogicalVolume* m_lv = nullptr;
};

#endif // NUSIM_LIBNUSIM_GEANT4_SCORING_MY_PARALLEL_WORLD_H
