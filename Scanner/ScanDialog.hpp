#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

#include "ScanDialog.hpp"

struct ScanDialog : QDialog
{
  ScanDialog (QWidget * =Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());
 ~ScanDialog ();
  
protected:
  void browseDestination ();
  void browseSource ();
  
  void scan ();
  
  QComboBox *m_texture_limit;
  QLineEdit *m_src, *m_dst;
  QSpinBox  *m_threads;
};
