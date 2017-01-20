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

/**
 * \brief This namespace is an interface into the Eigen library. It is required for some of the
 * functions and classes and is used to avoid naming confilct in the octave api.
 */
namespace Cu
{
    typedef Eigen::MatrixXd Matrix; ///< A double precision dynamic Eigen matrix
    typedef Eigen::VectorXd Vector; ///< A double precision dynamic Eigen vector

    /**
    * \brief Print an Eigen matrix to stdout
    * @param M The matrix to print
    * @param name The name of the matrix
    */
    void print_matrix(const Cu::Matrix&, const std::string&);

    /**
    * \brief Print a matrix to an octave file
    * @param M The matrix to print
    * @param name The file path
    */
    void print_matrix_octave(const Cu::Matrix&, const std::string&);

    /**
    * \brief  Print a vector of vectors to an octave file
    * @param A The vector of vectors, each vector must be the same length
    * @param name The file path
    */
    void print_matrix_octave(const std::vector<std::vector<double>>& A, const std::string& name);

    /**
    * \brief  Print column major matrix to an octave file
    * @param data A pointer to the first element in the contiguous array
    * @param M the number of rows
    * @param N the number of columns
    * @param name The file path
    */
    void print_matrix_octave(const double* data, uint32_t M, uint32_t N, const std::string& name);
}

/**
 * \brief A collection of functions and classes for fitting surfaces to point clouds.
 */
namespace Core
{
    typedef std::pair< std::vector< std::vector<double> >, std::vector< std::vector<uint32_t> > > VerticesFaces;

    /**
     * \brief Convert a column major matrix to an Eigen matrix.
     * This function uses the Eigen internal map method, and so it is as fast as a mem copy.
     * @param in A pointer to the first element in a contiguous array
     * @param M the number of rows
     * @param N the number of columns
     * @return An Eigen matrix
     */
    Cu::Matrix ConvertToEigen(const double *in, const uint32_t M, const uint32_t N );

    /**
    * \brief Normalise a homogeneous vector by dividing each element by the last element.
    * @param A the vector to normalis
    */
    void HomogeneousNormalise( Cu::Matrix& );

    /**
    * \brief Return the indecies in to the matrix 'in' which evaluate to true in the
    * functional 'func'
    * @param in An Eigen matrix
    * @param func A functional which takes a floating point number and returns a boolean
    */
    std::vector<uint32_t> find( const Cu::Matrix& in, const std::function<bool(float)>& func  );

    /**
     * @brief Return the eigenvectors and values of a matrix A.
     * The eigenvectors are normalised by the determinant
     * of the matrix U, to ensure that the determinant is 1 and not -1. Either are valid solutions.
     * Only the real parts of the eigenvectors are returned.
     * @param A A square matrix, positive definite matrix.
     * @return A pair of matrices corresponding to U and S in the eigenvalue factorisation.
     */
    std::pair<Cu::Matrix, Cu::Matrix> EigenValues( const Cu::Matrix& A  );

    /**
    * \brief Remove all faces and vertices from 'vf 'which contain the vertices in the vector
    * VerticesToRemove.
    * @param vf The vertcices and faces
    * @param VerticesToRemove A vector of vertex indices to remove from 'vf'
    */
    void RemoveVerticesFaces(VerticesFaces& vf, std::vector<uint32_t> &VerticesToRemove );

    /**
    * \brief Write a VerticesFaces type to the Stanford ply format.
    * @param vf The vertices and faces
    * @param filename The path to the file
    */
    void WritePly(const VerticesFaces& vf, const std::string filename);

    /**
    * \brief Compute the 1 dimensional Gaussian distribution
    * @param x The value from which to compute the likelihood
    * @param m The mean
    * @param s The variance
    */
    double normal_pdf(const double x, const double m, const double s);
}

#endif // Cu::Matrix_H
