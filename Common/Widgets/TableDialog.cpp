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
  , m_table(createTableView(m))
  , m_toolbar(createToolBar())
  {
    setLayout(createLayout());
  }
  
  
  TableDialog::~TableDialog()
  {
    QAbstractItemModel *mdl = m_table->model();
    
    m_table->setModel(Q_NULLPTR);
    delete(mdl);
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
  
  
  QTableView *
  TableDialog::createTableView(Classification::Model *model)
  {
    QTableView *table(new QTableView);
    table->setModel(model);
    table->setSortingEnabled(true);
    return (table);
  }
  
  
  QToolBar *
  TableDialog::createToolBar()
  {
    QToolBar *toolbar(new QToolBar());
    
    QAction *add(toolbar->addAction("Create"));
    QAction *drop(toolbar->addAction("Delete Selected"));
    QAction *close(toolbar->addAction("Close Window"));
    
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
    
    return (toolbar);
  }
  
  
  QLayout *
  TableDialog::createLayout()
  {
    QVBoxLayout *layout(new QVBoxLayout());
    
    layout->addWidget(m_toolbar);
    layout->addWidget(m_table);
    
    return (layout);
  }
}
