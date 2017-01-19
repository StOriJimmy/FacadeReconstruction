#include "zbuffer.h"

ZBuffer::ZBuffer(const double* vertices, const double* faces, const uint32_t Nv, const uint32_t Nf, const uint32_t height, const uint32_t width )
    : m_vertices(vertices), m_faces(faces), m_Nv(Nv), m_Nf(Nf), m_height(height), m_width(width), m_db(height,width), m_vertexDepth(1,m_Nv)
{
    m_db.fill(-1.0);
    m_vertexDepth.fill(-1.0);
}

ZBuffer::~ZBuffer()
{

}

Matrix ComputeLocation(const Matrix& P)
{
    Matrix R = -P.extract(0,0,2,2).inverse();
    Matrix t =  P.extract(0,3,2,3);
    return  R * t;
}

void SetVertices(const double* vertices, const double* faces, Matrix& a, Matrix& b, Matrix& c, const int& i)
{
    for(uint32_t k = 0; k < 3; k++)
    {
        a(k,0) = vertices[3*static_cast<uint32_t>(faces[3*i])+k];
        b(k,0) = vertices[3*static_cast<uint32_t>(faces[3*i+1])+k];
        c(k,0) = vertices[3*static_cast<uint32_t>(faces[3*i+2])+k];
    }
}

// This could be optimised using element operations
// there are a large number of copies here
bool InsideTriangle( const Matrix& a, const Matrix&b, const Matrix& c, const Matrix& x )
{
    return std::abs( std::acos(product(a-x,b-x).sum().elem(0)) +
                     std::acos(product(b-x,c-x).sum().elem(0)) +
                     std::acos(product(c-x,a-x).sum().elem(0)) - 2*M_PI) < 1e-1;
}

inline void Project(const Matrix& P, const Matrix& X, Matrix &x)
{
    x(0,0) = (P.data()[0]*X.data()[0] +
              P.data()[3]*X.data()[1] +
              P.data()[6]*X.data()[2] +
              P.data()[9]*X.data()[3]) /
            (P.data()[2]*X.data()[0] +
              P.data()[5]*X.data()[1] +
              P.data()[8]*X.data()[2] +
              P.data()[11]*X.data()[3]);
    x(1,0) = (P.data()[1]*X.data()[0] +
              P.data()[4]*X.data()[1] +
              P.data()[7]*X.data()[2] +
              P.data()[10]*X.data()[3]) /
            (P.data()[2]*X.data()[0] +
              P.data()[5]*X.data()[1] +
              P.data()[8]*X.data()[2] +
              P.data()[11]*X.data()[3]);
    x(2,0)=1;
}

void ZBuffer::ComputeDepthBuffer(const Matrix &P)
{
    Matrix C = ComputeLocation(P);
    Matrix a(4,1), b(4,1), c(4,1);
    a(3,0)=1; b(3,0)=1; c(3,0)=1;
    Matrix pa(3,1), pb(3,1), pc(3,1);
    double maxx, maxy, minx, miny, distance, angle1,angle2,angle3;
    Matrix center(2,1);
#pragma omp parallel for
    for(uint32_t i=0; i<m_Nf; i++)
    {
        SetVertices(m_vertices,m_faces,a,b,c,i);
        Project(P, a, pa);
        Project(P, b, pb);
        Project(P, c, pc);

        maxx = std::max( pa(0,0), std::max(pb(0,0), pc(0,0)) );
        maxy = std::max( pa(1,0), std::max(pb(1,0), pc(1,0)) );
        minx = std::min( pa(0,0), std::min(pb(0,0), pc(0,0)) );
        miny = std::min( pa(1,0), std::min(pb(1,0), pc(1,0)) );

        if(minx>0 && miny >0 && maxx<=m_width && maxy<=m_height)
        {
            //This distance calculation could be interpolated
            distance = 0.0;
            for (int k=0;k<3;k++)
            {
                distance += (C.data()[k] - (a.data()[k] + b.data()[k] + c.data()[k])/3.0) *
                            (C.data()[k] - (a.data()[k] + b.data()[k] + c.data()[k])/3.0);
            }
            for(int j=static_cast<int>(floorf(minx)); j < ceilf(maxx); j++)
            {
                for(int k=static_cast<int>(floorf(miny)); k< ceilf(maxy); k++)
                {
                    if( j>=0 && k>=0 && j<m_width && k<m_height &&
                        (m_db.data()[m_height*j+k]<0 || distance < m_db.data()[m_height*j+k]*m_db.data()[m_height*j+k]))
                    {
                        center(0,0)=static_cast<double>(j);
                        center(1,0)=static_cast<double>(k);

                        angle1 = 0.0;
                        angle2 = 0.0;
                        angle3 = 0.0;
                        for(int t=0; t<2;t++)
                        {
                            angle1+=(pa.data()[t]-center.data()[t])*(pb.data()[t]-center.data()[t]);
                            angle2+=(pb.data()[t]-center.data()[t])*(pc.data()[t]-center.data()[t]);
                            angle3+=(pc.data()[t]-center.data()[t])*(pa.data()[t]-center.data()[t]);
                        }

                        if(std::abs( std::acos(angle1) + std::acos(angle2) + std::acos(angle3) - 2*M_PI ) < 1e-1 )
                        {
                            m_db.fortran_vec()[m_height*j+k] = std::sqrt(distance);
                        }
                    }
                }
            }
        }
    }
}

const Matrix& ZBuffer::ReturnVertexDepth(const Matrix& P)
{
    Matrix C = ComputeLocation(P);
    for(int i=0;i<m_Nv;i++)
    {
        m_vertexDepth(0,i) = std::sqrt(
                    (m_vertices[3*i] - C.data()[0])*(m_vertices[3*i] - C.data()[0]) +
                    (m_vertices[3*i+1] - C.data()[1])*(m_vertices[3*i+1] - C.data()[1]) +
                    (m_vertices[3*i+2] - C.data()[2])*(m_vertices[3*i+2] - C.data()[2]));;
    }
    return m_vertexDepth;
}

const Matrix&  ZBuffer::ReturnDepthBuffer() const
{
    return m_db;
}
