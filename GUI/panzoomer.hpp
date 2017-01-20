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

/**
 * @brief The PanZoomer class.
 * A class for panning and zooming the GLPhoto widget.
 */
class PanZoomer
{
public:
    /**
     * @brief Construct a PanZoomer
     * @param buddy The GLPhoto to use with the PanZoomer
     */
    PanZoomer(GLPhoto *buddy);
    ~PanZoomer();

    void reset(QRectF zoom_rect);                               ///< \brief Reset the PanZoomer to the given rectangle
    QVector2D world_2_screen_space(QVector2D world_coords);     ///< \brief Convert between world and screen coodinates
    QVector2D screen_2_world_space(QVector2D screen_coords);    ///< \brief Convert between screen and world coordinates
    QVector2D screen_2_world_space(QPoint screen_coords);       ///< \brief Convert between screen and world coordinates
    void push_GL_transform();                                   ///< \brief Push transformations to GL
    void pop_GL_transform();                                    ///< \brief Pop the GL stack
    void start_panning(QPoint p);                               ///< \brief Start panning from a mouse position 'p'
    void pan(QPoint p);                                         ///< \brief Pan using the mouse position 'p'
    void end_panning();                                         ///< \brief End the panning
    void zoom(float delta);                                     ///< \brief Zoom, given a change in zoom level 'delta'
    bool can_zoom_in();                                         ///< \brief Return true if it is possible to zoom in
    bool can_zoom_out();                                        ///< \brief Return true if it is possible to zoom out
    qreal get_zoom_factor();                                    ///< \brief Return the zoom factor
    void zoom_2_original();                                     ///< \brief Return the zool level to its original

protected:
    void update_mouse_position();                               ///< \brief Update the mouse position

private:
    float ZOOM_BASE;                     ///< The base to which the zoomlevel is the exponent
    int MIN_ZOOM_LEVEL;                  ///< The minimum zoom level
    int MAX_ZOOM_LEVEL;                  ///< The maximum zoom level
    GLPhoto *canvas;                     ///< A pointer to the control we are hooked into
    float zoom_factor;                   ///< The magnification factor
    float zoom_level;                    ///< An easy way to change mag factor exponentially
    QVector2D last_mouse_pos;            ///< The last mouse position
    QVector2D zoom_pos_world_space;      ///< These keep track of current zoom position in object and mouse coords
    QVector2D zoom_pos_mouse_space;      ///< These keep track of current zoom position in object and mouse coords

    bool panning_enabled;                ///< True if panning was enabled
    bool currently_panning;              ///< True if we are currently panning

    void enable_panning(bool value);     ///< Set the 'panning enabled' flag to 'value'
    void set_zoom_level(float value);    ///< Set the zoom level

    int viewport_width();                ///< Return the viewport width
    int viewport_height();               ///< Return the viewport height
};

#endif /* PANZOOMER_HPP_ */
