#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>

namespace Widgets
{
  struct FilenameLineEdit : QWidget
  {
    Q_OBJECT
    
  public:
    FilenameLineEdit(bool accept_directories=false, QString text=QString());
    virtual ~FilenameLineEdit();
    
    QString text() const;
    void setText(QString text);
    
    void setAcceptDirectories(bool accept);
    
  Q_SIGNALS:
    void textChanged(const QString &);
    
  protected:
    void createGui();
    
    QLineEdit *m_line_edit;
    bool       m_accept_directories;
  };
}
