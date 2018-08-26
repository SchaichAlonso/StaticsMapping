#pragma once

#include <QtCore/QString>

struct DataPath
{
  static QString path(QString filename);
  static QString existingPath(QString filename);
};
