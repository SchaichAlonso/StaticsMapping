#include <QtCore/QSharedPointer>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDataWidgetMapper>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSplitter>

#include <Common/Classification/Definitions.hpp>

#include "AirlineGraphWidget.hpp"

namespace Widgets
{
  struct AirlineScreen : QDialog
  {
    AirlineScreen(Classification::Definitions *definitions, QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~AirlineScreen();
    
  public Q_SLOTS:
    void updateSelected();
    void relayout();
    
    void create();
    void remove();
    
  protected:
    QGroupBox *createAirlineBox();
    QGroupBox *createDefinitionsBox();
    QGroupBox *createLibGvcBox();
    
    Classification::Definitions  *m_definitions;
    QDataWidgetMapper            *m_data_mapper;
    Classification::AirlineModel *m_model;
    
    QString selected() const;
    void    setSelected(QString);
    QString airlineName(const Classification::Airline &) const;
    
    
    AirlineGraphWidget *m_tree;
    QScrollArea        *m_tree_scroll;
    
    QComboBox          *m_libgvc_engine;
    
    Q_OBJECT
  };
}
