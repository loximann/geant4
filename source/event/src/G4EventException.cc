#include "G4EventException.hh"

#include <stdexcept>

G4EventException::G4EventException(const G4Event *const a_event):
    std::runtime_error{"Error in event"},
    event{a_event}
{}

