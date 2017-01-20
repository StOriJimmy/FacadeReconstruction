
#include "Triangulate.h"

#include <Eigen/SVD>

namespace Core
{
    Cu::Matrix Triangulate( const std::vector<Cu::Matrix>& P,
                            const std::vector<Cu::Matrix>& x )
    {
        uint32_t N = x.front().cols(); ///< The number of correspondences
        //uint32_t M = x.front().rows(); ///< This should be 2
        uint32_t K = x.size(); ///< The number of views

        Cu::Matrix X(4,N);
        for(uint32_t j,k,n=0; n<N; ++n)
        {
            Cu::Matrix A(2*K, 4);
            for(k=0; k<K; ++k)
            {
                for(j=0; j<4; j++)
                {
                    A(2*k, j)   = P[k](0,j) - P[k](2,j)*x[k](0,n);
                    A(2*k+1, j) = P[k](1,j) - P[k](2,j)*x[k](1,n);
                }
            }
            Eigen::JacobiSVD<Cu::Matrix> svd(A, Eigen::ComputeThinU | Eigen::ComputeFullV);
            X.block(0,n,4,1) = svd.matrixV().block(0,3,4,1);
            X.block(0,n,4,1) /= X(3,n);
        }

        return X;
    }

    Cu::Matrix CrossMatrix( const Cu::Matrix& M )
    {
        Cu::Matrix out(3,3);
        out << 0, -M(2), M(1),
               M(2), 0, -M(0),
               -M(1), M(0), 0;
        return out;
    }

    inline Cu::Matrix pinv( const Cu::Matrix& P)
    {
        return P.transpose()*(P*P.transpose()).inverse();
    }

    Cu::Matrix FFromP(const Cu::Matrix &P1, const Cu::Matrix &P2)
    {
        Eigen::JacobiSVD<Cu::Matrix> svd(P1, Eigen::ComputeThinU | Eigen::ComputeFullV);
        Cu::Matrix C = svd.matrixV().block(0,3,4,1);
        return CrossMatrix(P2*C)*P2*pinv(P1);
    }
}
