// Vortex Criteria
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include "stdlib.h"
#include "stdio.h"

#include "VortexCriteria.h"

#include "linalg.h"

#include "unstructured.h"
#include "unisys.h"

#include "vortex_criteria_impl.cpp" // ### including .cpp

#include <vistle/core/unstr.h>
#include <vistle/core/vec.h>

using namespace vistle;

DEFINE_ENUM_WITH_STRING_CONVERSIONS(Quantity,
                                    (Helicity)
                                    (Velo_norm_helicity)
                                    (Vorticity_magnitude)
                                    (Z_vorticity)
                                    (Lambda2)
                                    (Q)
                                    (Delta)
                                    (Div_accel)
                                    (Divergence))

bool VortexCriteria::compute()
{
    // system wrapper
    UniSys us(this);

    // create unstructured wrapper for input
    std::vector<Vec<Scalar,3>::const_ptr> vvec;
    vvec.push_back(expect<Vec<Scalar,3>>(velocity));
    auto unstr = UnstructuredGrid::as(vvec[0]->grid());
    Unstructured unst_in(unstr, nullptr, &vvec);

    // scalar components come first in Covise-Unstructured
    int compVelo = 0;

    // compute gradient
    if (us.inputChanged("ucd", 0) || us.parameterChanged("smoothingRange"))
    {
        us.moduleStatus("computing gradient", 5);
        unst_in.gradient(compVelo, false, smoothingRange->getValue());
        us.moduleStatus("computing gradient", 50);
    }

    // go
    auto nNodes = unstr->getNumCoords();
    Vec<Scalar>::ptr scalarData(new Vec<Scalar>(nNodes));
    scalarData->setGrid(unstr);
    std::vector<Vec<Scalar>::const_ptr> svec;
    svec.push_back(scalarData);
    // wrapper for output
    Unstructured unst_scalar(unstr, &svec, nullptr);

    // compute
    vortex_criteria_impl(&us, &unst_in, compVelo,
                             NULL, 0, // ### TODO
                             &unst_scalar,
                             quantity->getValue() + 1,
                             smoothingRange->getValue() + 1,
                             NULL);

    scalarData->copyAttributes(vvec[0]);
    scalarData->addAttribute("_species", toString(Quantity(quantity->getValue())));
    updateMeta(scalarData);
    addObject(scalar, scalarData);

    return true;
}

bool VortexCriteria::changeParameter(const vistle::Parameter *param)
{
    if (!param || param == quantity) {
        setItemInfo(toString(Quantity(quantity->getValue())));
    }
    return Module::changeParameter(param);
}


VortexCriteria::VortexCriteria(const std::string &name, int moduleId, mpi::communicator comm)
: vistle::Module(name, moduleId, comm)
{
    velocity = createInputPort("velocity", "Vector Data");
    scalar = createOutputPort("scalar", "Scalar Criterion");
    quantity = addIntParameter("quantity", "quantity", Helicity, Parameter::Choice);
    V_ENUM_SET_CHOICES(quantity, Quantity);

    smoothingRange = addIntParameter("smoothingRange", "smoothing range for gradient", 1);
    setParameterRange<Integer>(smoothingRange, 1, INT_MAX);
}

MODULE_MAIN(VortexCriteria)
