
#include "zbuffer.h"
#include <octave/oct.h>

DEFUN_DLD(zbufferoct, args, , "esl = zbufferoct(vertices, faces, P,height,width)")
{
    int nargin = args.length ();
    if( nargin > 6 )
    {
      print_usage();
    }
    int verbose=0;
    if( nargin == 6 )
    {
      verbose = args(5).scalar_value();
    }

    if(verbose==1)
    {
        std::cout << "Running ZBuffer" << std::endl;
    }

    const Matrix vertices = args(0).array_value ();
    const Matrix faces = args(1).array_value();
    const Matrix P = args(2).array_value ();
    const int height = args(3).scalar_value();
    const int width = args(4).scalar_value();
    if(verbose == 1)
    {
        std::cout << "Vertices: " << vertices.cols() <<
                     " Faces:" << faces.cols() <<
                     " Pixels:" << height*width << std::endl;
    }
    ZBuffer zb(vertices.data(), faces.data(), vertices.cols(), faces.cols(), height, width );
    if(verbose == 1)
    {
        std::cout << "Computing Buffer" << std::endl;
    }
    zb.ComputeDepthBuffer(P);
    octave_value_list ovl;
    if(verbose == 1)
    {
        std::cout << "Return Buffer" << std::endl;
    }
    Matrix depthbuf = zb.ReturnDepthBuffer();
    if(verbose == 1)
    {
        std::cout << "Return Depth" << std::endl;
    }
    Matrix vertexdepth = zb.ReturnVertexDepth(P);
    ovl(0)=octave_value(depthbuf);
    ovl(1)=octave_value(vertexdepth);

    return ovl;
}
