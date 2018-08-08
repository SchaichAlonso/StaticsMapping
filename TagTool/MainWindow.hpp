#pragma once

#include <QtCore/QList>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QDataWidgetMapper>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QMainWindow>

#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/ObjectModel.hpp>

#include <Common/Widgets/ObjScreen.hpp>
#include <Common/Widgets/VisualObject.hpp>

struct MainWindow : QMainWindow
{
  Q_OBJECT;
  
public:
  MainWindow (QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  virtual ~MainWindow ();
    
public slots:
  void loadObjFile (QString);
  void loadObjFile ();
  void saveObjFile ();
  
  void saveDefinitions ();
  void saveDefinitionsAs ();
  void saveDefinitionsAs (QString);
  
  void showSupportedImageFormats ();
  
protected Q_SLOTS:
  void setDisplayedVisualObject (int);
  
  void unloadObjFile ();
  
  void showAircraftTableDialog ();
  void showAirlineHierarchyDialog ();
  void showAirlineTableDialog ();
  void showLibraryTableDialog ();
  void showObjectTableDialog ();
  
  void nonInteractiveInsert(Widgets::VisualObjectPointer obj);
  void interactiveInsert(Widgets::VisualObjectPointer obj);
  
protected:
  virtual void dragEnterEvent (QDragEnterEvent *) Q_DECL_OVERRIDE;
  virtual void dropEvent (QDropEvent *) Q_DECL_OVERRIDE;
  
  QMenuBar *createMenuBar();
  QComboBox *createFormComboBox(QStringList contents=QStringList());
  QComboBox *createFormComboBox(QMetaEnum);
  QDoubleSpinBox *createFormMetricInputSpinner();
  QLayout   *createForm();
  void createWidgets ();
  
protected:
  Classification::DefinitionsPointer m_definitions;
  
  Widgets::ObjScreen        *m_obj_screen;
  QList<Widgets::VisualObjectPointer> m_objects;
  
  QComboBox                   *m_object_select;
  Classification::ObjectModel *m_object_data_model;
  QDataWidgetMapper           *m_object_data_mapper;
  
  
  
  int m_current_visual_object_index;
};
