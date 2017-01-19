#include "edge_selection_dialog.hpp"




EdgeSelectionDialog::EdgeSelectionDialog(QWidget *parent)
  :QDialog(parent)
  , ds(DataStore::data())
{

  QVBoxLayout *mainlayout = new QVBoxLayout;
  QHBoxLayout *mhl = new QHBoxLayout;

  QVBoxLayout *rvl = new QVBoxLayout;

  QGroupBox *g1 = new QGroupBox;
  g1->setTitle("Surface kind:");

  QVBoxLayout *rbvl = new QVBoxLayout;

  surface_kind_button_group = new QButtonGroup;

  quadric_button = new QRadioButton("Quadric");
  surface_kind_button_group->addButton(quadric_button);
  rbvl->addWidget(quadric_button);
  quadric_button->setChecked(true);

  cylinder_button = new QRadioButton("Cylinder");
  surface_kind_button_group->addButton(cylinder_button);
  rbvl->addWidget(cylinder_button);

  gtm_interp_button = new QRadioButton("GTM Interpolated");
  surface_kind_button_group->addButton(gtm_interp_button);
  rbvl->addWidget(gtm_interp_button);

  gtm_ortho_button = new QRadioButton("GTM Orthogonal");
  surface_kind_button_group->addButton(gtm_ortho_button);
  rbvl->addWidget(gtm_ortho_button);

  g1->setLayout(rbvl);

  rvl->addWidget(g1);

  sampling_density_spinbox = new QSpinBox;
  sampling_density_spinbox->setValue(50);
  QHBoxLayout *h1 = new QHBoxLayout;
  h1->addWidget(new QLabel("sampling density"));
  h1->addWidget(sampling_density_spinbox);
  rvl->addLayout(h1);

  sigma_doublespinbox = new QDoubleSpinBox;
  sigma_doublespinbox->setValue(10);
  sigma_label = new QLabel("sigma");
  QHBoxLayout *h2 = new QHBoxLayout;
  h2->addWidget(sigma_label);
  h2->addWidget(sigma_doublespinbox);
  rvl->addLayout(h2);

  degree_spinbox = new QSpinBox;
  degree_spinbox->setValue(3);
  degree_label = new QLabel("degree");
  QHBoxLayout *h3 = new QHBoxLayout;
  h3->addWidget(degree_label);
  h3->addWidget(degree_spinbox);
  rvl->addLayout(h3);

  k_spinbox = new QSpinBox;
  k_spinbox->setValue(50);
  k_label = new QLabel("k");
  QHBoxLayout *h4 = new QHBoxLayout;
  h4->addWidget(k_label);
  h4->addWidget(k_spinbox);
  rvl->addLayout(h4);

  maxiter_spinbox = new QSpinBox;
  maxiter_spinbox->setValue(20);
  maxiter_label = new QLabel("max iter");
  QHBoxLayout *h5 = new QHBoxLayout;
  h5->addWidget(maxiter_label);
  h5->addWidget(maxiter_spinbox);
  rvl->addLayout(h5);


  lambda_spinbox = new QDoubleSpinBox;
  lambda_spinbox->setValue(5);
  lambda_label = new QLabel("lambda");
  QHBoxLayout *h6 = new QHBoxLayout;
  h6->addWidget(lambda_label);
  h6->addWidget(lambda_spinbox);
  rvl->addLayout(h6);

  mhl->addLayout(rvl);

  main_edge_button_group = new QButtonGroup;
  secondary_edge_button_group = new QButtonGroup;

  {
    QVBoxLayout *sal = new QVBoxLayout;

    foreach(edgeID eid,ds->edges.keys()){
      Edge *e=ds->edges[eid];
      QPushButton *btn = new QPushButton(QString::number(e->id));
      btn->setStyleSheet(COLOR_STYLE
                         .arg(e->edge_color.name())
                         .arg(getIdealTextColor(e->edge_color).name()));
      btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      btn->setCheckable(true);
      btn->setChecked(!e->already_used);
      btn->setProperty("id",eid);
      sal->addWidget(btn);

      main_edge_button_group->addButton(btn);
    }


    QFrame *f=new QFrame;
    f->setLayout(sal);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setMinimumHeight(parent->height()*0.3);
    scrollArea->setMaximumHeight(parent->height()*0.8);
    scrollArea->setWidget(f);

    QVBoxLayout *lvl = new QVBoxLayout;
    lvl->addWidget(scrollArea);

    QGroupBox *g2 = new QGroupBox;
    g2->setTitle("Select main edge:");

    g2->setLayout(lvl);

    mhl->addWidget(g2);
  }

  {
    QVBoxLayout *sal = new QVBoxLayout;

    foreach(edgeID eid,ds->edges.keys()){
      Edge *e=ds->edges[eid];
      QPushButton *btn = new QPushButton(QString::number(e->id));
      btn->setStyleSheet(COLOR_STYLE
                         .arg(e->edge_color.name())
                         .arg(getIdealTextColor(e->edge_color).name()));
      btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      btn->setCheckable(true);
      btn->setChecked(!e->already_used);
      btn->setProperty("id",eid);
      sal->addWidget(btn);

      secondary_edge_button_group->addButton(btn);

    }


    QFrame *f=new QFrame;
    f->setLayout(sal);

    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setMinimumHeight(parent->height()*0.3);
    scrollArea->setMaximumHeight(parent->height()*0.8);
    scrollArea->setWidget(f);

    QVBoxLayout *lvl = new QVBoxLayout;
    lvl->addWidget(scrollArea);

    secondary_edge_groupbox = new QGroupBox;
    secondary_edge_groupbox->setTitle("Select secondary edge (for GTMs)");

    secondary_edge_groupbox->setLayout(lvl);

    mhl->addWidget(secondary_edge_groupbox);
  }

  mainlayout->addLayout(mhl);

  QHBoxLayout *bottom_hlayout = new QHBoxLayout;

  bottom_hlayout->addStretch();

  QPushButton *closeButton = new QPushButton("Cancel");
  connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
  bottom_hlayout->addWidget(closeButton);

  QPushButton *make_edges_btn = new QPushButton("MAKE EDGES");
  make_edges_btn->setDefault(true);
  connect(make_edges_btn, SIGNAL(clicked()), this, SLOT(check_selection()));
  bottom_hlayout->addWidget(make_edges_btn);

  mainlayout->addStretch();
  mainlayout->addLayout(bottom_hlayout);

  setLayout(mainlayout);


 connect(surface_kind_button_group,SIGNAL(buttonClicked(int)),this,SLOT(update_enabled(int)));
update_enabled();
  setWindowTitle("Edge selection dialog");
}


