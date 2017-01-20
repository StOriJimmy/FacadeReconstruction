#ifndef EDGE_SELECTION_DIALOG_
#define EDGE_SELECTION_DIALOG_

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
 * @brief The EdgeSelectionDialog class.
 */
class EdgeSelectionDialog : public QDialog
{
  Q_OBJECT
public:
  EdgeSelectionDialog(QWidget *parent=0);

  QList<Edge *> edges;

  SurfaceInputParams *params;

public slots:
  void check_selection();

private slots:
  void update_enabled(int ignored=0);

private:
  DataStore *ds;

  QButtonGroup *main_edge_button_group;
  QButtonGroup *secondary_edge_button_group;
  QButtonGroup *surface_kind_button_group;
  QRadioButton *cylinder_button;
  QRadioButton *quadric_button;
  QRadioButton *gtm_interp_button;
  QRadioButton *gtm_ortho_button;

  QSpinBox *sampling_density_spinbox;
  QDoubleSpinBox *sigma_doublespinbox;
  QSpinBox *degree_spinbox;
  QSpinBox *k_spinbox;
  QSpinBox *maxiter_spinbox;
  QDoubleSpinBox *lambda_spinbox;
  QGroupBox *secondary_edge_groupbox;

  QLabel *sigma_label;
  QLabel *degree_label;
  QLabel *k_label;
  QLabel *maxiter_label;
  QLabel *lambda_label;
};


#endif /* EDGE_SELECTION_DIALOG_ */
