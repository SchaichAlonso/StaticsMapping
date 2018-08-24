#include <QtWidgets/QDialog>

#include <Classification/AirportDataQuery.hpp>

namespace Widgets
{
  struct AirportDataQueryDialog : QDialog
  {
    AirportDataQueryDialog(Classification::DefinitionsPointer definitions, QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
   ~AirportDataQueryDialog();
   
   virtual int exec() Q_DECL_OVERRIDE;
   
  protected:
    Classification::AirportDataQuery m_sync;
  };
}
