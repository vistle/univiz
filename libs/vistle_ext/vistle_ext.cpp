// helpers for Vistle
// filip sadlo
// cgl eth 2007

#include <vector>

#include "vistle_ext.h"

#include <cstdio>

using namespace vistle;

UnstructuredGrid::ptr generateUniformUSG(const char *name,
                                         float originX, float originY, float originZ,
                                         int cellsX, int cellsY, int cellsZ,
                                         float cellSize)
{ // generates uniform hexahedral USG

    Index ncells = cellsX * cellsY * cellsZ;
    Index nnodes = (cellsX + 1) * (cellsY + 1) * (cellsZ + 1);

    UnstructuredGrid::ptr grid(new UnstructuredGrid(ncells, ncells*8, nnodes));
    Index *elemList = grid->el().data();
    Index *cornerList = grid->cl().data();
    Scalar *coordX = grid->x().data();
    Scalar *coordY = grid->y().data();
    Scalar *coordZ = grid->z().data();
    unsigned char *typeList = grid->tl().data();


    if (!elemList || !cornerList || !coordX || !coordY || !coordZ || !typeList)
    {
        printf("allocation failed\n");
        return UnstructuredGrid::ptr();
    }

    // set coordinates and connectivity
    Scalar *xp = coordX;
    Scalar *yp = coordY;
    Scalar *zp = coordZ;
    int n = 0;
    int c = 0;
    for (int z = 0; z < cellsZ + 1; z++)
    {
        for (int y = 0; y < cellsY + 1; y++)
        {
            for (int x = 0; x < cellsX + 1; x++)
            {
                *(xp++) = originX + x * cellSize;
                *(yp++) = originY + y * cellSize;
                *(zp++) = originZ + z * cellSize;

                if ((x < cellsX) && (y < cellsY) && (z < cellsZ))
                {
                    int nX = cellsX + 1;
                    int nXY = nX * (cellsY + 1);
                    int nXYPX = nXY + nX;
                    int list[8] = {
                        n, n + 1, n + nX + 1, n + nX,
                        n + nXY, n + nXY + 1, n + nXYPX + 1, n + nXYPX
                    };

                    std::copy(list, list+8, cornerList + c * 8);
                    elemList[c] = c * 8;
                    typeList[c] = UnstructuredGrid::HEXAHEDRON;

                    c++;
                }
                n++;
            }
        }
    }

    return grid;
}
