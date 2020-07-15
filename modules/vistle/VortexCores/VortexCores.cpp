// Vortex Core Lines
// Ronald Peikert, Martin Roth <=2005 and Filip Sadlo >=2006
// Computer Graphics Laboratory, ETH Zurich

#include <cstdlib>
#include <cstdio>

#include "VortexCores.h"

#include "linalg.h"

#include "unstructured.h"
#include "unigeom.h"
#include "unisys.h"

#include "computeVortexCores.h"
#include "ucd_vortex_cores_impl.cpp" // ### including .cpp
#include <vistle/util/enum.h>
#include <vistle/core/unstr.h>

using namespace vistle;

bool VortexCores::compute()
{
    // system wrapper
    UniSys us(this);

    std::vector<Vec<Scalar,3>::const_ptr> vvec;
    vvec.push_back(expect<Vec<Scalar,3>>(velocity));
    if (!vvec[0])
        return true;

    Unstructured unst(UnstructuredGrid::as(vvec[0]->grid()), nullptr, &vvec);

    // scalar components come first in Covise-Unstructured
    int compVelo = 0;

    // go

    // setup geometry wrapper for output
    UniGeom ugeom(this, coreLines, vvec[0]);

    // compute
    ucd_vortex_cores_impl(&us,
                          &unst, compVelo,
                          method->getValue() + 1,
                          variant->getValue() + 1,
                          minVertices->getValue(),
                          maxExceptions->getValue(),
                          minStrength->getValue(),
                          maxAngle->getValue(),
                          &ugeom);

    // output data already assigned to ports

    return true;
}

DEFINE_ENUM_WITH_STRING_CONVERSIONS(Method, (Levy)(Sujudi_Haimes))
DEFINE_ENUM_WITH_STRING_CONVERSIONS(Variant, (Triangle)(Quad_Newton))

VortexCores::VortexCores(const std::string &name, int moduleId, mpi::communicator comm)
: vistle::Module("vortex corelines", name, moduleId, comm)
{
    velocity = createInputPort("velocity", "Vector Data");
    coreLines = createOutputPort("coreLines", "Vortex Core Lines");

    method = addIntParameter("method", "method", Levy, Parameter::Choice);
    V_ENUM_SET_CHOICES(method, Method);
    variant = addIntParameter("variant", "variant", Triangle, Parameter::Choice);
    V_ENUM_SET_CHOICES(variant, Variant);
    minVertices = addIntParameter("minVertices", "minimal number of vertices", 10);
    setParameterRange<Integer>(minVertices, 2, INT_MAX);
    maxExceptions = addIntParameter("maxExceptions", "maximal number of exceptions", 3);
    setParameterRange<Integer>(maxExceptions, 1, INT_MAX);
    minStrength = addFloatParameter("minStrength", "minimal strength", 1.0);
    setParameterRange<Float>(minStrength, 0., FLT_MAX);
    maxAngle = addFloatParameter("maxAngle", "maximal angle between v and w field", 30.0);
    setParameterRange(maxAngle, 0., 90.0);
}

MODULE_MAIN(VortexCores)
