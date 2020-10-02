// Unification Library for Modular Visualization Systems
//
// Geometry
//
// CGL ETH Zuerich
// Filip Sadlo 2006 - 2008

#include "unigeom.h"

#ifdef COVISE
#include <do/coDoData.h>
#endif

#ifdef VISTLE
using namespace vistle;
#endif

#ifdef AVS
UniGeom::UniGeom(GEOMedit_list *geom)
{
    avsGeomEditList = geom;
}
#endif

#ifdef COVISE
#ifdef COVISE5
UniGeom::UniGeom(coOutPort *geom, coOutPort *normals)
#else
UniGeom::UniGeom(coOutputPort *geom, coOutputPort *normals)
#endif
{
    covGeomIn = NULL;
    covGeom = geom;
    covNormals = normals;
}

#ifndef COVISE5
UniGeom::UniGeom(coInputPort *geom)
{
    if (geom->getCurrentObject()->isType("SPHERE"))
    {
        printf("unigeom: unsupported type: sphere\n");
        exit(1); // ###
    }
    else if (geom->getCurrentObject()->isType("POINTS"))
    {
        printf("unigeom: unsupported type: points\n");
        exit(1); // ###
    }
    else if (geom->getCurrentObject()->isType("LINES"))
    {
        geomType = GT_LINE;
    }
    else if (geom->getCurrentObject()->isType("POLYGN"))
    {
        geomType = GT_POLYHEDRON;
    }
    else if (geom->getCurrentObject()->isType("TRIANG"))
    {
        printf("unigeom: unsupported type: triangle strip\n");
        exit(1); // ###
    }
    else if (geom->getCurrentObject()->isType("VOLUME"))
    {
        printf("unigeom: unsupported type: volume\n");
        exit(1); // ###
    }

    covGeomIn = geom;
    covGeom = NULL;
    covNormals = NULL;

#if 0 // ###### DELETEME
  coDoLines * lin = ((coDoLines *) geom->getCurrentObject());
  float *x, *y, *z;
  int *corL, *linL;
  lin->getAddresses(&x, &y, &z, &corL, &linL);

  printf("pntCnt=%d vertCnt=%d lineCnt=%d\n",
         lin->get_no_of_points(),
         lin->get_no_of_vertices(),
         lin->get_no_of_lines()
         );
#endif
}
#endif

#endif

#ifdef VISTLE
UniGeom::UniGeom(vistle::Module *mod, vistle::Port *port, Object::const_ptr src)
: mod(mod)
, sourceObject(src)
, outputPort(port)
{
}

UniGeom::UniGeom(vistle::PortTask *task, vistle::Port *port, Object::const_ptr src)
: task(task)
, sourceObject(src)
, outputPort(port)
{
}
#endif

#ifdef VTK
UniGeom::UniGeom(vtkPolyData *vtkPolyD)
{
    vtkPolyDat = vtkPolyD;
    outputPoints = NULL;
    outputCells = NULL;
    outputNormals = NULL;
}
#endif

UniGeom::~UniGeom(void)
{
}

void UniGeom::createObj(int geomT)
{
#ifdef AVS
    switch (geomT)
    {
    case GT_LINE:
    {
        avsGeomObj = GEOMcreate_obj(GEOM_POLYTRI, NULL);
    }
    break;
    case GT_POLYHEDRON:
    {
        avsGeomObj = GEOMcreate_obj(GEOM_POLYHEDRON, NULL);
    }
    break;
    default:
        fprintf(stderr, "UniGeom::createObj: error: unsupported geom type\n");
    }
    // ok? ###
    *avsGeomEditList = GEOMinit_edit_list(*avsGeomEditList);
#endif

#ifdef COVISE
    geomType = geomT;
#endif

#ifdef VISTLE
    geomType = geomT;
    switch (geomT) {
    case GT_LINE:
        outLine.reset(new Lines(0,0,0));
        break;
    case GT_POLYHEDRON:
        outPoly.reset(new Polygons(0,0,0));
        break;
    }

#endif

#ifdef VTK
    geomType = geomT;
    outputPoints = vtkPoints::New();
    outputCells = vtkCellArray::New();
    numPoints = 0;
    numCells = 0;
#endif
}

