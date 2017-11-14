#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>

#include <Common/Classification/Definitions.hpp>

#include "GraphWidget.hpp"

struct AirlineGraphWidget : GraphWidget
{
  AirlineGraphWidget (QWidget* =Q_NULLPTR, Qt::WindowFlags =Qt::WindowFlags());
  virtual ~AirlineGraphWidget ();
  
  void layout (Classification::Definitions *, QString);
  
  QString     defaultLayoutEngine () const;
  QStringList layoutEngines () const;
};
