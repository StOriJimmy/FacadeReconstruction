#ifndef QUADRIC_H
#define QUADRIC_H

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

#include "stdint.h"
#include <vector>

#include "Matrix.h"

namespace Geometry
{
    class Quadric
    {
    public:
        class Parameters
        {
        public:
            double m_A[3][3];
            double m_b[3];
            double m_c;
        };

        Quadric();
        Quadric(const double* data, const uint32_t N, const double sigma);
        Quadric(const Parameters& params);

        Parameters GetParameters() const; ///< Returns the parameters
        double Fit(const double* data, const uint32_t N, const double sigma); ///< Fit to the data

        VerticesFaces GetMeshRep(const uint32_t N, const double *data, const uint32_t NPts) const; ///< Get a mesh representation
        void GetBoundedMeshRep(VerticesFaces& vf, const double *data, const uint32_t N) const;
        std::vector<bool> GetPointCloudBounds(const uint32_t StartN, const double* data, const uint32_t N) const;
        VerticesFaces GetPrincipleCylinder(const uint32_t N, const double *data, const uint32_t NPts) const; ///< Return a  mesh representation for the principle cylinder
    private:
        VerticesFaces GenerateUnitCircle(const uint32_t N ) const; ///< Create a unit circle
        VerticesFaces GenerateUnitCylinder(const uint32_t N , const uint32_t principleDimension) const; ///< Create a unit cylinder

        VerticesFaces GenerateEllipsoid( const uint32_t N, const double *data, const uint32_t NPts ) const; ///< Create an ellipsoid
        VerticesFaces GeneratePrincipleCylinder( const uint32_t N, const double *data, const uint32_t NPts) const; ///< Create an ellipsoid

        Parameters m_params;
    };
}

#endif // QUADRIC_H

