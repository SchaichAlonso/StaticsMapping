#include <QSharedPointer>

#include "Settings.hpp"


Settings *Settings::m_instance = Q_NULLPTR;



Settings::Settings (QObject *parent)
  : QSettings ("settings.ini", QSettings::IniFormat, parent) {
}



Settings::~Settings () {
}



Settings *
Settings::instance () {

  if (m_instance == Q_NULLPTR) {
    static QSharedPointer<Settings> delete_on_cxa_finish (new Settings());
    m_instance = delete_on_cxa_finish.data ();
  }
  
  return (m_instance);
}



float
Settings::floatValue (QString key) {
  return (instance()->value(key).toFloat());
}



int
Settings::intValue (QString key) {
  return (instance()->value(key).toInt());
}



QString
Settings::stringValue (QString key) {
  return (instance()->value(key).toString());
}
