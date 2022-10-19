#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include "DataPath.hpp"

QString
DataPath::path(QString filename)
{
  QDir path(QCoreApplication::applicationDirPath());
  path.cdUp();
  path.cd(SHARED_DATA_PATH);
  return (path.absoluteFilePath(filename));
}

QString
DataPath::existingPath(QString filename)
{
  QString candidate(path(filename));
  if(!QFile::exists(candidate)) {
    throw std::runtime_error(
      QString("File <%2> resolved from <%1> does not exist")
        .arg(filename)
        .arg(candidate)
        .toStdString()
    );
  }
  return (candidate);
}
