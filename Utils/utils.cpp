#include "utils.hpp"

bool ensureDir(const QString &dirName)
{
  QDir dir(dirName);
  if(!dir.exists()){
    if(!(QDir(QDir::cleanPath(dirName+"/..")).mkdir(dir.dirName()))){
      qDebug()<<"ensureDir cannot make"<<dirName;
      return false;
    }
  }
  return dir.isReadable();
}

bool cleanDir(const QString &dirName)
{
  QDir dir(dirName);
  if(dir.exists()){
    if(!removeDir(dir.absolutePath())){
      qDebug()<<"cleanDir cannot remove previous"<<dirName;
      return false;
    }
  }
  if(!(QDir(QDir::cleanPath(dirName+"/..")).mkdir(dir.dirName()))){
    qDebug()<<"cleanDir cannot make"<<dirName;
    return false;
  }
  return true;
}


bool removeDir(const QString &dirName)
{
  bool result = true;
  QDir dir(dirName);
  if (dir.exists(dirName)) {
    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
      if (info.isDir()) {
        result = removeDir(info.absoluteFilePath());
      }
      else {
        result = QFile::remove(info.absoluteFilePath());
      }
      if (!result) {
        return result;
      }
    }
    result = dir.rmdir(dirName);
  }
  return result;
}

bool copyDir(const QString &srcPath, const QString &dstPath)
{
  removeDir(dstPath);
  QDir parentDstDir(QFileInfo(dstPath).path());
  if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
    return false;

  QDir srcDir(srcPath);
  foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
    QString srcItemPath = srcPath + "/" + info.fileName();
    QString dstItemPath = dstPath + "/" + info.fileName();
    if (info.isDir()) {
      if (!copyDir(srcItemPath, dstItemPath)) {
        return false;
      }
    } else if (info.isFile()) {
      if (!QFile::copy(srcItemPath, dstItemPath)) {
        return false;
      }
    } else {
      qDebug() << "Unhandled item" << info.filePath() << "in copyDir";
    }
  }
  return true;
}


QString readfile(QString path)
{
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    qDebug()<<"cannot open file"<<path;
    exit(1);
  }
  return QTextStream(&file).readAll();
}

void writefile(QString path,QString text)
{
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
    qDebug()<<"cannot open file"<<path;
    exit(1);
  }
  QTextStream(&file)<<text;
  file.close();

}

void append2file(QString path,QString text)
{
  QFile file(path);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
    qDebug()<<"cannot open file"<<path;
    exit(1);
  }
  QTextStream(&file)<<text;
  file.close();
}

QString gen_random_alphanum(const int len)
{
  char s[len];
  static const char alphanum[] =
      "0123456789"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz";

  for (int i = 0; i < len; ++i) {
    s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  s[len] = 0;
  return QString(s);
}

QImage rotate90(const QImage &src) {
  QImage dst(src.height(), src.width(), src.format());
  for (int y=0;y<src.height();++y) {
    const uint *srcLine = reinterpret_cast< const uint * >(src.scanLine(y));
    for (int x=0;x<src.width();++x) {
      dst.setPixel(src.height()-y-1, x, srcLine[x]);
    }
  }
  return dst;
}

QImage rotate180(const QImage &src) {
  QImage dst(src.width(), src.height(), src.format());
  for (int y=0;y<src.height();++y) {
    const uint *srcLine = reinterpret_cast< const uint * >(src.scanLine(y));
    for (int x=0;x<src.width();++x) {
      dst.setPixel(src.width()-x-1, src.height()-y-1, srcLine[x]);
    }
  }
  return dst;
}

QImage rotate270(const QImage &src) {
  QImage dst(src.height(), src.width(), src.format());
  for (int y=0;y<src.height();++y) {
    const uint *srcLine = reinterpret_cast< const uint * >(src.scanLine(y));
    for (int x=0;x<src.width();++x) {
      dst.setPixel(y, src.width()-x-1, srcLine[x]);
    }
  }
  return dst;
}


