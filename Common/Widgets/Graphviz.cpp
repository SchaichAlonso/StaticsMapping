#include "Graphviz.hpp"

/// The agopen method for opening a graph
Agraph_t*
GVGraph::agopen(QString name, Agdesc_t desc)
{
  Agraph_t *retval;
  QByteArray name_utf8;
  
  name_utf8 = name.toUtf8();
  
  retval = ::agopen (name_utf8.data(), desc, Q_NULLPTR);
  
  return (retval);
}

/// Add an alternative value parameter to the method for getting an object's attribute
QString
GVGraph::agget(void *object, QString attr, QString alt) const
{
  QString retval;
  char *ptr;
  
  ptr = ::agget(object, const_cast<char *>(qPrintable(attr)));

  if (ptr != Q_NULLPTR) {
    retval = ptr;
  } else {
    retval = alt;
  }
  
  return (retval);
}

/// Directly use agsafeset which always works, contrarily to agset
int
GVGraph::agset(void *object, QString attr, QString value)
{
    return ::agsafeset(object, const_cast<char *>(qPrintable(attr)),
                     const_cast<char *>(qPrintable(value)),
                     const_cast<char *>(qPrintable(value)));
}

Agnode_t *
GVGraph::agnode (Agraph_t *graph, QString const& name)
{
  Agnode_t *retval;
  QByteArray name_utf8;
  
  name_utf8 = name.toUtf8();
  
  retval = ::agnode (graph, name_utf8.data(), 1);
  
  agset (retval, "fixedsize", "true");
  agset (retval, "label", name);
  agset (retval, "regular", "true");
  //agset (retval, "width", )
  
  return (retval);
}








const qreal GVGraph::DotDefaultDPI=72.0;

GVGraph::GVGraph(QString name, QFont font, qreal node_size)
  : _context (gvContext())
  , _graph (agopen(name, Agstrictundirected))
{
    //Set graph attributes
    agset(_graph, "overlap", "prism");
    agset(_graph, "splines", "true");
    agset(_graph, "pad", "0,2");
    agset(_graph, "dpi", "96,0");
    agset(_graph, "nodesep", "0,6");

    //Set default attributes for the future nodes
    //_agnodeattr(_graph, "fixedsize", "true");
    //_agnodeattr(_graph, "label", "");
    //_agnodeattr(_graph, "regular", "true");

    //Divide the wanted width by the DPI to get the value in points
    //QString nodePtsWidth("%1").arg(node_size/_agget(_graph, "dpi", "96,0").toDouble());
    //GV uses , instead of . for the separator in floats
    //_agnodeattr(_graph, "width", nodePtsWidth.replace('.', ","));

    setFont(font);
}

GVGraph::~GVGraph()
{
    gvFreeLayout(_context, _graph);
    agclose(_graph);
    gvFreeContext(_context);
}


void
GVGraph::addNode (const QString &name)
{
    if(_nodes.contains(name))
        removeNode(name);

    _nodes.insert(name, agnode(_graph, name));
}

void
GVGraph::addNodes (const QStringList &names)
{
    for(int i=0; i<names.size(); ++i)
        addNode(names.at(i));
}

void
GVGraph::removeNode(const QString &name)
{
    if(_nodes.contains(name))
    {
        agdelete(_graph, _nodes[name]);
        _nodes.remove(name);

        QList<QPair<QString, QString> > keys=_edges.keys();
        for(int i=0; i<keys.size(); ++i)
            if(keys.at(i).first==name || keys.at(i).second==name)
                removeEdge(keys.at(i));
    }
}

void GVGraph::clearNodes()
{
    QList<QString> keys=_nodes.keys();

    for(int i=0; i<keys.size(); ++i)
        removeNode(keys.at(i));
}


void
GVGraph::setRootNode (const QString &name)
{
    if(_nodes.contains(name))
        agset(_graph, "root", name);
}


void
GVGraph::addEdge(const QString &source, const QString &target)
{
  Q_ASSERT (_nodes.contains(source));
  Q_ASSERT (_nodes.contains(target));
  
  QPair<QString, QString> key(source, target);
  if(!_edges.contains(key))
    _edges.insert(key, agedge(_graph, _nodes[source], _nodes[target], "", 1));
}

void
GVGraph::removeEdge(const QString &source, const QString &target)
{
    removeEdge(QPair<QString, QString>(source, target));
}

void
GVGraph::removeEdge(const QPair<QString, QString> &key)
{
    if(_edges.contains(key))
    {
        agdelete(_graph, _edges[key]);
        _edges.remove(key);
    }
}


