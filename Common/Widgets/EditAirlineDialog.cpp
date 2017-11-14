#include <QFormLayout>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "EditAirlineDialog.hpp"



EditAirlineDialog::EditAirlineDialog (QWidget *parent, Qt::WindowFlags flags)
  : QDialog (parent, flags)
{
  QVBoxLayout *vbox;
  QHBoxLayout *top, *bottom;
  QFormLayout *form;
  QPushButton *create, *remove, *revert, *update;
  
  vbox   = new QVBoxLayout ();
  top    = new QHBoxLayout ();
  bottom = new QHBoxLayout ();
  form   = new QFormLayout ();
  
  create = new QPushButton ("new");
  remove = new QPushButton ("delete");
  revert = new QPushButton ("revert");
  update = new QPushButton ("apply");
  m_icao = new QComboBox ();
  m_name = new QLineEdit ();
  m_area = new QLineEdit ();
  m_comment = new QTextEdit ();
  
  m_icao->setEditable (false);
  
  top->addWidget (m_icao);
  top->addWidget (create);
  top->addWidget (remove);
  
  form->addRow ("Name",    m_name);
  form->addRow ("Region",  m_area);
  form->addRow ("Comment", m_comment);
  
  bottom->addWidget (revert);
  bottom->addStretch ();
  bottom->addWidget (update);
  
  vbox->addLayout (top);
  vbox->addLayout (form);
  vbox->addLayout (bottom);
  
  connect (m_icao, SIGNAL(currentIndexChanged(QString)), this, SLOT(selectDisplayAirline(QString)));
  connect (create, SIGNAL(released()), this, SLOT(createAirline()));
  connect (remove, SIGNAL(released()), this, SLOT(removeAirline()));
  
  connect (revert, SIGNAL(released()), this, SLOT(revertChanges()));
  connect (update, SIGNAL(released()), this, SLOT(updateChanges()));
  
  setLayout (vbox);
}



EditAirlineDialog::~EditAirlineDialog ()
{
}



void
EditAirlineDialog::setDefinitions (QSharedPointer<Definitions> definitions)
{
  m_definitions = definitions;
  
  m_icao->clear ();
  if (m_definitions) {
    m_icao->addItems (m_definitions->m_airlines.keys());
  }
}



void
EditAirlineDialog::selectDisplayAirline (QString icao)
{
  Operator o;
  
  if (m_definitions->m_airlines.contains(icao)) {
    o = m_definitions->m_airlines[icao];
  }
  
  m_comment->setPlainText (o.comment ());
  m_name->setText (o.name ());
  m_area->setText (o.area ());
}



void
EditAirlineDialog::createAirline ()
{
  Operator op;
  QString icao;
  bool ok;
  
  do {
    icao = QInputDialog::getText (
        this,
        "ICAO Code",
        "ICAO Airline Identifier:",
        QLineEdit::Normal,
        "AAL",
        &ok);
    
    if (not ok) {
      return;
    }
    
    ok = op.icaoCodeAcceptable (icao);
    if (not ok) {
      QMessageBox::warning (
          this,
          "ICAO Code",
          "Invalid ICAO Airline Code.");
      continue;
    }
    
    ok = not m_definitions->m_airlines.contains (icao);
    if (ok) {
      Airline airline;
      airline.setIcao (icao.toUpper ());
      m_definitions->upsert (airline);
      setDefinitions (m_definitions);
      m_icao->setCurrentText (icao);
    } else {
      QMessageBox::warning (
          this,
          "ICAO Code",
          QString("Airline %1 already exists.").arg(icao));
    }
  } while (not ok);
}



void
EditAirlineDialog::removeAirline ()
{
  QMessageBox::StandardButton input;
  QString icao;
  
  icao = m_icao->currentText ();
  
  if (icao.isEmpty()) {
    return;
  }
  
  input = QMessageBox::question (
      this,
      "Confirm",
      "Deleting the airline will purge all associated objects. Continue?");
  
  if (input == QMessageBox::Yes) {
    m_definitions->purgeOperator (m_icao->currentText());
    setDefinitions (m_definitions);
  }
}



void
EditAirlineDialog::updateChanges ()
{
  Operator op;
  QString icao;
  
  icao = m_icao->currentText ();
  
  if (icao.isEmpty ()) {
    return;
  }
  
  op.setArea (m_area->text());
  op.setComment (m_comment->toPlainText());
  op.setIcao (m_icao->currentText());
  op.setName (m_name->text());
  
  m_definitions->upsert (op);
}



void
EditAirlineDialog::revertChanges ()
{
}
