#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include "ScanDialog.hpp"
#include "Scanner.hpp"

ScanDialog::ScanDialog (QWidget *p, Qt::WindowFlags f)
  : QDialog (p, f)
{
  QPushButton *dst = new QPushButton ("browse...");
  QPushButton *src = new QPushButton ("browse...");
  QPushButton *scan = new QPushButton ("scan");
  QGridLayout *form = new QGridLayout ();
  
  m_src = new QLineEdit ();
  m_dst = new QLineEdit ();
  m_threads = new QSpinBox ();
  m_texture_limit = new QComboBox ();
  
  m_texture_limit->addItem ("No Limit", QVariant(-1));
  for (unsigned int i=0x4000; i>=0x40; i>>=1) {
    m_texture_limit->addItem (
        QString ("%1x%1").arg(i),
        QVariant (i)
    );
  }
  
  int row = 0;
  form->addWidget (new QLabel("Input"), row, 0);
  form->addWidget (m_src, row, 1);
  form->addWidget (src,   row, 2);
  form->addWidget (new QLabel("Output"), ++row, 0);
  form->addWidget (m_dst, row, 1);
  form->addWidget (dst,   row, 2);
  form->addWidget (new QLabel("Limit Texture Resolution"), ++row, 0);
  form->addWidget (m_texture_limit, row, 1);
  form->addWidget (new QLabel("Workers"), ++row, 0);
  form->addWidget (m_threads, row, 1);
  form->addWidget (scan,  ++row, 2);
  
  int cores = QThread::idealThreadCount();
  m_threads->setRange (1, cores);
  m_threads->setValue ((cores / 2) - 1);
  
  setLayout (form);
  
  connect (dst, &QPushButton::released,
           this, &ScanDialog::browseDestination);
  
  connect (src, &QPushButton::released,
           this, &ScanDialog::browseSource);
  
  connect (scan, &QPushButton::released,
           this, &ScanDialog::scan);
}


ScanDialog::~ScanDialog ()
{
}



void
ScanDialog::scan ()
{
  Scanner scanner (m_dst->text(), m_texture_limit->currentData().toInt());
  
  scanner.setMaxThreadCount (m_threads->value());
  
  scanner.scan (m_src->text());
}



void
ScanDialog::browseDestination ()
{
  QString dir = QFileDialog::getExistingDirectory (this);
  if (! dir.isEmpty()) {
    m_dst->setText(dir);
  }
}



void
ScanDialog::browseSource ()
{
  QString dir = QFileDialog::getExistingDirectory (this);
  if (! dir.isEmpty()) {
    m_src->setText(dir);
  }
}
