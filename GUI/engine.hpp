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

/**
 * @brief The Engine class.
 * Engine implements the policies of data manipulation
 * it follows a singleton design pattern
 */
class Engine: public QObject
{
  Q_OBJECT
public:
    /**
     * @brief Return a pointer to the Engine instance
     * @return
     */
    static Engine* engine();

    float tool_size;                        ///< The size of the sketching tool
    int tool_size_updated;                  ///< The time that the tool was updated
    bool sketching_permitted;               ///< True if sketching is permitted

    QColor sketch_color;                    ///< The colour of the sketch

    QList< meshID > surface_undo_stack;     ///< A QList of meshIds for the undo stack

    QString selected_tool;                  ///< The currently selected tool

    /**
     * @brief Add user inpute
     * @param input The user input to add
     */
    void add_sketch_input(const UserInput &input);

    /**
     * @brief Create an edge for a QList of Sketches
     * @param sketches The QList of Sketches
     * @param color_name The colour of the sketch
     * @param NumberOfNewSamples The number of new samples to draw
     * @param aspectratio The device pixel ratio
     * @return True on success
     */
    bool make_edge(QList<Sketch *> sketches, QString color_name, uint32_t NumberOfNewSamples=0, double aspectratio = 1.0);

    /**
     * @brief Make a surface from a QList of edges
     * @param edges The QList of edges
     * @param color_name The colour of the surface
     * @param params The surface input parameters
     * @return True on success
     */
    bool make_surface(QList<Edge *> edges, QString color_name, SurfaceInputParams *params);

public slots:
    /**
     * @brief Set the tool size
     * @param The size
     */
    void set_tool_size(int toolbar_value);

    /**
     * @brief Delete all of the sketches in the current raster
     */
    void delete_sketches_in_current_raster();

private:
    DataStore *ds;                          ///< A pointer to the datastore object

    // singleton stuff
    Engine();                               ///< \brief A private constructor for the singleton pattern
    virtual ~Engine();                      ///< \brief A private destructor for the singleton pattern
    Engine(const Engine &);                 ///< \brief A private copy constructor for the singleton pattern
    Engine& operator=(const Engine &);      ///< \brief A private assignment operator for the singleton pattern

    static Engine *instance;                ///< \brief A pointer to the static Engine instance

public:
    bool sketching;                         ///< True if a Sketch is currently being drawn

    void delete_all_sketches();             ///< \brief Delete all the sketches
    void delete_all_edges();                ///< \brief Delete all the edges
    void delete_all_surfaces();             ///< \brief Delete all the surfaces
    int count_surfaces();                   ///< \brief Return the number of surfaces

    /**
     * @brief Save everything in the datastore to disk
     * @param filename The path to save to
     * @param with_poisson  True if Poisson surface computation is required
     * @param with_color True if surface colouring is required
     * @return True on success
     */
    bool save_total(QString filename,bool with_poisson ,bool with_color);
};

#endif /* ENGINE_HPP_ */