void UniGeom::addPolyline(float *vertices, float *colors, int nvertices)
{
#ifdef AVS
    GEOMadd_polyline(avsGeomObj, (float *)vertices, colors, nvertices, GEOM_COPY_DATA);
#endif

#ifdef COVISE

    // ##### TODO: colors

    std::vector<float> fv;

    for (int v = 0; v < nvertices; v++)
    {
        fv.push_back(vertices[v * 3 + 0]);
        fv.push_back(vertices[v * 3 + 1]);
        fv.push_back(vertices[v * 3 + 2]);
    }

    lines.push_back(fv);
#endif

#ifdef VISTLE
    assert(geomType == GT_LINE);
    // ##### TODO: colors
    auto &x = outLine->x();
    auto &y = outLine->y();
    auto &z = outLine->z();
    auto &el = outLine->el();
    auto &cl = outLine->cl();
    for (int v=0; v<nvertices; ++v) {
        cl.emplace_back(cl.size());
        x.emplace_back(vertices[v*3+0]);
        y.emplace_back(vertices[v*3+1]);
        z.emplace_back(vertices[v*3+2]);
    }
    el.emplace_back(cl.size());
#endif

#ifdef VTK

    // ##### TODO: colors

    // insert points and cell
    outputCells->InsertNextCell(nvertices);
    for (int v = 0; v < nvertices; v++)
    {
        double pnt[3] = { vertices[v * 3 + 0], vertices[v * 3 + 1], vertices[v * 3 + 2] };
        outputPoints->InsertNextPoint(pnt);
        outputCells->InsertCellPoint(numPoints++);
    }
    numCells++;
#endif
}

void UniGeom::addVertices(float *verts, int nvertices)
{
#ifdef AVS
    GEOMadd_vertices(avsGeomObj, verts, nvertices, GEOM_COPY_DATA);
#endif

#ifdef COVISE
    for (int v = 0; v < nvertices; v++)
    {
        vertices.push_back(verts[v * 3 + 0]);
        vertices.push_back(verts[v * 3 + 1]);
        vertices.push_back(verts[v * 3 + 2]);
    }
#endif

#ifdef VISTLE
    assert(geomType == GT_POLYHEDRON);
    auto &x = outPoly->x();
    auto &y = outPoly->y();
    auto &z = outPoly->z();
    for (int v=0; v<nvertices; ++v) {
        x.emplace_back(verts[v*3+0]);
        y.emplace_back(verts[v*3+1]);
        z.emplace_back(verts[v*3+2]);
    }
#endif

#ifdef VTK
    for (int v = 0; v < nvertices; v++)
    {
        double pnt[3] = { verts[v * 3 + 0], verts[v * 3 + 1], verts[v * 3 + 2] };
        outputPoints->InsertNextPoint(pnt);
        numPoints++;
    }
#endif
}

void UniGeom::addPolygon(int nvertices, int *indices)
{ // here, vertex indices are zero-based!
// note that AVS uses 1-based indices!
#ifdef AVS
    int *buf = new int[nvertices];
    for (int v = 0; v < nvertices; v++)
    {
        buf[v] = indices[v] + 1;
    }
    GEOMadd_polygon(avsGeomObj, nvertices, buf, 0, GEOM_COPY_DATA);
    delete[] buf;
#endif

#ifdef VISTLE
    assert(geomType == GT_POLYHEDRON);
    // ##### TODO: colors
    auto &cl = outPoly->cl();
    auto &el = outPoly->el();
    for (int v=0; v<nvertices; ++v) {
        cl.emplace_back(indices[v]);
    }
    el.emplace_back(cl.size());
#endif

#ifdef COVISE
    std::vector<int> poly;
    for (int i = 0; i < nvertices; i++)
    {
        poly.push_back(indices[i]);
    }
    polygons.push_back(poly);
#endif

#ifdef VTK
    // ### DELETEME: outputCells->InsertNextCell((vtkIdType) nvertices, (vtkIdType*) indices);
    vtkIdType ids[nvertices];
    for (int i = 0; i < nvertices; i++)
    {
        ids[i] = (vtkIdType)indices[i];
    }
    outputCells->InsertNextCell((vtkIdType)nvertices, ids);
    numCells++;
#endif
}

