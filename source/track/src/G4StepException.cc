#include "G4StepException.hh"

G4StepException::G4StepException(const G4Step *const a_step):
    std::runtime_error{"Error in step"},
    step{a_step}
{}


