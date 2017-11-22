#pragma once

#include <QtWidgets/QComboBox>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWizardPage>

#include <Common/Classification/AirlineModel.hpp>
#include <Common/Classification/Afiliates.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/ObjectModelPrefiltered.hpp>
#include <Common/Classification/Object.hpp>

#include <Common/Widgets/FilenameLineEdit.hpp>

#include "PreviewablesModel.hpp"

struct AfiliatesPage : QWidget
{
    AfiliatesPage (Classification::DefinitionsPointer definitions, QWidget *parent = Q_NULLPTR);
  virtual ~AfiliatesPage ();
  
  void resetDefinitions (Classification::DefinitionsPointer definitions);
  
protected:
  void updatePreviewables ();
  void showPreviewDialog ();
  
  void createGui();
  QWidget *createPreviewsGui();
  QWidget *createFiltersGui();
  
  Classification::DefinitionsPointer m_definitions;
  Classification::Afiliations m_afiliations;
  
  QComboBox *m_airline;
  Classification::AirlineModelPointer m_airline_model;
  
  QTableView *m_previewables;
  ObjectsByXPClassModelPointer m_previewables_model;
};
