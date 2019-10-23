// helpers for Vistle
// filip sadlo
// cgl eth 2007

#ifndef UNIVIZ_VISTLE_EXT_H
#define UNIVIZ_VISTLE_EXT_H

#include <module/module.h>
#include <core/parameter.h>
#include <core/unstr.h>
#include <vector>
#include "export.h"

// Basic ----------------------------------------------------------------------

vistle::UnstructuredGrid::ptr V_UNIVIZEXPORT generateUniformUSG(const char *name,
                                         float originX, float originY, float originZ,
                                         int cellsX, int cellsY, int cellsZ,
                                         float cellSize);

#endif // UNIVIZ_VISTLE_EXT_H
