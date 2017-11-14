#pragma once

#include <QSettings>
#include <QString>


struct Settings : QSettings {

  virtual ~Settings ();

  static QString stringValue (QString);
  static int intValue (QString);
  static float floatValue (QString);
  
  static Settings *instance ();
  
protected:
  Settings (QObject * = nullptr);

  static Settings *m_instance;
};
