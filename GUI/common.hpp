#ifndef COMMON_HPP_
#define COMMON_HPP_

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

#include <QtOpenGL>

#include <Logger.h>

// portability issues
#if defined(_WIN32) || defined(_WIN64)
#define snprintf _snprintf
#define vsnprintf _vsnprintf
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define uint unsigned int
#define M_E 2.71828182845904523536028747135266250   /* e */
#define round(x) floor(x+0.5f)
#define fmax(x,y) qMax(x,y)
#endif

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE 0x809D
#endif

// extern common vars (defined and initialized in main.cpp)
extern QString APP_DISPLAY_NAME;
extern QString APP_TECH_NAME;
extern QString BASE_PATH;
extern QString COLOR_STYLE;
//extern QString LOG_PATH;

//global application time (definition and start is in main.cpp)
extern QTime *gTime;

// portable sleeper
class Sleeper: public QThread
{
public:
  static void usleep(int us)
  {
    QThread::usleep(us);
  }
  static void msleep(unsigned long us)
  {
    QThread::msleep(us);
  }

};

/**
 * \brief return time since start in msecs (wraps after 24 hours)
 */
inline QString now()
{
  return QString::number(gTime->elapsed());
}

/**
 * @brief Return a good text colour
 * @param rBackgroundColor
 * @return
 */
inline QColor getIdealTextColor(const QColor& rBackgroundColor)
{
    const int THRESHOLD = 105;
    int BackgroundDelta = (rBackgroundColor.red() * 0.299) + (rBackgroundColor.green() * 0.587) + (rBackgroundColor.blue() * 0.114);
    return QColor((255- BackgroundDelta < THRESHOLD) ? Qt::black : Qt::white);
}

//-////////////////// VCG stuff //////////////////-//

// vcg imports
#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/create/platonic.h>

// wrapper imports
#include <wrap/io_trimesh/import.h>
#include <wrap/io_trimesh/export.h>
#include <wrap/gui/trackball.h>
#include <wrap/qt/trackball.h>
#include <wrap/gl/trimesh.h>
//#include <vcg/math/shot.h>

// declaring edge and face type
class CFace;
class CVertex;

/**
 * @brief The used types for vcg
 */
struct MyUsedTypes
    : public vcg::UsedTypes<
    vcg::Use<CVertex>::AsVertexType,
    vcg::Use<CFace>::AsFaceType
    >
{
};

// compositing wanted proprieties

/**
 * @brief The CVertex class.
 * A specification for the vertex attributes in the mesh
 */
class CVertex
    : public vcg::Vertex<
    MyUsedTypes,
    vcg::vertex::Coord3f,
    vcg::vertex::Normal3f,
    vcg::vertex::BitFlags,
    vcg::vertex::TexCoord2f
    >
{
};

/**
 * @brief The CFace class.
 * A specification for the face attributes in the mesh
 */
class CFace
    : public vcg::Face<
    MyUsedTypes,
    vcg::face::VertexRef,
    vcg::face::Normal3f,
    vcg::face::BitFlags
    >
{
};

/**
 * @brief The CMesh class.
 * The definition of the mesh in terms of the declared vcg types.
 */
class CMesh
    : public vcg::tri::TriMesh<
    std::vector<CVertex>,
    std::vector<CFace>
    >
{
};

#endif /* COMMON_HPP_ */
