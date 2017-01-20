#ifndef SAMPLER_H
#define SAMPLER_H

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

#include "Matrix.h"
#include <random>

namespace Core
{
    /**
     * @brief The Generator class.
     * A class used to generate samples from a collection of probability distributions. This class is
     * necessary because it generates pseudo random number and so keeps an update of how many samples
     * have been taken since the seed.
     */
    class Generator
    {
    public:
        /**
         * @brief Generator.
         * Construct an empty random number generator.
         */
        Generator( );

        /**
         * @brief SampleUniformInteger.
         * Sample a uniform signed 32bit integer with the given parameters
         * @param min The minimum number
         * @param max The maximum number
         * @param NSamples The number of samples to draw
         * @return A vector of length NSamples
         */
        std::vector<int32_t> SampleUniformInteger(const int32_t min, const int32_t max, const uint32_t NSamples);

        /**
         * @brief SampleUniformInteger.
         * Sample a uniform unsigned 32bit integer with the given parameters
         * @param min The minimum number
         * @param max The maximum number
         * @param NSamples The number of samples to draw
         * @return A vector of length NSamples
         */
        std::vector<uint32_t> SampleUniformIntegerUI(const uint32_t min, const uint32_t max, const uint32_t NSamples);

        /**
         * @brief SampleDiscrete1D.
         * Sample from a discrete distribution.
         * @param distribution A 1xK matrix
         * @param N The number of samples to draw
         * @return A vector of unsigned 32bit integers - the indecies
         */
        std::vector<uint32_t> SampleDiscrete1D( const Cu::Matrix& distribution, const uint32_t N );

        /**
         * @brief SampleDiscrete1D.
         * Sample from a discrete distribution.
         * @param distribution A vector of length K
         * @param N The number of samples to draw
         * @return A vector of unsigned 32bit integers - the indecies
         */
        std::vector<uint32_t> SampleDiscrete1D( std::vector<double> dist, const uint32_t N );

        /**
         * @brief SampleDiscrete2D.
         * Sample from a discrete 2D distribution, using Gibbs sampling.
         * @param distribution A KxL matrix
         * @param N The number of samples to draw
         * @return A pair of 32bit unsigned integers, the 2D indecies.
         */
        std::vector<std::pair<uint32_t, uint32_t> > SampleDiscrete2D( const Cu::Matrix& distribution, const uint32_t N );

        /**
         * @brief SampleMultivariateNormal.
         * Draw samples from a multivariate normal distribution.
         * @param mu A Mx1 matrix containing the mean
         * @param Sigma An MxM matrix containing the covariance
         * @param N The number of samples to draw
         * @return A MxN matrix of samples, each column containing a different sample.
         */
        Cu::Matrix SampleMultivariateNormal( const Cu::Matrix& mu, const Cu::Matrix& Sigma, const uint32_t N );

        /**
         * @brief Sample3DEdge.
         * Given a collection of probabilistic images, an their associated projection matrices. Sample
         * a collection of points in 3D. The method uses Gibbs sampling and assumes that the edge maps
         * are conditionally independent given the trajectory they were sampled from.
         * @param edges A vector of matrices, each one a probabilistic edge map
         * @param P A vector of 3x4 projection matrices
         * @param K The number of samples to draw
         * @return A vector of 3D vectors, each one a point.
         */
        std::vector<std::vector<double> > Sample3DEdge( const std::vector<Cu::Matrix>& edges,
                                                       const std::vector<Cu::Matrix>& P,
                                                       const uint32_t K);
    protected:
        std::default_random_engine m_e; ///< The std random number generator
        uint32_t seed;                  ///< The initial seed
    };
}

#endif // SAMPLER_H
