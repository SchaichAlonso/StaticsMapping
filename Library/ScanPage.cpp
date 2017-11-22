#include <QtCore/QDir>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QVBoxLayout>

#include <Common/Classification/Definitions.hpp>

#include "AfiliatesPage.hpp"
#include "ScanPage.hpp"
#include "Wizard.hpp"



ScanPage::ScanPage (QString dst, QString src, int threads, int texlimit, QWidget *parent)
  : QWizardPage (parent)
  , m_output (new QPlainTextEdit)
  , m_scanner (new Scanner(dst, src, texlimit))
{
  m_scanner->setMaxThreadCount (threads);
  
  createGui ();
  
  
  setTitle("Scaning");
  
  connect (
    m_scanner.data(), &Scanner::diagnostic,
    this, &ScanPage::appendDiagnostic
  );
  
  connect (
    m_scanner.data(), &Scanner::completed,
    this, &ScanPage::completed
  );
}



ScanPage::~ScanPage ()
{
}



void
ScanPage::appendDiagnostic (const QString& text)
{
  QScrollBar *scroll = m_output->verticalScrollBar();
  qDebug("%s", qUtf8Printable(text));
  m_output->appendPlainText(text);
  scroll->setValue(scroll->maximum());
}



void
ScanPage::createGui ()
{
  QVBoxLayout *layout = new QVBoxLayout();
  
  m_output->setReadOnly(true);
  
  layout->addWidget(m_output);
  
  setLayout(layout);
}



void
ScanPage::completed ()
{
  appendDiagnostic("Complete.");
  
  Wizard *wiz = dynamic_cast<Wizard*>(wizard());
  wiz->setDefinitions(m_scanner->dstDir().absolutePath(), m_scanner->result());
  
  emit completeChanged();
}



bool
ScanPage::isComplete () const
{
  if (not m_scanner->hasCompleted()) {
    m_scanner->scan();
  }
  
  return (m_scanner->hasCompleted());
}



int
ScanPage::nextId () const
{
  return (Wizard::Afiliates);
}

