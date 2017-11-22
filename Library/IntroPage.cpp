#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QThread>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

#include <Common/Classification/Definitions.hpp>
#include <Common/Widgets/FilenameLineEdit.hpp>

#include "IntroPage.hpp"
#include "Wizard.hpp"


IntroPage::IntroPage (QString dst, QString src, QString use, QWidget *parent)
  : QWizardPage (parent)
  , m_definitions (use)
  , m_scan_src (src)
  , m_scan_dst (dst)
  , m_scan_threads (QThread::idealThreadCount() / 2)
  , m_scan_texture_limit (-1)
  , m_want_reuse (true)
  
{
  createGui ();
  
  setTitle("Welcome");
}


IntroPage::~IntroPage ()
{
}



QWidget *
IntroPage::createScanGroup()
{
  QGroupBox *group = new QGroupBox("Create Options");
  QGridLayout *layout = new QGridLayout();
  
  FilenameLineEdit *dst = new FilenameLineEdit(true, m_scan_dst);
  FilenameLineEdit *src = new FilenameLineEdit(true, m_scan_src);
  
  QSpinBox *threads = new QSpinBox ();
  QComboBox *texture_limit = new QComboBox ();
  
  threads->setRange (1, QThread::idealThreadCount()+1);
  threads->setValue (m_scan_threads);
  
  texture_limit->addItem ("No Limit", QVariant(-1));
  for (unsigned int i=0x4000; i>=0x40; i>>=1) {
    texture_limit->addItem (QString("%1x%1").arg(i), QVariant(i));
  }
  
  connect (src, &FilenameLineEdit::textChanged,
    [this](const QString &path)
    {
      m_scan_src = path;
      emit completeChanged();
    }
  );
  
  connect (dst, &FilenameLineEdit::textChanged,
    [this](const QString &path)
    {
      m_scan_dst = path;
      emit completeChanged();
    }
  );
  
  connect (threads, (void (QSpinBox::*)(int))(&QSpinBox::valueChanged),
    [this](const int &value)
    {
      m_scan_threads = value;
    }
  );
  
  connect (texture_limit, (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged),
    [this, texture_limit]()
    {
      m_scan_texture_limit = texture_limit->currentData().toInt();
    }
  );
  
  int row = 0;
  layout->addWidget (new QLabel("Output Path"), row, 0, 1, 1);
  layout->addWidget (dst, row, 1, 1, 2);
  
  layout->addWidget (new QLabel("Scan Path"), ++row, 0, 1, 1);
  layout->addWidget (src, row, 1, 1, 2);
  
  layout->addWidget (new QLabel("Worker Threads"), ++row, 0, 1, 1);
  layout->addWidget (threads, row, 1, 1, 1);
  
  layout->addWidget (new QLabel("Texture Limit"), ++row, 0, 1, 1);
  layout->addWidget (texture_limit, row, 1, 1, 1);
  
  group->setLayout(layout);
  
  return (group);
}



QWidget *
IntroPage::createReuseGroup ()
{
  QGroupBox *group = new QGroupBox("Reuse Options");
  QGridLayout *layout = new QGridLayout();
  
  FilenameLineEdit *edit = new FilenameLineEdit(false, m_definitions);
  QLabel *description = new QLabel("Path to found.json");
  
  connect (edit, &FilenameLineEdit::textChanged,
    [this](const QString &path)
    {
      m_definitions = path;
      emit completeChanged();
    }
  );
  
  layout->addWidget(description, 0, 0, 1, 1);
  layout->addWidget(edit, 0, 1, 1, 2);
  
  group->setLayout(layout);
  return (group);
}


void
IntroPage::createGui ()
{
  QVBoxLayout *layout = new QVBoxLayout ();
  
  QRadioButton *scan = new QRadioButton ("Generate New");
  QRadioButton *reuse = new QRadioButton ("Reuse Old");
  
  QWidget *scan_options = createScanGroup();
  QWidget *reuse_options = createReuseGroup();
  
  scan->setChecked(!m_want_reuse);
  scan_options->setEnabled(!m_want_reuse);
  
  reuse->setChecked(m_want_reuse);
  reuse_options->setEnabled(m_want_reuse);
  
  connect (scan, &QRadioButton::clicked,
    [this,scan_options,reuse_options]()
    {
      m_want_reuse = false;
      scan_options->setEnabled(!m_want_reuse);
      reuse_options->setEnabled(m_want_reuse);
      emit completeChanged();
    }
  );
  
  connect (reuse, &QRadioButton::clicked,
    [this,scan_options,reuse_options]()
    {
      m_want_reuse = true;
      scan_options->setEnabled(!m_want_reuse);
      reuse_options->setEnabled(m_want_reuse);
      completeChanged();
    }
  );
  
  layout->addWidget(scan);
  layout->addWidget(scan_options);
  layout->addWidget(reuse);
  layout->addWidget(reuse_options);
  setLayout(layout);
}



bool
IntroPage::isComplete() const
{
  bool ok = false;
  
  if (m_want_reuse) {
    ok = QFileInfo(m_definitions).exists();
  } else {
    ok = QFileInfo(m_scan_dst).isDir() && QFileInfo(m_scan_src).isDir();
  }
  
  return (ok);
}


bool
IntroPage::validatePage()
{
  bool ok = false;
  Wizard *wiz = dynamic_cast<Wizard*>(wizard());
  
  
  try {
    if (m_want_reuse) {
      QDir d(m_definitions);
      d.cdUp();
      wiz->setDefinitions (
        d.absolutePath(),
        Classification::Definitions::fromFile(m_definitions)
      );
    } else {
      wiz->setScanSettings(m_scan_dst, m_scan_src, m_scan_threads, m_scan_texture_limit);
    }
    ok = true;
  } catch (const std::exception &e) {
    QMessageBox::critical(this, "Error", QString(e.what()));
  }
  
  return (ok);
}



int
IntroPage::nextId() const
{
  if (m_want_reuse) {
    return (Wizard::Afiliates);
  } else {
    return (Wizard::Scan);
  }
}
