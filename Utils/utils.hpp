#ifndef UTILS_HPP_
#define UTILS_HPP_

//#include <QtCore>
#include <QtGui>

bool ensureDir(const QString &dirName);
bool cleanDir(const QString &dirName);
bool removeDir(const QString &dirName);
bool copyDir(const QString &srcPath, const QString &dstPath);

QString readfile(QString path);
void writefile(QString path,QString text);
void append2file(QString path,QString text);

QString gen_random_alphanum(const int len);

QImage rotate90(const QImage &src);
QImage rotate180(const QImage &src);
QImage rotate270(const QImage &src);


#endif /* UTILS_HPP_ */
