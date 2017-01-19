//API functions v1.0

#ifndef BACK_END_HPP_
#define BACK_END_HPP_

/* ----------------------------------------------------------------------
 * Copyright (C) 2016 Daniel Beale and Luca Benedetti. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * ---------------------------------------------------------------------- */

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "datastore.hpp"

#include "Quadric.h"
#include "Sample.h"
#include "PolyGTM.h"

    /**
     * Convert a collection of sketches to an edge.
     */
    void SketchesToEdge(const QList<Sketch*> &sketches,
                        const QMap<rasterID, Raster*> &rasters,
                        const Mesh* point_cloud,
                        QList< vcg::Point3f > &edge_points,
                        const uint32_t NumberOfNewSamples=0,
                        const double aspectratio = 1.0);


    /**
     * Convert a collection of edges to a surface.
     */
    void EdgesToSurface(const QList<Edge *> &edges,
                        const SurfaceInputParams *params,
                        CMesh &surface_out);

//    /**
//     * Create a Poisson Surface from a mesh
//     */
//    void CreatePoissonSurface(CMesh &inmesh , CMesh &outmesh);


    /**
    * Merge the surfaces with poisson reconstruction, optionally colorize them using the mlp file and saves the results in filename.
    */
   bool SaveTotal(QString filename, QList<Mesh *> surfaces, bool make_poisson=true, bool colorize=false, QString mlp_filename="");


#endif
