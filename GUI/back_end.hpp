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
 * @brief Convert a collection of sketches to an edge.
 * @param sketches A QList of Sketches
 * @param rasters A QMap of rasters
 * @param point_cloud The input point cloud
 * @param edge_points A QList of edge points
 * @param NumberOfNewSamples The number of samples to draw from the sketches
 * @param aspectratio The devicepixelratio
 */
void SketchesToEdge(const QList<Sketch*> &sketches,
                    const QMap<rasterID, Raster*> &rasters,
                    const Mesh* point_cloud,
                    QList< vcg::Point3f > &edge_points,
                    const uint32_t NumberOfNewSamples=0,
                    const double aspectratio = 1.0);


/**
 * @brief Convert a collection of edges to a surface.
 * @param edges A QList of edge maps
 * @param params The parameters of the model
 * @param surface_out A CMesh which is written to
 */
void EdgesToSurface(const QList<Edge *> &edges,
                    const SurfaceInputParams *params,
                    CMesh &surface_out);

/**
 * @brief SaveTotal.
 * Merge the surfaces with poisson reconstruction, optionally colorize them using the mlp file and saves the results in filename.
 * @param filename The name of the file to save to
 * @param surfaces A QList of surfaces
 * @param make_poisson If true, run the poisson reconstruction using MeshLab
 * @param colorize If true apply colours to the surface
 * @param mlp_filename The mlp file to save to (only used if colourize is set to true)
 * @return true on success
 */
bool SaveTotal(QString filename, QList<Mesh *> surfaces, bool make_poisson=true, bool colorize=false, QString mlp_filename="");


#endif