void UniGeom::generateNormals(void)
{
#ifdef AVS
    GEOMgen_normals(avsGeomObj, 0);
#endif

#ifdef COVISE
    // ### this is not really needed, since normals are computed by separate
    // module GenNormals
    if (covNormals)
    {
        int numPoints = int(vertices.size() / 3);
        int numCells = int(polygons.size());

        normals.clear();

        // init
        for (int n = 0; n < numPoints; n++)
        {
            normals.push_back(0.0);
            normals.push_back(0.0);
            normals.push_back(0.0);
        }

        // accumulate
        for (int p = 0; p < numCells; p++)
        {
            int nvts = int(polygons[p].size());

            // compute and sum normals of current polygon at its corners
            for (int v = 0; v < nvts; v++)
            {

                int vCurr, vPrev, vNext;
                vCurr = polygons[p][v];
                if (v == 0)
                    vPrev = polygons[p][nvts - 1];
                else
                    vPrev = polygons[p][v - 1];

                if (v == nvts - 1)
                    vNext = polygons[p][0];
                else
                    vNext = polygons[p][v + 1];

                vec3 curr, prev, next, norm;
                getVertex(vPrev, prev);
                getVertex(vCurr, curr);
                getVertex(vNext, next);
                vec3sub(prev, curr, prev);
                vec3sub(next, curr, next);
                vec3cross(prev, next, norm);
                vec3nrm(norm, norm);

                // orient normal
                vec3 existing = { normals[vCurr * 3 + 0],
                                  normals[vCurr * 3 + 1],
                                  normals[vCurr * 3 + 2] };
                if (vec3dot(existing, norm) < 0)
                {
                    vec3scal(norm, -1.0, norm);
                }

                normals[vCurr * 3 + 0] += norm[0];
                normals[vCurr * 3 + 1] += norm[1];
                normals[vCurr * 3 + 2] += norm[2];
            }
        }

        // normalize
        for (int n = 0; n < numPoints; n++)
        {
            vec3 norm = { normals[n * 3 + 0], normals[n * 3 + 1], normals[n * 3 + 2] };
            vec3nrm(norm, norm);
            normals[n * 3 + 0] = -norm[0];
            normals[n * 3 + 1] = -norm[1];
            normals[n * 3 + 2] = -norm[2];
        }
    }
//printf("UniGeom::generateNormals ERROR: not yet implemented\n");
#endif

#ifdef VTK
#if 1 // ### this is not really needed, since normals are computed by separate
    // module NormalsGeneration in paraview see e.g. Contour module
    outputNormals = vtkFloatArray::New();
    outputNormals->SetNumberOfComponents(3);
    outputNormals->SetNumberOfTuples(numPoints);
    outputNormals->SetName("Normals");
    float *nptr = outputNormals->GetPointer(0);

    // init
    for (int n = 0; n < numPoints; n++)
    {
        nptr[n * 3 + 0] = 0.0;
        nptr[n * 3 + 1] = 0.0;
        nptr[n * 3 + 2] = 0.0;
    }

    // accumulate
    vtkIdType *pptr = outputCells->GetPointer();
    for (int p = 0; p < numCells; p++)
    {
        int nvts = *pptr;
        pptr++;
        // pptr points to nvts vertex indices

        // compute and sum normals of current polygon at its corners
        for (int v = 0; v < nvts; v++)
        {

            int vCurr, vPrev, vNext;
            vCurr = pptr[v];
            if (v == 0)
                vPrev = pptr[nvts - 1];
            else
                vPrev = pptr[v - 1];

            if (v == nvts - 1)
                vNext = pptr[0];
            else
                vNext = pptr[v + 1];

            vec3 curr, prev, next, norm;
            outputPoints->GetPoint(vPrev, prev);
            outputPoints->GetPoint(vCurr, curr);
            outputPoints->GetPoint(vNext, next);
            vec3sub(prev, curr, prev);
            vec3sub(next, curr, next);
            vec3cross(prev, next, norm);
            vec3nrm(norm, norm);

            // orient normal
            vec3 existing = { nptr[vCurr * 3 + 0],
                              nptr[vCurr * 3 + 1],
                              nptr[vCurr * 3 + 2] };
            if (vec3dot(existing, norm) < 0)
            {
                vec3scal(norm, -1.0, norm);
            }

            nptr[vCurr * 3 + 0] += norm[0];
            nptr[vCurr * 3 + 1] += norm[1];
            nptr[vCurr * 3 + 2] += norm[2];
        }
        pptr += nvts;
    }

    // normalize
    for (int n = 0; n < numPoints; n++)
    {
        vec3 norm = { nptr[n * 3 + 0], nptr[n * 3 + 1], nptr[n * 3 + 2] };
        vec3nrm(norm, norm);
        nptr[n * 3 + 0] = -norm[0];
        nptr[n * 3 + 1] = -norm[1];
        nptr[n * 3 + 2] = -norm[2];
    }
//printf("UniGeom::generateNormals ERROR: not yet implemented\n");
#endif
#endif
}

