// Flow Topology
// Ronald Peikert and Filip Sadlo 2005
// CGL ETHZ

#include <cstdlib>
#include <cstdio>

#include "FlowTopo.h"

#include "linalg.h"

#include "unstructured.h"
#include "unifield.h"
#include "unisys.h"

using namespace vistle;

#include "flow_topo_impl.cpp" // ### including .cpp

FlowTopo::FlowTopo(const std::string &name, int moduleId, mpi::communicator comm)
: Module("flow topology", name, moduleId, comm)
{
    // ports
    velocity = createInputPort("velocity", "Vector Data");
    wallDistance = createInputPort("wallDistance", "Scalar Data");
    //wallDistance->setRequired(0);

    criticalPoints = createOutputPort("criticalPoints", "Critical Points");

    // params
    divideByWallDist = addIntParameter("divideByWallDist", "divide by wall distance", false, Parameter::Boolean);
    interiorCritPoints = addIntParameter("interiorCritPoints", "compute interior critical points", true, Parameter::Boolean);
    boundaryCritPoints = addIntParameter("boundaryCritPoints", "compute boundary critical points", false, Parameter::Boolean);
    generateSeeds = addIntParameter("generateSeeds", "generate seeds", false, Parameter::Boolean);
    seedsPerCircle = addIntParameter("seedsPerCircle", "seeds per circle", 8);
    setParameterRange<Integer>(seedsPerCircle, 1, INT_MAX);
    radius = addFloatParameter("radius", "radius", 1.);
    setParameterRange<Float>(radius, 0., FLT_MAX);
    offset = addFloatParameter("offset", "offset", 1.);
    setParameterRange<Float>(offset, 0., FLT_MAX);
}

bool FlowTopo::compute()
{
    // system wrapper
    UniSys us(this);

    auto wallDistanceIn = accept<Vec<Scalar>>(wallDistance);
    auto velocityIn = expect<Vec<Scalar,3>>(velocity);

    // create unstructured object for input
    std::vector<Vec<Scalar>::const_ptr> svec;
    if (wallDistanceIn)
        svec.push_back(wallDistanceIn);
    std::vector<Vec<Scalar,3>::const_ptr> vvec;
    vvec.push_back(velocityIn);
    Unstructured unst(UnstructuredGrid::as(velocityIn->grid()), (wallDistanceIn ? &svec : nullptr), &vvec);

    // scalar components come first in Covise-Unstructured
    // ### HACK, should retrieve from Unstructured?
    int compWallDist;
    int compVelo;
    if (wallDistanceIn)
    {
        compWallDist = 0;
        compVelo = 1;
    }
    else
    {
        compWallDist = -1;
        compVelo = 0;
    }

#if 0
  // Generate output data
  coDoFloat *odata;
  {
    int numEl, numConn, numCoord;
    ((coDoUnstructuredGrid *) (grid->getObj()))->get_grid_size(&numEl, &numConn, &numCoord);
    odata = new coDoVec3(matDeriv->getObjName(), numCoord);
  }
#endif

    // go

    // setup Unifield for output, without allocating or assigning data
    UniField *unif = new UniField;

    // compute
    flow_topo_impl(&us, &unst, compVelo, compWallDist, &unif,
                   divideByWallDist->getValue(),
                   interiorCritPoints->getValue(),
                   boundaryCritPoints->getValue(),
                   generateSeeds->getValue(),
                   seedsPerCircle->getValue(),
                   radius->getValue(),
                   offset->getValue());

    if (!unif) {
        return false;
    }

    // assign output data to ports
    vistle::StructuredGrid::ptr wgrid;
    vistle::Vec<Scalar>::ptr wdat;
    unif->getField(&wgrid, &wdat, (vistle::Vec<Scalar,3>::ptr *)nullptr);
    delete unif;
    unif = nullptr;

    wdat->setGrid(wgrid);

    addObject(criticalPoints, wdat);

    return true;
}

MODULE_MAIN(FlowTopo)
