#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLayout>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>

#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Model.hpp>

namespace Widgets
{
  struct TableDialog : QDialog
  {
    TableDialog(Classification::DefinitionsPointer defs, Classification::Model *model, QString primary_key_name, QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~TableDialog();
    
    virtual bool checkDuplicate(QString) const = 0;
    virtual bool verify(QString) const = 0;
    virtual int  drop(QString) = 0;
    virtual void insert(QString) = 0;
    
  protected:
    void handleDropRequest();
    void handleInsertRequest();
    
    QTableView *createTableView(Classification::Model *model);
    QToolBar *createToolBar();
    QLayout *createLayout();
    
    Classification::DefinitionsPointer m_definitions;
    QString m_primary_key_name;
    QTableView *m_table;
    QToolBar *m_toolbar;
  
    Q_OBJECT;
  };
}
