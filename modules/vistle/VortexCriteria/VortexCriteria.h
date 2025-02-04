// Vortex Criteria
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include <vistle/module/module.h>

#include "vistle_ext.h"

class VortexCriteria : public vistle::Module
{
private:
    // Member functions
    bool compute() override;
    bool changeParameter(const vistle::Parameter *param) override;

    // Inputs
    vistle::Port *velocity = nullptr;

    // Outputs
    vistle::Port *scalar = nullptr;

    // Parameters
    vistle::IntParameter *quantity = nullptr;
    vistle::IntParameter *smoothingRange = nullptr;

public:
    VortexCriteria(const std::string &name, int moduleId, mpi::communicator comm);
};
