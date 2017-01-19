#ifndef ZBUFFER_H
#define ZBUFFER_H

#include <vector>
#include "octave/oct.h"
//#include "MMatrix.h"

/**
 * @brief The ZBuffer class
 * This class represents a z buffer for a collection of vertices and faces. The implementation
 * is supposed to be fast, but does not use any external libraries except octave for its
 * matrices. There is no use of the GPU or of multiple processors. A faster implementation
 * exists in OpenGL which uses the GPU and can do this computation in real time, compared to
 * about half a second for an image in the order of 10M pixels, and a mesh with 1M faces.
 */
class ZBuffer
{
public:
    /**
     * @brief ZBuffer
     * Contructor for the ZBuffer class
     * @param vertices A pointer to the vertex list (column major with 3 rows)
     * @param faces A pointer to the face list (column major 3 rows)
     * @param Nv The number of vertices
     * @param Nf The number fo faces
     * @param height The height of the image
     * @param width the width of the image
     */
    ZBuffer(const double* vertices, const double* faces, const uint32_t Nv, const uint32_t Nf, const uint32_t height, const uint32_t width );
    ~ZBuffer(); ///< Empty destructor

    void ComputeDepthBuffer(const Matrix& P);         ///< Compute the depth buffer for camera @param P
    const Matrix& ReturnDepthBuffer() const;          ///< return the current depth buffer
    const Matrix& ReturnVertexDepth(const Matrix &P); ///< compute and return the current vertex depths for camera @param P
private:
    const double* m_vertices; ///< A pointer to the vertex array
    const double* m_faces;    ///< A pointer to the face index array
    uint32_t m_Nv;            ///< the number of vertices
    uint32_t m_Nf;            ///< The number of faces
    uint32_t m_height;        ///< The height of the buffer
    uint32_t m_width;         ///< The width of the buffer

    Matrix m_db;              ///< The depth buffer
    Matrix m_vertexDepth;     ///< The depth of each vertex
};

/**
 * @brief InsideTriangle
 * Check whether a point (x) lies within a triangle (a,b,c)
 * This is not used in favour of more efficient inline code
 */
inline bool InsideTriangle( const Matrix& a, const Matrix&b, const Matrix& c, const Matrix& x );
inline void SetVertices(const double* vertices, const double* faces, Matrix& a, Matrix& b, Matrix& c, const int& i);

/**
 * @brief Project
 * Project a 3d homogenous (X )point on to a 2D point (x) using a projection
 * matrix (P).
 * This is a fast implementation which does no checking.
 */
inline void Project(const Matrix& P, const Matrix& X, Matrix &x);

#endif // ZBUFFER_H
