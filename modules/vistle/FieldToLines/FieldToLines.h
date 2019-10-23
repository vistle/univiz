// Field To Lines
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include <module/module.h>

#include "vistle_ext.h"

class FieldToLines : public vistle::Module
{
private:
    // Member functions
    virtual bool compute() override;

    // Inputs
    vistle::Port *lineField = nullptr;

    // Outputs
    vistle::Port *lines = nullptr;

    // Parameters
    vistle::IntParameter *nodesX = nullptr;
    vistle::IntParameter *nodesY = nullptr;
    vistle::IntParameter *nodesZ = nullptr;
    vistle::IntParameter *stride = nullptr;

public:
    FieldToLines(const std::string &name, int moduleId, mpi::communicator comm);
};
