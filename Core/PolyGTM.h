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

namespace GTM
{
    struct Output
    {
        Cu::Matrix W;
        Cu::Matrix Phi;
        Cu::Matrix Pi;
        double BetaInv;
    };

    struct Input
    {
        const double* data;
        const uint32_t M;
        const uint32_t N;
        const uint32_t K;
        const uint32_t MaxIter;
        const double lambda;
        const uint32_t degree;
    };


    Output CreateStartPointUsingPCA( const Input& input); ///< Create a start point using PCA
    Output Polynomial( Input& input ); ///< Run a GTM with a polynomial basis
}

#endif // POLYGTM_H