#ifndef COVISE
void UniGeom::assignObj(const char *name)
#else
void UniGeom::assignObj(const char *)
#endif
{
#ifdef AVS
    char w[4096]; // ###
    strcpy(w, name);
    GEOMedit_geometry(*avsGeomEditList, w, avsGeomObj);
    GEOMdestroy_obj(avsGeomObj);
#endif

#ifdef COVISE
    // creating objects here

    switch (geomType)
    {
    case GT_LINE:
    {

        // get sizes
        int num_lines = int(lines.size());
        int num_points = 0, num_corners = 0;
        for (int l = 0; l < (int)lines.size(); l++)
        {
            num_points += int(lines[l].size() / 3);
            num_corners += int(lines[l].size() / 3); // ### actually lines can not share point
        }

//printf("%d lines, %d points, %d corners\n", num_lines, num_points, num_corners);

// alloc
#ifdef COVISE5
        DO_Lines *lin = new DO_Lines
#else
        coDoLines *lin = new coDoLines
#endif
            (covGeom->getObjName(), num_points, num_corners, num_lines);
        if (!lin)
            fprintf(stderr, "UniGeom::assignObj: out of memory\n");

        // setup
        {
            float *x, *y, *z;
            int *corL, *linL;
#ifdef COVISE5
            lin->get_adresses(&x, &y, &z, &corL, &linL);
#else
            lin->getAddresses(&x, &y, &z, &corL, &linL);
//lin->get_addresses(&x, &y, &z, &corL, &linL);
#endif

            int vertIdx = 0;
            for (int l = 0; l < (int)lines.size(); l++)
            {
                linL[l] = vertIdx;

                for (int ll = 0; ll < (int)lines[l].size() / 3; ll++)
                {
                    x[vertIdx] = lines[l][ll * 3 + 0];
                    y[vertIdx] = lines[l][ll * 3 + 1];
                    z[vertIdx] = lines[l][ll * 3 + 2];

                    corL[vertIdx] = vertIdx;
                    vertIdx++;
                }
            }
        }

// assign object to port
#ifdef COVISE5
        covGeom->setObj(lin);
#else
        covGeom->setCurrentObject(lin);
#endif
    }
    break;

    case GT_POLYHEDRON:
    {

        // get sizes
        int num_points = int(vertices.size() / 3);
        int num_polygons = int(polygons.size());
        int num_corners = 0;
        for (int p = 0; p < (int)polygons.size(); p++)
        {
            num_corners += int(polygons[p].size());
        }

//printf("%d lines, %d points, %d corners\n", num_lines, num_points, num_corners);

// alloc
#ifdef COVISE5
        DO_Polygons *poly = new DO_Polygons
#else
        coDoPolygons *poly = new coDoPolygons
#endif
            (covGeom->getObjName(), num_points, num_corners, num_polygons);
        if (!poly)
            fprintf(stderr, "UniGeom::assignObj: out of memory\n");

        // setup
        {
            float *x, *y, *z;
            int *corL, *polyL;
#ifdef COVISE5
            poly->get_adresses(&x, &y, &z, &corL, &polyL);
#else
            poly->getAddresses(&x, &y, &z, &corL, &polyL);
//poly->get_addresses(&x, &y, &z, &corL, &polyL);
#endif

            int vertIdx = 0;
            for (int p = 0; p < (int)polygons.size(); p++)
            {
                //polyL[p] = polygons[p][0];
                polyL[p] = vertIdx;

                for (int pp = 0; pp < (int)polygons[p].size(); pp++)
                {
                    corL[vertIdx] = polygons[p][pp];
                    vertIdx++;
                }
            }

            vertIdx = 0;
            for (int v = 0; v < (int)vertices.size() / 3; v++)
            {
                x[vertIdx] = vertices[v * 3 + 0];
                y[vertIdx] = vertices[v * 3 + 1];
                z[vertIdx] = vertices[v * 3 + 2];
                vertIdx++;
            }
        }

#if 0 // #### QUICK TEST
    {
      const int width = 8;
      const int height = 2;
      const int textureComponents = 3;
      unsigned char imageBuf[width*height*textureComponents] =
        {
          255, 0, 0,
          0, 255, 0,
          255, 255, 0,
          0, 0, 255,
          255, 255, 255,
          255, 255, 255,
          255, 255, 255,
          255, 255, 255,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127,
          127, 127, 127
        };
      
      DO_PixelImage *img = new DO_PixelImage("tmp", width, height, textureComponents, textureComponents, (const char *) imageBuf);
      
      float *txCoord[2];
      float mapping[] =                              // X coord
        {
          1.0,0.0,0.0,1.0,
          1.0,1.0,0.0,0.0                             // Y coord
        };
      txCoord[0] = mapping;
      txCoord[1] = mapping+4;

      float *x, *y, *z;
      int *corL, *polyL;
      poly->getAddresses(&x, &y, &z, &corL, &polyL);
      
      coDoTexture *texture = new coDoTexture("tmp2",img,
                                             0,4,0,4 /* no_points */,
                                             corL,4 /* no_points */,
                                             txCoord);

      // make a coDoGeometry...
      coDoGeometry *dogeom = new coDoGeometry(covGeom->getObjName(),poly);
      dogeom->set_texture(0,texture);
      
      covGeom->setCurrentObject(dogeom);

    
    }
#else

// assign object to port
#ifdef COVISE5
        covGeom->setObj(poly);
#else
        covGeom->setCurrentObject(poly);
#endif

#endif

        // output normals
        if (covNormals)
        {
            coDoVec3 *normalsOut;
            normalsOut = new coDoVec3(covNormals->getObjName(), int(vertices.size() / 3));

            float *u, *v, *w;
            normalsOut->getAddresses(&u, &v, &w);

            int numPoints = int(vertices.size() / 3);

            for (int p = 0; p < numPoints; p++)
            {
                u[p] = normals[p * 3 + 0];
                v[p] = normals[p * 3 + 1];
                w[p] = normals[p * 3 + 2];
            }

            covNormals->setCurrentObject(normalsOut);
        }
    }
    break;

    default:
        fprintf(stderr, "UniGeom::assignObj: error: unsupported geom type\n");
    }
#endif

#ifdef VISTLE
    switch (geomType) {
    case GT_LINE:
        outLine->copyAttributes(sourceObject);
        if (task)
            task->addObject(outputPort, outLine);
        else if (mod)
            mod->addObject(outputPort, outLine);
        break;
    case GT_POLYHEDRON:
        outPoly->copyAttributes(sourceObject);
        if (task)
            task->addObject(outputPort, outPoly);
        else if (mod)
            mod->addObject(outputPort, outPoly);
        break;
    }
#endif

#ifdef VTK
    switch (geomType)
    {
    case GT_LINE:
    {
        vtkPolyDat->SetPoints(outputPoints);
        vtkPolyDat->SetLines(outputCells);
    }
    break;

    case GT_POLYHEDRON:
    {
        vtkPolyDat->SetPoints(outputPoints);
        if (outputNormals)
            vtkPolyDat->GetPointData()->SetNormals(outputNormals);
        vtkPolyDat->SetPolys(outputCells);
    }
    break;

    default:
        fprintf(stderr, "UniGeom::assignObj: error: unsupported geom type\n");
    }
    outputPoints->Delete();
    outputCells->Delete();
    if (outputNormals)
        outputNormals->Delete();
#endif
}

