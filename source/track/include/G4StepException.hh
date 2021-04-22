#ifndef G4STEPEXCEPTION_HH
#define G4STEPEXCEPTION_HH

#include "G4Step.hh"

#include <stdexcept>

class G4StepException : public std::runtime_error {
    public:
    G4StepException(const G4Step *const a_step);

    const G4Step *const step;
};
#endif // G4STEPEXCEPTION_HH
