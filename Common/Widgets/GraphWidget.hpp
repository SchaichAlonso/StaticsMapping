#pragma once

#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QSet>
#include <QtCore/QSizeF>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>

struct Curve
{
  Curve (const QPointF & =QPointF());
  Curve (const QPointF &, const QPointF &, const QPointF &);
  
  QPointF points[3];
  bool    is_line;
};

struct Edge
{
  QPointF start;
  QList<Curve> curves;
  
  QString src, dst;
};

struct Node
{
  QString label;
  QPointF center;
  QSizeF  dimensions;
  
  bool    selected;
};

struct GraphWidget : QWidget
{
  GraphWidget (QWidget* =Q_NULLPTR, Qt::WindowFlags =Qt::WindowFlags());
  virtual ~GraphWidget ();
  
  QPointF position (QString) const;
  
public slots:
  void resetSelected ();
  void setSelected (QString, bool);
  
Q_SIGNALS:
  void nodeClicked (QString);
  
protected:
  virtual void mousePressEvent (QMouseEvent* event) Q_DECL_OVERRIDE;
  virtual void paintEvent (QPaintEvent *) Q_DECL_OVERRIDE;
  
protected:
  typedef QList<Edge> Edges;
  typedef QList<Node> Nodes;
  
  Edges m_edges;
  Nodes m_nodes;
  
  Q_OBJECT
};
