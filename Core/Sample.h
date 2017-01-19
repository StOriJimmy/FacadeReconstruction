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

namespace Sample
{

    class Generator
    {
    public:
        Generator( );

        std::vector<int32_t> SampleUniformInteger(const int32_t min, const int32_t max, const uint32_t NSamples);
        std::vector<uint32_t> SampleUniformIntegerUI(const uint32_t min, const uint32_t max, const uint32_t NSamples);

        std::vector<uint32_t> SampleDiscrete1D( const Cu::Matrix& distribution, const uint32_t N );
        std::vector<uint32_t> SampleDiscrete1D( std::vector<double> dist, const uint32_t N );

        std::vector<std::pair<uint32_t, uint32_t> > SampleDiscrete2D( const Cu::Matrix& distribution, const uint32_t N );

        Cu::Matrix SampleMultivariateNormal( const Cu::Matrix& mu, const Cu::Matrix& Sigma, const uint32_t N );

        std::vector<std::vector<double> > Sample3DEdge( const std::vector<Cu::Matrix>& edges,
                                                       const std::vector<Cu::Matrix>& P,
                                                       const uint32_t K);
    protected:
        std::default_random_engine m_e;
        uint32_t seed;
    };
}

#endif // SAMPLER_H
