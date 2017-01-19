#ifndef TRIANGULATE_H
#define TRIANGULATE_H

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

namespace Triangulate
{

    Cu::Matrix Triangulate( const std::vector<Cu::Matrix>& P,
                            const std::vector<Cu::Matrix>& x );

    inline Cu::Matrix pinv( const Cu::Matrix& P); ///< Compute the moore penrose pseudo inverse
    inline Cu::Matrix CrossMatrix( const Cu::Matrix& M ); ///< Compute the cross matrix
    Cu::Matrix FFromP( const Cu::Matrix& P1, const Cu::Matrix& P2 ); ///< Compute the fundamental matrix from a pair of prjection matrices
}

#endif
