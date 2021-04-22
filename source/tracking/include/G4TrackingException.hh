#ifndef G4TRACKINGEXCEPTION_HH
#define G4TRACKINGEXCEPTION_HH

#include "G4Track.hh"

#include <stdexcept>

class G4TrackException : public std::runtime_error {
    public:
    G4TrackException(const G4Track *const a_track);
    const G4Track *const track;
};

#endif // G4TRACKINGEXCEPTION_HH
