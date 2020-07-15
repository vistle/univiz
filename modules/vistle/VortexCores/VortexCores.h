// Vortex Core Lines
// Ronald Peikert, Martin Roth <=2005 and Filip Sadlo >=2006
// Computer Graphics Laboratory, ETH Zurich

#include <vistle/module/module.h>

#include <vistle_ext.h>

class VortexCores : public vistle::Module
{
private:
    // Member functions
    bool compute() override;

    // Inputs
    vistle::Port *velocity = nullptr;

    // Outputs
    vistle::Port *coreLines = nullptr;

    // Parameters
    vistle::IntParameter *method = nullptr;
    vistle::IntParameter *variant = nullptr;
    vistle::IntParameter *minVertices = nullptr;
    vistle::IntParameter *maxExceptions = nullptr;
    vistle::FloatParameter *minStrength = nullptr;
    vistle::FloatParameter *maxAngle = nullptr;

public:
    VortexCores(const std::string &name, int moduleId, mpi::communicator comm);
};
