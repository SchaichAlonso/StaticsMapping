#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QWizardPage>

#include "Scanner.hpp"

struct ScanPage : QWizardPage
{
  ScanPage (QString dst, QString src, int threads, int texlimit, QWidget *parent = Q_NULLPTR);
  virtual ~ScanPage ();
  
  void appendDiagnostic (const QString &text);
  
protected:
  void createGui();
  void completed();
  
  virtual bool isComplete() const Q_DECL_OVERRIDE;
  virtual int  nextId() const Q_DECL_OVERRIDE;
  
  QPlainTextEdit *m_output;
  QSharedPointer<Scanner> m_scanner;
};
