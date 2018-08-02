#pragma once

#include <QtCore/QList>
#include <QtCore/QPointF>
#include <QtCore/QRectF>
#include <QtCore/QSet>
#include <QtCore/QSizeF>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>
#include <QtWidgets/QWidget>


namespace Widgets
{
  struct Curve
  {
    Curve(const QPointF &p=QPointF());
    Curve(const QPointF &p1, const QPointF &p2, const QPointF &p3);
    
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
    GraphWidget(QWidget *parent=Q_NULLPTR, Qt::WindowFlags flags=Qt::WindowFlags());
    virtual ~GraphWidget();
    
    QPointF position(QString label) const;
    
  public slots:
    void resetSelected();
    void setSelected(QString label, bool value);
    
  Q_SIGNALS:
    void nodeClicked(QString label);
    
  protected:
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    
  protected:
    typedef QList<Edge> Edges;
    typedef QList<Node> Nodes;
    
    Edges m_edges;
    Nodes m_nodes;
    
    Q_OBJECT
  };
}
