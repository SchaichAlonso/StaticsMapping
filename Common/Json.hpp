#include <QtCore/QJsonDocument>

struct Json
{
  static QJsonDocument readJson(QString json);
  static QJsonDocument readJsonFile(QString filename);
};
