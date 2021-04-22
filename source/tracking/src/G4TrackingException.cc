#include "G4TrackingException.hh"

G4TrackException::G4TrackException(const G4Track *const a_track):
    std::runtime_error{"Error in track"},
    track{a_track}
{}


