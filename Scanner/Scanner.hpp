#pragma once

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QThreadPool>

#include <Classification/Definitions.hpp>
#include <Classification/Object.hpp>

struct Scanner : QThreadPool
{
  Scanner (QString, int);
  ~Scanner ();
  
  QString found (Classification::ObjectPointer);
  void    scan (QString);

protected:
  void scanImpl (QString);
  
  QMutex            m_objs_lock;
  QMap<QString,int> m_objs;
  
  Classification::DefinitionsPointer m_all, m_found;
  
  QDir m_dst;
  int  m_texture_limit;
};
