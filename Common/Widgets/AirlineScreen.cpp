#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QTextEdit>

#include <Common/Classification/Airline.hpp>
#include <Common/Classification/AirlineModel.hpp>

#include "AirlineScreen.hpp"


namespace Widgets
{

  AirlineScreen::AirlineScreen(
    Classification::Definitions *definitions,
    QWidget *p,
    Qt::WindowFlags f
  )
  : QDialog(p, f)
  , m_definitions(definitions)
  , m_data_mapper(new QDataWidgetMapper(this))
  , m_model(definitions->airlineModel())
  , m_tree(new AirlineGraphWidget)
  , m_tree_scroll(new QScrollArea)
  , m_libgvc_engine(new QComboBox)
  {
    QBoxLayout *primary, *secondary;
    
    primary   = new QHBoxLayout;
    secondary = new QVBoxLayout;
    
    m_tree_scroll->setWidget(m_tree);
    
    secondary->addWidget(createAirlineBox());
    secondary->addWidget(createDefinitionsBox());
    secondary->addWidget(createLibGvcBox());
    
    primary->addWidget(m_tree_scroll, 1);
    primary->addLayout(secondary);
    
    setLayout(primary);
    
    connect(
      m_model, &Classification::AirlineModel::modelReset,
      this, &AirlineScreen::relayout
    );
    connect(
      m_tree, &AirlineGraphWidget::nodeClicked,
      this, &AirlineScreen::setSelected
    );
    
    relayout();
  }



  AirlineScreen::~AirlineScreen()
  {
  }



  void
  AirlineScreen::updateSelected()
  {
    QPointF want;
    QString icao;
    
    icao = selected();
    want = m_tree->position(icao);
    
    m_tree->resetSelected();
    m_tree->setSelected(icao, true);
    
    m_tree_scroll->ensureVisible(
      want.x(),
      want.y(),
      m_tree_scroll->width()/2,
      m_tree_scroll->height()/2
    );
    
    m_tree->update();
  }



  void
  AirlineScreen::relayout()
  {
    m_tree->layout(m_definitions, m_libgvc_engine->currentText());
    updateSelected();
  }



  QString
  AirlineScreen::selected() const
  {
    QModelIndex mi;
    int row;
    
    row = m_data_mapper->currentIndex();
    mi = m_model->index(row, 0);
    
    return (m_model->data(mi, Qt::UserRole).toString());
  }



  void
  AirlineScreen::setSelected(QString icao)
  {
    Classification::AirlinePointer ptr = m_definitions->airline(icao);
    if (ptr) {
      int index = m_definitions->indexOf(ptr);
    
      if (index != -1) {
        m_data_mapper->setCurrentIndex(index);
      }
    }
  }



  QString
  AirlineScreen::airlineName(const Classification::Airline &airline) const
  {
    QString retval(
      tr("%1 (%2)")
      .arg(airline.icao())
      .arg(airline.name())
    );
    
    return (retval);
  }



  void
  AirlineScreen::create()
  {
    Classification::Airline op;
    QString icao;
    bool    ok;
    
    do {
      icao = QInputDialog::getText(
        this,
        tr("ICAO Code"),
        tr("ICAO Airline Identifier:"),
        QLineEdit::Normal,
        "AAL",
        &ok
      );
      
      if (not ok) {
        return;
      }
      
      ok = op.verifyIcaoCode(icao);
      if (not ok) {
        QMessageBox::warning(
          this,
          tr("ICAO Code"),
          tr("Invalid ICAO Airline Code.")
        );
        continue;
      }
      
      ok = not m_definitions->airline(icao);
      if (ok) {
        Classification::AirlinePointer ptr(new Classification::Airline);
        ptr->setIcao(icao);
        
        m_definitions->upsert (ptr);
      } else {
        QMessageBox::warning(
          this,
          tr("ICAO Code"),
          QString(tr("Airline %1 already exists.")).arg(icao)
        );
      }
    } while (not ok);
  }



