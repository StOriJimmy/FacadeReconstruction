#ifndef PANZOOMER_HPP_
#define PANZOOMER_HPP_

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

class GLPhoto;
class QVector2D;
class QPoint;

class PanZoomer
{

public:
  PanZoomer(GLPhoto *buddy);
  ~PanZoomer();

  void reset(QRectF zoom_rect);
  QVector2D world_2_screen_space(QVector2D world_coords);
  QVector2D screen_2_world_space(QVector2D screen_coords);
  QVector2D screen_2_world_space(QPoint screen_coords);
  void push_GL_transform();
  void pop_GL_transform();
  void start_panning(QPoint p);
  void pan(QPoint p);
  void end_panning();
  void zoom(float delta);
  bool can_zoom_in();
  bool can_zoom_out();
  qreal get_zoom_factor();
  void zoom_2_original();

protected:
  void update_mouse_position();

private:
  float ZOOM_BASE; // the base to which the zoomlevel is the exponent
  int MIN_ZOOM_LEVEL;
  int MAX_ZOOM_LEVEL;
  GLPhoto *canvas; // the control we are hooked into
  float zoom_factor;   // The magnification factor
  float zoom_level;    // An easy way to change mag factor exponentially
  QVector2D last_mouse_pos;
  QVector2D zoom_pos_world_space;    // WL: These keep track of current zoom position in object and mouse coords
  QVector2D zoom_pos_mouse_space;

  bool panning_enabled;
  bool currently_panning;

  void enable_panning(bool value);
  void set_zoom_level(float value);

  int viewport_width();
  int viewport_height();

};

#endif /* PANZOOMER_HPP_ */
