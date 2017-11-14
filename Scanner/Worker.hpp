#pragma once

#include <QtCore/QRunnable>
#include <QtCore/QThreadPool>

#include <Classification/Definitions.hpp>

#include "Scanner.hpp"

struct Worker : QRunnable
{
  Worker (Scanner *, Classification::DefinitionsPointer, QString, int);
  virtual ~Worker ();
  
  void run () Q_DECL_OVERRIDE;
  
protected:
  QByteArray content () const;
  
  Classification::DefinitionsPointer m_definitions;
  
  QString  m_src;
  Scanner *m_scanner;
  int      m_texture_limit;
};
