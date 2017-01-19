#ifndef Matrix_H
#define Matrix_H

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

#include "Eigen/Dense"
#include "Eigen/LU"

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

// These matrices are dynamic and slower than their
// static counterparts. The speed difference is in the allocation
// of memory on the heap.
namespace Cu
{ // The namespace is necessary to avoid conflicts with another library, so it is also quite short
    typedef Eigen::MatrixXd Matrix;
    typedef Eigen::VectorXd Vector;
    void print_matrix(const Cu::Matrix&, const std::string&);
    void print_matrix_octave(const Cu::Matrix&, const std::string&);
    void print_matrix_octave(const std::vector<std::vector<double>>& A, const std::string& name);
    void print_matrix_octave(const double* data, uint32_t M, uint32_t N, const std::string& name);
}

typedef std::pair< std::vector< std::vector<double> >, std::vector< std::vector<uint32_t> > > VerticesFaces;

Cu::Matrix ConvertToEigen(const double *in, const uint32_t M, const uint32_t N );

namespace MatrixFuncs
{
    void HomogeneousNormalise( Cu::Matrix& ); ///< Normalise a Cu::Cu::Matrix using homogeneous form
    std::vector<uint32_t> find( const Cu::Matrix& in, const std::function<bool(float)>& func  ); ///< Find indexes in the Cu::Cu::Matrix which satisfy the lambda function
    std::pair<Cu::Matrix, Cu::Matrix> EigenValues( const Cu::Matrix& A  );
}

namespace MeshFuncs
{
    void RemoveVerticesFaces(VerticesFaces& vf, std::vector<uint32_t> &VerticesToRemove );
        ///< Remove all faces and vertices from vf which contain the faces in FacesToRemove - note the second list is sorted
    void WritePly(const VerticesFaces& vf, const std::string filename);
}

namespace Funcs
{
    double normal_pdf(const double x, const double m, const double s);
}

#endif // Cu::Matrix_H
