#ifndef G4EVENTEXCEPTION_HH
#define G4EVENTEXCEPTION_HH

#include "G4Event.hh"

#include <stdexcept>

class G4EventException : public std::runtime_error {
    public:
    G4EventException(const G4Event *const a_event);
    const G4Event *const event;
};

#endif // G4EVENTEXCEPTION_HH
