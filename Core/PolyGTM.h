#ifndef POLYGTM_H
#define POLYGTM_H

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

#include <stdint.h>
#include <cmath>
#include <numeric>
#include <limits>

#include "Matrix.h"

/**
 * A collection of functions, structures and classes for computing a Generative Topographic Map with a latent
 * space of dimension 1.
 *
 * For more information about generative topographic maps, please see the paper,
 *      "GTM: The generative topographic mapping" Bishop, Svensén and Williams. Neural Computation, 1998.
 */
namespace GTM
{
    /**
     * @brief The Output struct.
     * A structure containing all of the required data for a polynomial spline, and its probability distribution.
     */
    struct Output
    {
        Cu::Matrix W;   ///< The coefficient matrix
        Cu::Matrix Phi; ///< The basis function matrix
        Cu::Matrix Pi;  ///< The probability coeffiients
        double BetaInv; ///< The variance
    };

    /**
     * @brief The Input struct.
     * A structure containing all of the data and parameters required to fit a GTM.
     */
    struct Input
    {
        const double* data;     ///< A pointer to the first point in a column major data matrix, each column an observation.
        const uint32_t M;       ///< The number of dimensions, or rows in the matrix
        const uint32_t N;       ///< The number of observations
        const uint32_t K;       ///< The number of components in the GTM
        const uint32_t MaxIter; ///< The maximum number of iterations
        const double lambda;    ///< The variance prior
        const uint32_t degree;  ///< The degree of polynomial to fit
    };

    /**
     * @brief CreateStartPointUsingPCA.
     * Create a start point for the GTM using PCA. The GTM computation is iterative and so it requires
     * a decent starting point. This method uses PCA as a method for dimensionality reduction.
     * In a lot of cases the result of this function is enough, without further optimisation.
     * @param input The unput parameters and data.
     * @return An output struct containing the starting point
     */
    Output CreateStartPointUsingPCA( const Input& input);

    /**
     * @brief Polynomial.
     * Run the EM algorithm to fit the GTM with a polynomial basis. This function implicitly calls the CreateStartPointUsingPCA, and
     * so one only need to provide the input data and parameters.
     * @param input The unput data and parameters
     * @return The output struct.
     */
    Output Polynomial( Input& input );
}

#endif // POLYGTM_H

