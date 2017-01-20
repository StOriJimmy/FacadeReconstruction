#include "PolyGTM.h"

namespace Core
{
    void distance2(const double* data1, const double* data2, double* D,  const uint32_t N1, const uint32_t N2, const uint32_t K); ///< The distance 2 function
    void mahal(const uint32_t M, const double BetaInv, const double* D, double* R, const uint32_t N, const uint32_t K, const double *sum);
    void betainv(double* betainv, const double* D, const double* R, const uint32_t K, const uint32_t N , const double *sum);
    void G(double* G, const double* R, const uint32_t K, const uint32_t N, const double *sum);
    void NormaliseLogarithmicMatrix(double* R, double *sum, const uint32_t K, const uint32_t N);


    Output CreateStartPointUsingPCA( const Input& input)
    {
        Cu::Matrix data = ConvertToEigen(input.data, input.M, input.N);
        Cu::Matrix mu = data.rowwise().sum() / input.N;
        Cu::Matrix dminmu = data - mu.replicate(1,input.N);
        Cu::Matrix sigma = (dminmu*dminmu.transpose())/ (input.N-1) ;

        auto US = EigenValues( sigma );
        uint32_t maxrow;
        Cu::Vector(US.second).maxCoeff(&maxrow);

        Output out;
        out.BetaInv = Cu::Vector(US.second).minCoeff();
        out.Phi = Cu::Matrix(input.degree+1,input.K);
        uint32_t i,j;
        for(i=0;i<input.K;++i)
        {
            for(j=0;j<=input.degree;++j)
            {
                out.Phi(j,i) = std::pow(static_cast<double>(i) / (input.K-1), static_cast<double>(j));
            }
        }
        out.W = Cu::Matrix(input.M, input.degree+1);
        out.W.setZero();

        Cu::Matrix w0 = US.first.block(0,maxrow,input.M,1).transpose()*dminmu;

        std::sort(w0.data(), w0.data()+input.N);

        Cu::Matrix w1 = Cu::Matrix(input.M, input.N);
        w1.setZero();
        w1.block(maxrow,0,1,input.N) = w0;
        w1 = US.first*w1 + mu.replicate(1, input.N);

        out.W.block(0,0,input.M,1) = w1.block(0,0,input.M,1);
        out.W.block(0,1,input.M,1) = w1.block(0,input.N-1,input.M,1) - w1.block(0,0,input.M,1);

        return out;
    }

    Output Polynomial( Input& input )
    {
        Output out = CreateStartPointUsingPCA(input);
        const double tol = 1e-6;

        uint32_t iter=0;
        double oldVal=std::numeric_limits<double>::max();
        (void) oldVal; // Currently unused
        double err=std::numeric_limits<double>::max();

        Cu::Matrix D(input.K, input.N), R(input.K, input.N), Gm(input.K, input.K), eye(input.degree+1, input.degree+1);
        D.setZero(); R.setZero(); Gm.setZero(); eye.setIdentity();
        Cu::Matrix WPhi = out.W * out.Phi;
        Cu::Matrix data = ConvertToEigen(input.data, input.M, input.N);

        double* sum = new double[input.N];

        for(uint32_t i=0;i<input.N;++i)
            sum[i]=1;


        while(iter < input.MaxIter && err > tol)
        {
            if(iter == 0)
            {
                WPhi = out.W * out.Phi;
                distance2(WPhi.data(), input.data, D.data(), input.K, input.N, input.M);
            }
            mahal(input.M, out.BetaInv, D.data(), R.data(), input.N, input.K, sum );

            NormaliseLogarithmicMatrix(R.data(), sum, input.K, input.N);

            G(Gm.data(), R.data(), input.K, input.N, sum);
            out.W = ((out.Phi*Gm*out.Phi.transpose() + (input.lambda*out.BetaInv)*eye ).inverse()*out.Phi * R *data.transpose()).transpose();

            WPhi = out.W * out.Phi;
            distance2(WPhi.data(), input.data, D.data(), input.K, input.N, input.M);

            betainv( &out.BetaInv, D.data(), R.data(), input.K, input.N, sum );
            ++iter;
        }
        delete [] sum;

        return out;
    }

    void distance2(const double* data1, const double* data2, double* D, const uint32_t N1, const uint32_t N2, const uint32_t K)
    {
        uint32_t k,j,i;
        for(i=0;i<N1; ++i)
        {
            for(j=0;j<N2;++j)
            {
                D[N1*j + i]=0.0;
                for(k=0;k<K;++k)
                {
                    D[N1*j + i]+=(data1[K*i + k] - data2[K*j+k])*(data1[K*i + k] - data2[K*j+k]);
                }
            }
        }
    }

    void mahal(const uint32_t M, const double BetaInv, const double* D, double* R, const uint32_t N, const uint32_t K, const double* sum)
    {
        uint32_t i,k;
        double c = (-static_cast<double>(M)/2)*std::log(2*M_PI*BetaInv);
        for(k=0;k<K;++k)
        for(i=0;i<N;++i)
            if(sum[i]>0)
                R[K*i+k] = c - (D[K*i+k]/BetaInv);
    }

    void betainv( double* betainv, const double* D, const double* R, const uint32_t K, const uint32_t N, const double* sum )
    {
        uint32_t i,k,count=0;
        *betainv=0.0;
        for(k=0;k<K;++k)
        for(i=0;i<N;++i)
            if(sum[i]>0)
            {
                *betainv += R[K*i+k]*D[K*i+k];
                ++count;
            }
        *betainv /= count;
    }

    void G(double* G, const double* R, const uint32_t K, const uint32_t N, const double* sum)
    {
        uint32_t j,i;
        for(i=0;i<K;++i)
        for(j=0;j<K;++j)
            G[K*j + i]=0.0;

        for(i=0;i<K;++i)
        for(j=0; j<N; ++j)
            if(sum[i]>0)
                G[K*i + i] += R[K*j +i];
    }

    void NormaliseLogarithmicMatrix(double* R, double *sum, const uint32_t K, const uint32_t N)
    {
        uint32_t i,j;
        double *maxes = new double[N];
        double apmin = std::numeric_limits<double>::min();
        for(i=0;i<N;++i)
        {
            maxes[i] = apmin;
            for(j=0;j<K;++j)
            {
                maxes[i]= R[K*i + j] > maxes[i] ? R[K*i + j] : maxes[i];
            }
            sum[i]=0.0;
        }

        for(i=0;i<N;++i)
        for(j=0;j<K;++j)
        {
            R[K*i + j] = std::exp( R[K*i + j] - maxes[i] );
            sum[i] += R[K*i + j];
        }

        for(i=0;i<N;++i)
        for(j=0;j<K;++j)
        {
            if(sum[i]>0)
                R[K*i + j] /= sum[i] ;
        }

        delete [] maxes;
    }
}
