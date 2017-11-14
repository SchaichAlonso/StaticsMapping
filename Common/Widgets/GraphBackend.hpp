#pragma once

#include <QtCore/QString>
#include <QtCore/QMap>

#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>

#include "GraphWidget.hpp"

struct GraphBackend
{
  GraphBackend (const QFont &, int);
  virtual ~GraphBackend ();
  
  void addEdge(const QString &, const QString &);
  void addNode(const QString &);
  
  void setFont (const QFont &font);
  
  void setRootNode (const QString &name);
  
  void applyLayout (const QString &engine);
  QRectF boundingRect () const;
  
  QList<Edge> edges() const;
  QList<Node> nodes () const;
  
protected:
    Agraph_t* agopen (QString name, Agdesc_t desc);
    QString   agget (void *object, QString attr, QString alt=QString()) const;
    int       agset (void *object, QString attr, QString value);
    Agnode_t *agnode (Agraph_t *graph, QString const& name);

    double    resolution () const;
    
    static QPointF scaled (boxf const&, double, double, double);
    
private:
    typedef QMap<QString, Agnode_t *> Nodes;
    typedef QMap<QPair<QString, QString>, Agedge_t *> Edges;
    
    GVC_t *m_context;
    Agraph_t *m_graph;
    Nodes m_nodes;
    Edges m_edges;
    
    static double g_default_resolution;
};
