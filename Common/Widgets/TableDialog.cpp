#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>

#include "TableDialog.hpp"

namespace Widgets
{
  TableDialog::TableDialog(
    Classification::DefinitionsPointer defs,
    Classification::Model *m,
    QString primary_key_name,
    QWidget *parent,
    Qt::WindowFlags flags
  )
  : QDialog(parent, flags)
  , m_definitions(defs)
  , m_primary_key_name(primary_key_name)
  {
    createWidgets (m);
  }
  
  
  TableDialog::~TableDialog()
  {
  }
  
  
  
  void
  TableDialog::handleInsertRequest()
  {
    QString key;
    bool ok;
    
    do {
      key = QInputDialog::getText(
        this,
        tr("Input Required"),
        tr("Enter %1").arg(m_primary_key_name),
        QLineEdit::Normal,
        QString(),
        &ok
      );
      
      if (not ok) {
        return;
      }
      
      ok = verify (key);
      if (not ok) {
        QMessageBox::warning(
          this,
          tr("Error"),
          tr("Poor %1: \"%2\"")
          .arg(m_primary_key_name)
          .arg(key)
        );
        continue;
      }
      
      ok = checkDuplicate(key);
      if (ok) {
        insert(key);
      } else {
        QMessageBox::warning(
          this,
          tr("Error"),
          tr("%1 already exists.")
          .arg(m_primary_key_name)
          .arg(key)
        );
      }
    } while (not ok);
  }
  
  
  void
  TableDialog::handleDropRequest()
  {
    QModelIndex mi = m_table->currentIndex();
    
    if (mi.isValid()) {
      
      QString key = mi.data(Qt::UserRole).toString();
      
      QMessageBox::StandardButton input;
      input = QMessageBox::question(
        this,
        tr("Confirm"),
        tr("Delete %1 \"%2\" as well as all associated records. Continue?")
          .arg(m_primary_key_name)
          .arg(key)
      );
      
      if (input == QMessageBox::Yes) {
        int n = drop(key);
        QMessageBox::information(
          this,
          tr("Report"),
          tr("%1 Records were removed.").arg (n)
        );
      }
    }
  }
  
  
  
  void
  TableDialog::createWidgets(Classification::Model *model)
  {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QToolBar    *toolbar = new QToolBar();
    QAction     *add, *drop, *close;
    
    m_table = new QTableView();
    
    m_table->setModel(model);
    m_table->setSortingEnabled(true);
    
    add    = toolbar->addAction("Create");
    drop   = toolbar->addAction("Delete Selected");
    close  = toolbar->addAction("Close Window");
    
    layout->addWidget(toolbar);
    layout->addWidget(m_table);
    
    connect(
      add, &QAction::triggered,
      this, &TableDialog::handleInsertRequest
    );
    connect(
      drop, &QAction::triggered,
      this, &TableDialog::handleDropRequest
    );
    connect(
      close, &QAction::triggered,
      this,  &QDialog::accept
    );
  }
}