void
GVGraph::setFont (QFont font)
{
    _font=font;

    agset(_graph, "fontname", font.family());
    agset(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    //_agnodeattr(_graph, "fontname", font.family());
    //_agnodeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));

    //_agedgeattr(_graph, "fontname", font.family());
    //_agedgeattr(_graph, "fontsize", QString("%1").arg(font.pointSizeF()));
}



void
GVGraph::applyLayout ()
{
    //gvFreeLayout(_context, _graph);
    //gvRender (_context, _graph, "plain", f);
    gvLayout(_context, _graph, "dot");
}

QRectF GVGraph::boundingRect() const
{
  boxf bb;
  
  bb = GD_bb(_graph);
  
  qreal factor = resolution() / DotDefaultDPI;
  
  return QRectF(factor*bb.LL.x, factor*bb.LL.y,
                factor*bb.UR.x, factor*bb.UR.y);
}



QList<GVNode> GVGraph::nodes() const
{
  QList<GVNode> list;
  qreal dpi, factor;
  
  dpi = resolution ();
  factor = dpi / DotDefaultDPI;
  
    boxf bb;
    bb = GD_bb(_graph);
    
    for(QMap<QString, Agnode_t*>::ConstIterator it=_nodes.begin(); it!=_nodes.end();++it)
    {
        Agnode_t *node=it.value();
        GVNode object;
        pointf coord;
        
        //Set the name of the node
        object.name=ND_label(node)->text;

        coord = ND_coord(node);
        
        //Fetch the X coordinate, apply the DPI conversion rate (actual DPI / 72, used by dot)
        qreal x = coord.x*factor;

        //Translate the Y coordinate from bottom-left to top-left corner
        qreal y=(bb.UR.y - coord.y)*factor;
        object.centerPos=QPoint(x, y);

        //Transform the width and height from inches to pixels
        object.height=ND_height(node)*dpi;
        object.width=ND_width(node)*dpi;

        list << object;
    }
    
    return (list);
}


QList<GVEdge> GVGraph::edges() const
{
    QList<GVEdge> list;
    qreal dpi, factor;
    boxf bb;
    
    dpi = resolution ();
    factor = dpi / DotDefaultDPI;
    bb = GD_bb (_graph);
    
    for(QMap<QPair<QString, QString>, Agedge_t*>::ConstIterator it=_edges.constBegin();
        it!=_edges.constEnd();++it)
    {
        Agedge_t *edge=it.value();
        GVEdge object;
        splines *spl;

        //Fill the source and target node names
        textlabel_t *src, *dst;
        
        src = ED_tail_label(edge);
        dst = ED_tail_label(edge);
        
        //object.source = src->text;
        //object.target = dst->text;
        
        spl = ED_spl (edge);

        //Calculate the path from the spline (only one spline, as the graph is strict. If it
        //wasn't, we would have to iterate over the first list too)
        //Calculate the path from the spline (only one as the graph is strict)
        if((spl->list!=0) && (spl->list->size%3 == 1))
        {
            //If there is a starting point, draw a line from it to the first curve point
            if(spl->list->sflag)
            {
                object.path.moveTo(spl->list->sp.x*factor,
                             (bb.UR.y - spl->list->sp.y)*factor);
               // object.path.lineTo(spl->list->list[0].x*factor,
               //         (bb.UR.y - spl->list->list[0].y)*factor);
                object.path.cubicTo(spl->list->list[0].x*factor,
                    (bb.UR.y - spl->list->list[0].y)*factor,
                    spl->list->list[0].x*factor,
                                            (bb.UR.y - spl->list->list[0].y)*factor,
                    spl->list->list[0].x*factor,
                                            (bb.UR.y - spl->list->list[0].y)*factor
                      );
            }
            else
                object.path.moveTo(spl->list->list[0].x*factor,
                        (bb.UR.y - spl->list->list[0].y)*factor);

            //Loop over the curve points
            for(int i=1; i<spl->list->size; i+=3)
                object.path.cubicTo(spl->list->list[i].x*factor, 
                      (bb.UR.y - spl->list->list[i].y)*factor,
                      spl->list->list[i+1].x*factor,
                      (bb.UR.y - spl->list->list[i+1].y)*factor,
                      spl->list->list[i+2].x*factor,
                      (bb.UR.y - spl->list->list[i+2].y)*factor);

            //If there is an ending point, draw a line to it
            if(spl->list->eflag)
                object.path.lineTo(spl->list->ep.x*factor,
                             (bb.UR.y - spl->list->ep.y)*factor);
        }

        list << object;
    }

    return list;
}


double
GVGraph::resolution () const
{
  QString dpi;
  qreal   retval;
  
  dpi = agget(_graph, "dpi", "96,0");
  dpi.replace (",", ".");
  
  retval = dpi.toDouble ();
  return (retval);
}
