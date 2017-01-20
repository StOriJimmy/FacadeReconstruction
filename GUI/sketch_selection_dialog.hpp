#ifndef SKETCH_SELECTION_DIALOG_
#define SKETCH_SELECTION_DIALOG_

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
#include "common.hpp"
#include "datastore.hpp"
#include "engine.hpp"

/**
 * @brief The SketchSelectionDialog class.
 * The dialogue box used in advanced mode to determine the type of fit to be computed.
 * @todo Update the dialogue box so that points can be sampled on the edge.
 */
class SketchSelectionDialog : public QDialog
{
  Q_OBJECT
public:
    /**
     * @brief Construct a SketchSelectionDialogue
     * @param parent A pointer to the parent
     */
    SketchSelectionDialog(QWidget *parent=0);

    QSet<sketchID> selection;       ///< The sketch selection
    uint32_t NumberOfNewSamples=0;  ///< The number of new samples (always zero for now)
    double aspectratio = 1.0;       ///< The the aspect ratio

public slots:
  void check_selection();           ///< \brief Check that all of the input is correct

private:
  DataStore *ds;                    ///< A pointer to the datastor object
  QList<QPushButton *> btns;        ///< A Qlist of buttons
  QSpinBox *s1;                     ///< A selection spinbox
  QDoubleSpinBox *s2;               ///< A selection spinbox
};


#endif /* SKETCH_SELECTION_DIALOG_ */
