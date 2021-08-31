// Field To Lines
// Filip Sadlo 2007
// Computer Graphics Laboratory, ETH Zurich

#include "stdlib.h"
#include "stdio.h"

#include "FieldToLines.h"

#include "linalg.h"

#include "unifield.h"
#include "unigeom.h"
#include "unisys.h"

#include "field_to_lines_impl.cpp" // ### including .cpp

#include <vistle/core/database.h>

using namespace vistle;

bool FieldToLines::compute()
{
    // system wrapper
    UniSys us(this);

    auto data = expect<DataBase>(lineField);
    if (!data)
        return true;

    // wrapper for input field
    UniField unif(Vec<Scalar>::ptr(), Vec<Scalar,2>::as(data));

    // geometry wrapper for output
    UniGeom ugeom(this, lines);


   // compute
    if (!field_to_lines_impl(&us,
                             &unif,
                             nodesX->getValue(),
                             nodesY->getValue(),
                             nodesZ->getValue(),
                             stride->getValue(),
                             &ugeom,
                             NULL,
                             NULL))
    {
        sendError("field_to_lines_impl failed");
        return true;
    }

    return true;
}

FieldToLines::FieldToLines(const std::string &name, int moduleId, mpi::communicator comm)
: vistle::Module(name, moduleId, comm)
{
    lineField = createInputPort("lineField", "Line Data");

    lines = createOutputPort("lines", "Lines From Field");

    nodesX = addIntParameter("nodesX", "number of nodes in grid in x-direction", 10);
    nodesY = addIntParameter("nodesY", "number of nodes in grid in y-direction", 10);
    nodesZ = addIntParameter("nodesZ", "number of nodes in grid in z-direction", 10);
    stride = addIntParameter("stride", "take each stride\'th line in each dimension", 1);
    setParameterRange<Integer>(stride, 1, INT_MAX);
}

MODULE_MAIN(FieldToLines)
