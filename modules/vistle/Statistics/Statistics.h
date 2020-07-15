// Statistics
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include <vistle/module/module.h>

#include "vistle_ext.h"

class Statistics : public vistle::Module
{
private:
    // Member functions
    bool compute() override;

    // Inputs
    vistle::Port *grid = nullptr;
    vistle::Port *scalar = nullptr;
    vistle::Port *vector = nullptr;

    // Outputs

    // Parameters

public:
    Statistics(const std::string &name, int moduleId, mpi::communicator comm);
};