void UniGeom::getVertex(int vertex, vec3 pos)
{
#ifdef AVS
    // ###### TODO: HACK: fixed to polyhedra
    pos[0] = PH(avsGeomObj).verts.l[vertex * 3 + 0];
    pos[1] = PH(avsGeomObj).verts.l[vertex * 3 + 1];
    pos[2] = PH(avsGeomObj).verts.l[vertex * 3 + 2];
#endif

#ifdef COVISE
    if (covGeom)
    {
        pos[0] = vertices[(vertex)*3 + 0];
        pos[1] = vertices[(vertex)*3 + 1];
        pos[2] = vertices[(vertex)*3 + 2];
    }
    else if (covGeomIn)
    {
        if (geomType == GT_LINE)
        {
            coDoLines *lin = ((coDoLines *)covGeomIn->getCurrentObject());
            float *x, *y, *z;
            int *corL, *linL;
            lin->getAddresses(&x, &y, &z, &corL, &linL);
            pos[0] = x[vertex];
            pos[1] = y[vertex];
            pos[2] = z[vertex];
        }
        else if (geomType == GT_POLYHEDRON)
        {
            printf("UniGeom::getVertex not yet implemented for polygon type\n");
        }
    }
#endif

#ifdef VISTLE
    auto &x = geomType==GT_LINE ? outLine->x() : outPoly->x();
    auto &y = geomType==GT_LINE ? outLine->y() : outPoly->y();
    auto &z = geomType==GT_LINE ? outLine->z() : outPoly->z();
    pos[0] = x[vertex];
    pos[1] = y[vertex];
    pos[2] = z[vertex];
#endif

#ifdef VTK
    outputPoints->GetPoint(vertex, pos);
#endif
}

int UniGeom::getVertexCnt()
{
#ifdef AVS
    printf("UniGeom::getVertexCnt: not yet implemented\n");
#endif
#ifdef COVISE
    if (covGeom)
    {
        printf("UniGeom::getVertexCnt: not yet implemented for output object\n");
    }
    else if (covGeomIn)
    {
        if (geomType == GT_LINE)
        {
            coDoLines *lin = ((coDoLines *)covGeomIn->getCurrentObject());
            return lin->getNumVertices();
        }
        else if (geomType == GT_POLYHEDRON)
        {
            printf("UniGeom::getVertexCnt: not yet implemented for polygon type\n");
        }
    }
#endif
#ifdef VISTLE
    switch (geomType) {
    case GT_LINE:
        return outLine->getSize();
        break;
    case GT_POLYHEDRON:
        return outPoly->cl().size();
        break;
    }
#endif
#ifdef VTK
    printf("UniGeom::getVertexCnt: not yet implemented\n");
#endif
    return 0;
}
