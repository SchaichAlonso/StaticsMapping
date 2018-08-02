#pragma once

#include <QtWidgets/QDialog>

#include <Classification/Afiliates.hpp>
#include <Classification/Definitions.hpp>

#include "GlobalDistributionWidget.hpp"

namespace Widgets
{
  struct GlobalDistributionDialog : QDialog
  {
    GlobalDistributionDialog (Classification::DefinitionsPointer, QWidget * =Q_NULLPTR, Qt::WindowFlags =Qt::WindowFlags());
    ~GlobalDistributionDialog ();
    
  protected:
    void createGui (Classification::DefinitionsPointer);
    QWidget *createFiltersGui ();
    void updatePreview ();
    
    Classification::Afiliations m_afilia;
    GlobalDistributionWidget   *m_globe;
    QString m_selected_airline;
  };
}
