#include <ciso646>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "FilenameLineEdit.hpp"

namespace Widgets
{
  FilenameLineEdit::FilenameLineEdit (bool accept, QString path)
  : QWidget()
  , m_line_edit(Q_NULLPTR)
  , m_accept_directories(accept)
  {
    createGui();
    
    setText(path);
  }
  
  
  FilenameLineEdit::~FilenameLineEdit ()
  {
  }
  
  
  
  void
  FilenameLineEdit::createGui ()
  {
    QHBoxLayout *layout = new QHBoxLayout;
    m_line_edit         = new QLineEdit;
    QPushButton *browse = new QPushButton("browse ...");
    
    layout->addWidget(m_line_edit);
    layout->addWidget(browse);
    
    setLayout(layout);
    
    connect(
      browse, &QPushButton::released,
      [this]() {
        QString result;
        if (m_accept_directories)
          result = QFileDialog::getExistingDirectory(this);
        else
          result = QFileDialog::getOpenFileName(this);
      
        if (not result.isNull()) {
          m_line_edit->setText (result);
        }
      }
    );
    
    connect(
      m_line_edit, &QLineEdit::textChanged,
      [this](const QString &path) {
        emit textChanged(path);
      }
    );
  }
  
  
  
  QString
  FilenameLineEdit::text() const
  {
    return (m_line_edit->text());
  }
  
  
  
  void
  FilenameLineEdit::setText(QString text)
  {
    m_line_edit->setText(text);
  }
  
  
  
  void
  FilenameLineEdit::setAcceptDirectories(bool accept)
  {
    m_accept_directories = accept;
  }
}
