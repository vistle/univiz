// Statistics
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include <cstdlib>
#include <cstdio>

#include "Statistics.h"

#include "linalg.h"

#include "unstructured.h"
#include "unisys.h"

#include "statistics_impl.cpp" // ### including .cpp

using namespace vistle;

bool Statistics::compute()
{
    // system wrapper
    UniSys us(this);

    // create unstructured wrapper for input
    int compScalar = -1;
    int compVector = -1;

    std::vector<Vec<Scalar>::const_ptr> svec;
    if (auto obj = accept<Object>(scalar))
    {
        svec.push_back(Vec<Scalar,1>::as(obj));
        compScalar = 0;
    }
    std::vector<Vec<Scalar,3>::const_ptr> vvec;
    if (auto obj = accept<Object>(vector))
    {
        vvec.push_back(Vec<Scalar,3>::as(obj));
        compVector = compScalar + 1;
    }

    Unstructured *unst_in = nullptr;
    auto g = accept<UnstructuredGrid>(grid);
    if (!svec.empty())
    {
        unst_in = new Unstructured(g, &svec, NULL);
    }
    else
    {
        unst_in = new Unstructured(g, NULL, &vvec);
    }

    if (compScalar < 0 && compVector < 0)
    {
        return false;
    }

    if (compScalar >= 0 && !vvec.empty())
    {
        us.warning("scalar and vector input available, computing statistics only for scalar");
    }

    // compute
    statistics_impl(&us, unst_in, (compScalar >= 0 ? compScalar : compVector));

    return true;
}

Statistics::Statistics(const string &name, int moduleId, mpi::communicator comm)
: vistle::Module(name, moduleId, comm)
{
    grid = createInputPort("grid", "Unstructured Grid");
    vector = createInputPort("vector", "Vector Data");
    scalar = createInputPort("scalar", "Scalar Data");

    // default values
}

MODULE_MAIN(Statistics)
