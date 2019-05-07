#include <stdexcept>

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QtGlobal>

#include "DataPath.hpp"

QString
DataPath::path(QString filename)
{
  if (qEnvironmentVariableIsSet("SHARED_DATA_PATH")) {
    return QDir(qEnvironmentVariable("SHARED_DATA_PATH")).filePath(filename);
  }
  QDir path(QCoreApplication::applicationDirPath());
  path.cdUp();
  path.cd(SHARED_DATA_PATH);
  return (path.filePath(filename));
}

QString
DataPath::existingPath(QString filename)
{
  QString candidate(path(filename));
  if(!QFile::exists(candidate)) {
    throw std::runtime_error(
      QString("File <%1> does not exist")
        .arg(filename)
        .toStdString()
    );
  }
  return (candidate);
}
