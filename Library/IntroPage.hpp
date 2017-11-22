#pragma once

#include <QtWidgets/QWizardPage>


struct IntroPage : QWizardPage
{
  IntroPage (QString dst=QString(), QString src=QString(), QString use=QString(), QWidget *parent = Q_NULLPTR);
  virtual ~IntroPage ();
  
protected:
  void createGui();
  QWidget *createReuseGroup();
  QWidget *createScanGroup();
  
  bool isComplete() const Q_DECL_OVERRIDE;
  bool validatePage() Q_DECL_OVERRIDE;
  int  nextId() const Q_DECL_OVERRIDE;
  
  QString m_definitions;
  QString m_scan_src;
  QString m_scan_dst;
  int     m_scan_threads;
  int     m_scan_texture_limit;
  bool m_want_reuse;
};
