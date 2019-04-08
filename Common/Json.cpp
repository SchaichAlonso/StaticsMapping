#include <QtCore/QFile>

#include "Json.hpp"

QJsonDocument Json::readJson(QString json)
{
  QJsonParseError error;
  QJsonDocument retval{QJsonDocument::fromJson(json.toUtf8(), &error)};
  
  if (error.error != QJsonParseError::NoError) {
    throw (std::runtime_error(error.errorString().toStdString()));
  }
  
  return (retval);
}


QJsonDocument Json::readJsonFile(QString filename)
{
  QFile file{filename};
  
  bool ok{file.open(QFile::ReadOnly)};
  if (!ok) {
    throw (std::runtime_error(QString("Cannot open <%1>").arg(filename).toStdString()));
  }
  
  return readJson(QString::fromUtf8(file.readAll()));
}