  void
  AirlineScreen::remove()
  {
    QMessageBox::StandardButton input;
    QString icao;
    
    icao = selected();
    
    if (icao.isEmpty()) {
      return;
    }
    
    input = QMessageBox::question(
      this,
      tr("Confirm"),
      tr("Deleting the airline will purge all associated objects. Continue?")
    );
    
    if (input == QMessageBox::Yes) {
      int deleted = m_definitions->drop(m_definitions->airline(icao));
      QMessageBox::information(
        this,
        tr("Report"),
        tr("Deleted %1 entities.").arg(deleted)
      );
    }
  }



  QGroupBox *
  AirlineScreen::createAirlineBox()
  {
    QGroupBox *retval;
    QFormLayout *form;
    
    QComboBox *icao, *owner;
    QLineEdit *name, *area;
    QTextEdit *comment;
    
    icao  = new QComboBox;
    owner = new QComboBox;
    name  = new QLineEdit;
    area  = new QLineEdit;
    comment = new QTextEdit;
    
    
    retval = new QGroupBox(tr("airline data"));
    form = new QFormLayout;
    form->addRow("Airline:", icao);
    form->addRow("Name:",    name);
    form->addRow("Area:",    area);
    form->addRow("Comment:", comment);
    form->addRow("Parent:",  owner);
    icao->setEditable(false);
    owner->setEditable(false);
    
    icao->setModel(m_model);
    icao->setModelColumn(m_model->column(Classification::Airline::IcaoProperty));
    owner->setModel(m_model);
    owner->setModelColumn(m_model->column(Classification::Airline::IcaoProperty));
    
    m_data_mapper->setModel(m_model);
    m_data_mapper->addMapping(icao, m_model->column(Classification::Airline::IcaoProperty), "currentText");
    m_data_mapper->addMapping(name, m_model->column(Classification::Airline::NameProperty));
    m_data_mapper->addMapping(area, m_model->column(Classification::Airline::HubsProperty));
    m_data_mapper->addMapping(comment, m_model->column(Classification::Airline::CommentProperty), "plainText");
    m_data_mapper->addMapping(owner, m_model->column(Classification::Airline::ParentProperty), "currentText");
    m_data_mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    m_data_mapper->toFirst();
    
    connect(
      icao, SIGNAL(currentIndexChanged(int)),
      m_data_mapper, SLOT(setCurrentIndex(int))
    );
    connect(
      icao, SIGNAL(currentIndexChanged(int)),
      this, SLOT(updateSelected())
    );

    retval->setLayout(form);
    
    return (retval);
  }



  QGroupBox *
  AirlineScreen::createDefinitionsBox()
  {
    
    QGroupBox *retval;
    QPushButton *create, *remove;
    QGridLayout *grid;
    
    retval = new QGroupBox(tr("definitions"));
    grid = new QGridLayout();
    
    create = new QPushButton("create");
    remove = new QPushButton("remove");
    
    connect(create, &QPushButton::released, this, &AirlineScreen::create);
    connect(remove, &QPushButton::released, this, &AirlineScreen::remove);
    
    grid->addWidget(create, 0, 0);
    grid->addWidget(remove, 0, 2);
    retval->setLayout(grid);
   
    return (retval);
  }



  QGroupBox *
  AirlineScreen::createLibGvcBox()
  {
    QGroupBox *retval;
    QFormLayout *form;
    
    retval = new QGroupBox(tr("libgvc options"));
    form = new QFormLayout();
    
    m_libgvc_engine->addItems(m_tree->layoutEngines());
    
    form->addRow("engine:", m_libgvc_engine);
    
    connect(
      m_libgvc_engine, SIGNAL(currentIndexChanged(int)),
      this, SLOT(relayout())
    );
    
    retval->setLayout(form);
    
    return (retval);
  }

}
