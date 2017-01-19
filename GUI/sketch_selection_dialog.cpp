#include "sketch_selection_dialog.hpp"




SketchSelectionDialog::SketchSelectionDialog(QWidget *parent)
  :QDialog(parent)
  , ds(DataStore::data())
{
  QVBoxLayout *mainlayout = new QVBoxLayout;

  QHBoxLayout *h1 = new QHBoxLayout;
  QLabel *l1=new QLabel;
  l1->setText("Number Of New Samples");
  h1->addWidget(l1);
  s1 = new QSpinBox;
  s1->setValue(0);
  h1->addWidget(s1);

  h1->addSpacing(12);
  

  QLabel *l2=new QLabel;
  l2->setText("Aspect Ratio");
  h1->addWidget(l2);
  s2 = new QDoubleSpinBox;
  s2->setValue(1.0);
  h1->addWidget(s2);
  h1->addStretch();

  mainlayout->addLayout(h1);

  QVBoxLayout *sal = new QVBoxLayout;

  foreach (rasterID rid, ds->rasters.keys()) {
    if(ds->rasters[rid]->sketches.isEmpty())
      continue;

    QHBoxLayout *hlayout = new QHBoxLayout;


    QLabel *imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    imageLabel->setScaledContents(false);

    imageLabel->setPixmap(ds->rasters[rid]->thumbnail);
    imageLabel->adjustSize();
    imageLabel->resize(imageLabel->pixmap()->size());

    qDebug()<<ds->rasters[rid]->thumbnail.isNull()<<ds->rasters[rid]->thumbnail.size();

    hlayout->addWidget(imageLabel);

    foreach(sketchID sid,ds->rasters[rid]->sketches){
      Sketch *s=ds->sketches[sid];
      QPushButton *btn = new QPushButton(QString::number(s->id));
      btn->setStyleSheet(COLOR_STYLE
                         .arg(s->color.name())
                         .arg(getIdealTextColor(s->color).name()));
      btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
      btn->setCheckable(true);
      btn->setChecked(!s->already_used);
      btn->setProperty("id",sid);
      hlayout->addWidget(btn);

      btns.append(btn);

    }
    hlayout->addStretch();

    sal->addLayout(hlayout);

  }
  QFrame *f=new QFrame;
  f->setLayout(sal);

  QScrollArea *scrollArea = new QScrollArea;
  scrollArea->setMinimumHeight(parent->height()*0.3);
  scrollArea->setMaximumHeight(parent->height()*0.8);
  //  scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff	);
  scrollArea->setWidget(f);
  mainlayout->addWidget(scrollArea);

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

  setWindowTitle("Sketch selection dialog");
}

void SketchSelectionDialog::check_selection()
{
  NumberOfNewSamples = s1->value();
  aspectratio = s2->value();

  if(btns.isEmpty()){
    QMessageBox msgBox;
    msgBox.setText("You must create some sketches before selecting them");
    msgBox.exec();
    return;
  }
  
//  QSet<rasterID> rasters;
  
  selection.clear();
  foreach(QPushButton *btn,btns){
    if(btn->isChecked()){
      sketchID id=btn->property("id").toUInt();
      selection.insert(id);
//      rasters.insert(ds->rasters[ds->sketches[id]->raster]->id);
    }
  }

//  if(selection.size()<2){
//    QMessageBox msgBox;
//    msgBox.setText("You must select more than a single sketch");
//    msgBox.exec();
//    return;
//  }

//  if(rasters.size()<2){
//    QMessageBox msgBox;
//    msgBox.setText("You must select sketches that belong to multiple rasters");
//    msgBox.exec();
//    return;
//  }

  accept();
}


