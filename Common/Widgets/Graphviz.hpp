#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QRectF>
#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QPainterPath>

#include <gvc.h>
#include <cgraph.h>

/*
 * Originally from
 * http://www.mupuf.org/blog/2010/07/08/how_to_use_graphviz_to_draw_graphs_in_a_qt_graphics_scene/
 */





struct GVNode
{
    /// The unique identifier of the node in the graph
    QString name;

    /// The position of the center point of the node from the top-left corner
    QPoint centerPos;

    /// The size of the node in pixels
    double height, width;
};

/// A struct containing the information for a GVGraph's edge
struct GVEdge
{
    /// The source and target nodes of the edge
    QString source;
    QString target;

    /// Path of the edge's line
    QPainterPath path;
};

class GVGraph
{
public:
    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    GVGraph(QString name, QFont font=QFont(), qreal node_size=50);
    ~GVGraph();

    /// Add and remove nodes
    void addNode(const QString &name);
    void addNodes(const QStringList &names);
    void removeNode(const QString &name);
    void clearNodes();

    /// Add and remove edges
    void addEdge(const QString &source, const QString &target);
    void removeEdge(const QString &source, const QString &target);
    void removeEdge(const QPair<QString, QString> &key);

    /// Set the font to use in all the labels
    void setFont(QFont font);
    
    void setRootNode (const QString &name);
    
    void applyLayout ();
    QRectF boundingRect () const;
    
    QList<GVEdge> edges() const;
    QList<GVNode> nodes () const;

protected:
    Agraph_t* agopen (QString name, Agdesc_t desc);
    QString   agget (void *object, QString attr, QString alt=QString()) const;
    int       agset (void *object, QString attr, QString value);
    Agnode_t *agnode (Agraph_t *graph, QString const& name);

    double    resolution () const;
    
private:
    GVC_t *_context;
    Agraph_t *_graph;
    QFont _font;
    QMap<QString, Agnode_t*> _nodes;
    QMap<QPair<QString, QString>, Agedge_t*> _edges;
};
