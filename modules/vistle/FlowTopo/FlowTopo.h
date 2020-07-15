#include <vistle/module/module.h>

#include "vistle_ext.h"

class FlowTopo: public vistle::Module
{
private:
    // Member functions
    bool compute() override;

    // Inputs
    vistle::Port *velocity = nullptr;
    vistle::Port *wallDistance = nullptr;

    // Outputs
    vistle::Port *criticalPoints = nullptr;

    // Parameters
    vistle::IntParameter *divideByWallDist = nullptr;
    vistle::IntParameter *interiorCritPoints = nullptr;
    vistle::IntParameter *boundaryCritPoints = nullptr;
    vistle::IntParameter *generateSeeds = nullptr;
    vistle::IntParameter *seedsPerCircle = nullptr;
    vistle::FloatParameter *radius = nullptr;
    vistle::FloatParameter *offset = nullptr;

public:
    FlowTopo(const std::string &name, int moduleId, mpi::communicator comm);
};
