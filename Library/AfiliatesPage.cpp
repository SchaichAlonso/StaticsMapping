#include <QtCore/QDir>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDataWidgetMapper>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>

#include <Common/Classification/AfiliateModel.hpp>
#include <Common/Classification/AirlineModel.hpp>
#include <Common/Widgets/AirlineDelegate.hpp>
#include <Common/Widgets/GlobalDistributionDialog.hpp>

#include "AfiliatesPage.hpp"
#include "ObjViewDialog.hpp"
#include "PreviewablesModel.hpp"



AfiliatesPage::AfiliatesPage (
  QString prefix,
  Classification::DefinitionsPointer definitions,
  QWidget *parent
)
  : QWizardPage (parent)
  , m_definitions (definitions)
  , m_afiliations (definitions)
  , m_airline (new QComboBox)
  , m_airline_model (definitions->airlineModel())
  , m_previewables (new QTableView)
  , m_previewables_model (new ObjectsByXPClassModel)
  , m_prefix (prefix)
  , m_extend (true)
{
  createGui ();
  
  setTitle("Generate XPlane Library");
  
  updatePreviewables ();
}



AfiliatesPage::~AfiliatesPage ()
{
}



void
AfiliatesPage::writeLibrary ()
{
  QString path = QDir(m_prefix).absoluteFilePath("library.txt");
  QFile out(path);
  
  if (out.open(QFile::WriteOnly | QFile::Truncate)) {
    out.write(m_afiliations.library().toString(m_extend).toUtf8());
    out.close();
    QMessageBox::information(this, "Report", QString("Wrote %1").arg(path));
  } else {
    QMessageBox::critical(this, "Error", QString("Cannot write %1").arg(path));
  }
}



void
AfiliatesPage::updatePreviewables ()
{
  Classification::AirlinePointer airline;
  Classification::WeightedObjectsByXPClass objects;
  
  if (m_definitions) {
    QString icao = m_airline->currentData().toString();
    airline = m_definitions->airline(icao);
    objects = m_afiliations.objectsAvailable (airline);
  }
  
  m_previewables_model->setObjectsByXPClass (objects);
}



void
AfiliatesPage::showPreviewDialog ()
{
  QModelIndex i = m_previewables->currentIndex();
  
  if (i.isValid()) {
    QString key = i.data(Qt::UserRole).toString();
    Classification::ObjectPointer o = m_definitions->object(key);
    
    if (o) {
      ObjViewDialog dialog(m_prefix, m_definitions, o, this);
      dialog.setWindowTitle(o->fileName());
      dialog.setModal(true);
      dialog.exec();
    }
  }
}



void
AfiliatesPage::createGui ()
{
  QVBoxLayout *layout  = new QVBoxLayout ();
  
  
  layout->addWidget (createFiltersGui ());
  layout->addWidget (createPreviewsGui ());
  
  
  QPushButton *write = new QPushButton("write library");
  connect (
    write, &QPushButton::released,
    this, &AfiliatesPage::writeLibrary
  );
  layout->addWidget(write);
  
  setLayout (layout);
}



QWidget *
AfiliatesPage::createPreviewsGui ()
{
  QGroupBox   *group   = new QGroupBox ("Previews");
  QVBoxLayout *vbox    = new QVBoxLayout ();
  QGridLayout *form    = new QGridLayout ();
  QPushButton *preview = new QPushButton ("Preview Selected");
  
  m_airline->setModel (m_airline_model.data());
  m_airline->setModelColumn (m_airline_model->column(Classification::Airline::IcaoProperty));
  m_airline->setItemDelegate (new AirlineDelegate (m_definitions));
  
  m_previewables->setModel (m_previewables_model.data());
  m_previewables->setSortingEnabled (true);
  m_previewables->horizontalHeader()->setStretchLastSection(true);
  
  void (QComboBox::*current_index_changed)(int) = &QComboBox::currentIndexChanged;
  connect (
    m_airline, current_index_changed,
    this, &AfiliatesPage::updatePreviewables
  );
  
  connect (
    preview, &QPushButton::released,
    this, &AfiliatesPage::showPreviewDialog
  );
  
  form->addWidget (new QLabel("Operator"), 0, 0, 1, 1);
  form->addWidget (m_airline, 0, 1, 1, 1);
  form->addWidget (m_previewables, 1, 0, 1, 2);
  form->setColumnStretch(0, 0);
  form->setColumnStretch(1, 1);
  
  vbox->addLayout (form);
  vbox->addWidget (preview);
  
  group->setLayout (vbox);
  
  return (group);
}



QWidget *
AfiliatesPage::createFiltersGui ()
{
  QGroupBox *group = new QGroupBox ("Library Options");
  QScrollArea *area = new QScrollArea ();
  QWidget *dummy = new QWidget();
  QFormLayout *form = new QFormLayout ();
  QComboBox *extend = new QComboBox();
  
  Classification::AfiliateModel *model = new Classification::AfiliateModel(&m_afiliations);
  
  connect (
    model, &QAbstractItemModel::dataChanged,
    this, &AfiliatesPage::updatePreviewables
  );
  
  QList<QSpinBox *> spins;
  for (int i=0; i!=model->columnCount(); ++i) {
    QSpinBox *sb = new QSpinBox(this);
    sb->setRange(0, 10000);
    spins.append(sb);
  }
  
  QDataWidgetMapper *mapper = new QDataWidgetMapper(this);
  mapper->setModel (model);
  for (int i=0; i!=spins.size(); ++i) {
    mapper->addMapping (spins[i], model->column(i));
  }
  mapper->setCurrentIndex (0);
  
  for (int i=0; i!=model->columnCount(); ++i) {
    form->addRow (
      model->headerData(i, Qt::Horizontal, Qt::DisplayRole).toString(),
      spins[i]
    );
    
    connect (
      spins[i], (void (QSpinBox::*)(int)) &QSpinBox::valueChanged,
      mapper, &QDataWidgetMapper::submit
    );
  }
  
  extend->addItem("EXPORT_EXTEND", QVariant(true));
  extend->addItem("EXPORT", QVariant(false));
  connect (
    extend, (void (QComboBox::*)(int))(&QComboBox::currentIndexChanged),
    [extend,this]() {
      m_extend = extend->currentData().toBool();
    }
  );
  form->addRow ("First Occurence", extend);
  
  dummy->setLayout (form);
  area->setWidget(dummy);
  group->setLayout(new QVBoxLayout());
  group->layout()->addWidget(area);
  
  return (group);
}
