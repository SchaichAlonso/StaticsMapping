#pragma once

#include <QtCore/QSharedPointer>
#include <QtWidgets/QWidget>

#include <Common/Classification/Definitions.hpp>

#include "GraphWidget.hpp"

namespace Widgets
{
  struct AirlineGraphWidget : GraphWidget
  {
    AirlineGraphWidget(QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~AirlineGraphWidget();
    
    void layout(Classification::Definitions *, QString);
    
    QString     defaultLayoutEngine() const;
    QStringList layoutEngines() const;
  };
}