void EdgeSelectionDialog::check_selection()
{
  if((main_edge_button_group->checkedButton() == 0)||
     (secondary_edge_button_group->checkedButton() == 0)){
    QMessageBox msgBox;
    msgBox.setText("You must create some edges before selecting them");
    msgBox.exec();
    return;
  }


  if(cylinder_button->isChecked()){
    params = new CylinderInputParams;
    params->SurfaceType = CYLINDER;
    params->SamplingDensity = sampling_density_spinbox->value();
    static_cast<CylinderInputParams*>(params)->sigma = sigma_doublespinbox->value();

    edgeID main_edge = main_edge_button_group->checkedButton()->property("id").toUInt();
    edges.clear();
    edges.append(ds->edges[main_edge]);

  } else if(quadric_button->isChecked()){
    params = new QuadricInputParams;
    params->SurfaceType = QUADRIC;
    params->SamplingDensity = sampling_density_spinbox->value();
    static_cast<QuadricInputParams*>(params)->sigma = sigma_doublespinbox->value();

    edgeID main_edge = main_edge_button_group->checkedButton()->property("id").toUInt();
    edges.clear();
    edges.append(ds->edges[main_edge]);

  } else if((gtm_interp_button->isChecked())||
            (gtm_ortho_button->isChecked())){

    params = new GTMInputParams;

    if(gtm_ortho_button->isChecked())
      params->SurfaceType = GTM_ORTHOGONAL;
    else
      params->SurfaceType = GTM_INTERPOLATED;
    params->SamplingDensity = sampling_density_spinbox->value();

    static_cast<GTMInputParams*>(params)->Degree = degree_spinbox->value();
    static_cast<GTMInputParams*>(params)->K = k_spinbox->value();
    static_cast<GTMInputParams*>(params)->MaxIter = maxiter_spinbox->value();
    static_cast<GTMInputParams*>(params)->lambda = lambda_spinbox->value();

    edgeID main_edge = main_edge_button_group->checkedButton()->property("id").toUInt();
    edgeID secondary_edge = secondary_edge_button_group->checkedButton()->property("id").toUInt();
    if(main_edge == secondary_edge){
      QMessageBox msgBox;
      msgBox.setText("cannot select the same edge twice");
      msgBox.exec();
      return;
    }
    edges.clear();
    edges.append(ds->edges[main_edge]);
    edges.append(ds->edges[secondary_edge]);



  }



  accept();

}

void EdgeSelectionDialog::update_enabled(int)
{
  bool b = cylinder_button->isChecked() || quadric_button->isChecked();

  sigma_doublespinbox->setEnabled(b);
  sigma_label->setEnabled(b);
  degree_spinbox->setEnabled(!b);
  degree_label->setEnabled(!b);
  k_spinbox->setEnabled(!b);
  k_label->setEnabled(!b);
  maxiter_spinbox->setEnabled(!b);
  maxiter_label->setEnabled(!b);
  lambda_spinbox->setEnabled(!b);
  lambda_label->setEnabled(!b);
  secondary_edge_groupbox->setEnabled(!b);
}


