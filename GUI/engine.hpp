#ifndef ENGINE_HPP_
#define ENGINE_HPP_

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

#include <QtGui>
#include <QtXml>
#include "common.hpp"
#include "datastore.hpp"
#include "user_input.hpp"

// engine implements the policies of data manipulation
// it follows a singleton design pattern
class Engine: public QObject
{
  Q_OBJECT
public:
  static Engine* engine();

  float tool_size;
  int tool_size_updated;
  bool sketching_permitted;

  QColor sketch_color;

  QList< meshID > surface_undo_stack;

  QString selected_tool;
  void add_sketch_input(const UserInput &input);

  bool make_edge(QList<Sketch *> sketches, QString color_name, uint32_t NumberOfNewSamples=0, double aspectratio = 1.0);
  bool make_surface(QList<Edge *> edges, QString color_name, SurfaceInputParams *params);
public slots:
  void set_tool_size(int toolbar_value);
  void delete_sketches_in_current_raster();



private:

  DataStore *ds;

  // singleton stuff
  Engine();
  virtual ~Engine();
  Engine(const Engine &);
  Engine& operator=(const Engine &);
  static Engine *instance;

public:
  bool sketching;
  void delete_all_sketches();
  void delete_all_edges();
  void delete_all_surfaces();
  int count_surfaces();
  bool save_total(QString filename,bool with_poisson ,bool with_color);
};

#endif /* ENGINE_HPP_ */
