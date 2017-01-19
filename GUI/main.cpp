#include <QApplication>
#include <QDesktopWidget>

#include "common.hpp"
#include "mainwindow.hpp"
#include "utils.hpp"

#include <ConsoleAppender.h>
//#include <FileAppender.h>

// global vars (externally defined in common.hpp)
QString APP_DISPLAY_NAME;
QString APP_TECH_NAME;
QString BASE_PATH;
QString COLOR_STYLE;
//QString LOG_PATH;

QTime *gTime = NULL;

int main(int argc, char *argv[])
{
  Q_INIT_RESOURCE(resources);

  qDebug()<<QColor::colorNames().size();

  uint ms = QDateTime::currentMSecsSinceEpoch();
  srand(ms);
  qsrand(ms);

  //  QGL::setPreferredPaintEngine(QPaintEngine::OpenGL2);
  //  QGL::setPreferredPaintEngine(QPaintEngine::OpenGL);

  QApplication app(argc, argv);

  if(!QGLFormat::hasOpenGL() ||
     !QGLFramebufferObject::hasOpenGLFramebufferObjects()) {
    QMessageBox::information(0, "OpenGL framebuffer objects",
                             "This system does not support OpenGL/framebuffer objects.");
    return -1;
  }

  APP_DISPLAY_NAME = QString("FacadeReconstruction");
  APP_TECH_NAME = QString("FacadeGUI");
  BASE_PATH = QDir(QApplication::applicationDirPath()).absolutePath() + "/";
  COLOR_STYLE = QString("QPushButton { background-color : %1; color : %2; }");

  gTime = new QTime();
  gTime->start(); //start the global timer

  QCoreApplication::setOrganizationName("University Of Bath");
  QCoreApplication::setOrganizationDomain("bath.ac.uk");
  QCoreApplication::setApplicationName(APP_TECH_NAME);

  QGLFormat glFmt;
  glFmt.setSwapInterval(1); // 1= vsync on
  glFmt.setAlpha(true);
  glFmt.setRgba(true);
  glFmt.setDoubleBuffer(true); // default
  glFmt.setOverlay(false);
  glFmt.setSampleBuffers(false);
  QGLFormat::setDefaultFormat(glFmt);

  QDir::setCurrent(BASE_PATH + "../data");
  //  LOG_PATH = QString(QDir(BASE_PATH+"../logs").absolutePath()+"/");
  //  if(!ensureDir(LOG_PATH)){
  //    qDebug()<< "cannot use log dir"<<LOG_PATH<<", please make it available.";
  //    return -1;
  //  }

  QString logFormat;
  //  logFormat = QString("[%-7l] <%C> %m\n");
  logFormat = QString("[%{time}{HH:mm:ss}]%{function} %{message}\n");
  //  logFormat = QString("%m\n");
  ConsoleAppender *consoleAppender = new ConsoleAppender();
  consoleAppender->setFormat(logFormat);
  logger->registerAppender(consoleAppender);
  LOG_INFO() << "Starting" << APP_DISPLAY_NAME;

  MainWindow *window = new MainWindow;
  window->show();

  if ( QApplication::desktop()->screenCount() > 1 ) {
    QRect rect =  QApplication::desktop()->screenGeometry(1);
    rect.setHeight(1060);
    rect.setTop(rect.top()+20);
    window->setMaximumSize(rect.size());
    window->setGeometry(rect);
//    window->move(rect.topLeft());

  }

  // todo: proper auto-open call
  Sleeper::usleep(1000);
  window->openAction->trigger();

  int ret_value = app.exec();

  delete window;
  delete gTime;
  return ret_value;
}
