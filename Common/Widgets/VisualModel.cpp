#include <QtCore/QDir>

#include <Common/Obj8/Command/Geometry/LightNamed.hpp>
#include <Common/Obj8/Command/Geometry/Lights.hpp>
#include <Common/Obj8/Command/Geometry/Lines.hpp>
#include <Common/Obj8/Command/Geometry/Triangles.hpp>
#include <Common/Obj8/Command/State/Draped.hpp>
#include <Common/Obj8/Command/State/Lod.hpp>
#include <Common/Obj8/Command/State/NoDraped.hpp>
#include <Common/Obj8/Data/Index10.hpp>
#include <Common/Obj8/Data/LineVertex.hpp>
#include <Common/Obj8/Data/LightVertex.hpp>
#include <Common/Obj8/Data/Vertex.hpp>
#include <Common/Obj8/Global/PointsCounts.hpp>
#include <Common/Obj8/Global/Texture.hpp>
#include <Common/Obj8/Global/TextureDraped.hpp>
#include <Common/Obj8/Global/TextureLit.hpp>
#include <Common/Obj8/Global/TextureNormal.hpp>

#include "VisualModel.hpp"

#include "OpenGL/DrawElementsObj8.hpp"
#include "OpenGL/Mesh.hpp"
#include "OpenGL/Obj8Shader.hpp"

#include <QtPlugin>
Q_IMPORT_PLUGIN (QDDSPlugin);

namespace Widgets
{
  VisualModel::State::State()
  : draped(false)
  , lod_near(0)
  , lod_far(0)
  {
  }
  
  
  
  VisualModel::Vertex::Vertex()
  {
    coordinates[0] = coordinates[1] = coordinates[2] = 0.0;
    color[0] = color[1] = color[2] = 1.0;
    normal[0] = normal[1] = normal[2] = 0.0;
    texcoord[0] = texcoord[1] = 0.0;
  }
  
  
  
  VisualModel::Light::Light()
  : coordinates()
  , color()
  , enabled(false)
  {
  }
  
  
  
  VisualModel::Light::Light(QVector3D coord, QVector3D color)
  : coordinates(coord)
  , color(color)
  , enabled(true)
  {
  }
  
  
  
  VisualModel::VisualModel(QString filename)
    : AbstractVisitor(filename)
    , m_symbol_table()
    , m_current_state()
    , m_mesh(new OpenGL::Mesh())
    , m_model(new OpenGL::Model(m_mesh))
    , m_draped()
    , m_lit()
    , m_normal()
    , m_texture()
    , m_vertices()
    , m_indices()
    , m_groups()
    , m_lights()
    , m_vertex_count(-1)
    , m_line_vertex_count(-1)
    , m_light_count(-1)
    , m_index_count(-1)
    , m_vertex_index(-1)
    , m_line_vertex_index(-1)
    , m_light_index(-1)
    , m_index_index(-1)
  {
    setSatisfied("GLOBAL_LIGHTING", false);
    setSatisfied("GLOBAL_SHADOWS", false);
    setSatisfied("VERSION10", true);
    
    /*
     * x-plane 11.10
     */
    setSatisfied("SCENERY_SHADOWS", true);
  }
  
  
  
  VisualModel::~VisualModel()
  {
  }
  
  
  
  void
  VisualModel::reset()
  {
    m_draped = m_lit = m_normal = m_texture = QImage();
    m_current_state = State();
    
    m_vertices.clear();
    m_indices.clear();
    m_groups.clear();
    m_lights.clear();
    
    m_vertex_count = m_line_vertex_count = m_light_count = m_index_count = -1;
    m_vertex_index = m_line_vertex_index = m_light_index = m_index_index = -1;
  }
  
  
  
  bool
  VisualModel::satisfied(Obj8::StringRef symbol)
  {
    SymbolTable::ConstIterator i, e;
    bool retval;
    
    i = m_symbol_table.constFind(symbol);
    e = m_symbol_table.constEnd();
    
    if (i != e) {
      retval = *i;
    } else {
      qCritical ("Unkown symbolic name queried: %s", qUtf8Printable(symbol));
      retval = false;
    }
    
    return (retval);
  }
  
  
  
  void
  VisualModel::setSatisfied(Obj8::String symbol, bool value)
  {
    m_symbol_table[symbol] = value;
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::Geometry::LightNamed *t)
  {
    /*
     * https://github.com/der-On/XPlane2Blender/blob/master/io_xplane2blender/resources/lights.txt
     */
    QMap<QString,QVector3D> known;
    known["airplane_nav_tail_static_h"]  = QVector3D(1.00f, 1.00f, 0.80f);
    known["airplane_nav_tail_static"]    = QVector3D(1.00f, 1.00f, 0.80f);
    known["airplane_nav_left_static_h"]  = QVector3D(0.90f, 0.10f, 0.00f);
    known["airplane_nav_left_static"]    = QVector3D(0.90f, 0.10f, 0.00f);
    known["airplane_nav_right_static_h"] = QVector3D(0.02f, 0.74f, 0.36f);
    known["airplane_nav_right_static"]   = QVector3D(0.02f, 0.74f, 0.36f);
    
    m_model->addLight(
      OpenGL::LightPointer(
        new OpenGL::Light(
          QVector3D(
            t->m_x.toDouble(),
            t->m_y.toDouble(),
            t->m_z.toDouble()
          ),
          known.value(t->m_name.value(), QVector3D(1,1,1))
        )
      )
    );
    
    m_lights.append(
      Light(
        QVector3D(
          t->m_x.toDouble(),
          t->m_y.toDouble(),
          t->m_z.toDouble()
        ),
        known.value(t->m_name.value(), QVector3D(1,1,1))
      )
    );
  }
  
  
  void
  VisualModel::visit(Obj8::Command::Geometry::Lights *t)
  {
    if (t->offset() + t->count() > m_light_count) {
      Q_ASSERT(false);
    }
    
    for(int i=0; i<t->count(); ++i) {
      m_lights[t->offset() + i].enabled = true;
    }
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::Geometry::Lines *t)
  {
    addGeometry(t->count(), t->offset(), true);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::Geometry::Triangles *t)
  {
    addGeometry(t->count(), t->offset(), false);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::State::Draped *d)
  {
    m_current_state.draped = true;
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::State::NoDraped *l)
  {
    m_current_state.draped = false;
  }
  
  
  
  void
  VisualModel::visit(Obj8::Command::State::Lod *l)
  {
    int n, f;
    
    n = l->nearDistance();
    f = l->farDistance();
    
    if (n != m_current_state.lod_far) {
      Q_ASSERT(true);
    }
    if (n >= f) {
      Q_ASSERT(true);
    }
    
    m_current_state.lod_near = n;
    m_current_state.lod_far = f;
  }
  
  
  
  void
  VisualModel::visit(Obj8::Data::Index *i)
  {
    visit((Obj8::Data::Index10*)i);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Data::Index10 *i)
  {
    Q_FOREACH (Obj8::Parameter::Integer const& n, i->indices()) {
      if (++m_index_index > m_index_count)
        Q_ASSERT (false);
      
      m_indices.append(n.toInt());
    }
  }
  
  
  
  void
  VisualModel::visit(Obj8::Data::LightVertex *v)
  {
    if (++m_light_index > m_light_count)
      Q_ASSERT(false);
    
    Light *l(&m_lights[m_light_index]);
    l->coordinates[0] = v->x.toDouble();
    l->coordinates[1] = v->y.toDouble();
    l->coordinates[2] = v->z.toDouble();
    
    l->color[0] = v->r.toDouble();
    l->color[1] = v->g.toDouble();
    l->color[2] = v->b.toDouble();
  }
  
  
  
  void
  VisualModel::visit(Obj8::Data::LineVertex *v)
  {
    Vertex *vertex;
    
    if (++m_line_vertex_index > m_line_vertex_count)
      Q_ASSERT(false);
    
    vertex = &m_vertices[m_vertex_count + m_line_vertex_index];
    
    vertex->coordinates[0] = v->x.toDouble();
    vertex->coordinates[1] = v->y.toDouble();
    vertex->coordinates[2] = v->z.toDouble();
    
    vertex->color[0] = v->r.toDouble();
    vertex->color[1] = v->g.toDouble();
    vertex->color[2] = v->b.toDouble();
    
    m_mesh->setVertex(
      m_vertex_count + m_line_vertex_index,
      QVector3D(v->x.toDouble(), v->y.toDouble(), v->z.toDouble()),
      QVector4D(v->r.toDouble(), v->g.toDouble(), v->b.toDouble(), 1)
    );
  }
  
  
  
  void
  VisualModel::visit(Obj8::Data::Vertex *v)
  {
    Vertex *vertex;
    
    if (++m_vertex_index > m_vertex_count)
      Q_ASSERT(false);
    
    vertex = &m_vertices[m_vertex_index];
    
    vertex->coordinates[0] = v->x.toDouble();
    vertex->coordinates[1] = v->y.toDouble();
    vertex->coordinates[2] = v->z.toDouble();
    
    vertex->normal[0] = v->nx.toDouble();
    vertex->normal[1] = v->ny.toDouble();
    vertex->normal[2] = v->nz.toDouble();
    
    vertex->texcoord[0] =  v->s.toDouble();
    vertex->texcoord[1] = -v->t.toDouble();
    
    m_mesh->setVertex(
      m_vertex_index,
      QVector3D(v->x.toDouble(), v->y.toDouble(), v->z.toDouble()),
      QVector3D(v->nx.toDouble(), v->ny.toDouble(), v->nz.toDouble()),
      QVector2D(v->s.toDouble(), -v->t.toDouble())
    );
  }
  
  
  
  void
  VisualModel::visit(Obj8::Global::PointsCounts *p)
  {
    if (m_vertex_count != -1)
      Q_ASSERT(false);
    if (m_line_vertex_count != -1)
      Q_ASSERT(false);
    if (m_light_count != -1)
      Q_ASSERT(false);
    if (m_index_count != -1)
      Q_ASSERT(false);
    
    m_vertex_count = p->vertices();
    m_line_vertex_count = p->lineVertices();
    m_light_count = p->lights();
    m_index_count = p->indices();
    
    m_vertices.resize(m_vertex_count + m_line_vertex_count);
    
    m_mesh->setVertexCount(m_vertex_count + m_line_vertex_count);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Global::Texture *t)
  {
    m_texture = loadTexture(t->path ());
    
    m_model->setTexture(0, m_texture);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Global::TextureDraped *t)
  {
    m_draped = loadTexture(t->path());
    
    m_model->setTexture(1, m_draped);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Global::TextureLit *t)
  {
    m_lit = loadTexture(t->path());
    
    m_model->setTexture(2, m_lit);
  }
  
  
  
  void
  VisualModel::visit(Obj8::Global::TextureNormal *t)
  {
    m_normal = loadTexture(t->path());
    
    m_model->setTexture(3, m_normal);
  }
  
  
  
  void
  VisualModel::addGeometry(int count, int offset, bool is_line)
  {
    GeometryGroup g;
    int i, e, array_size;
    
    array_size = is_line? m_line_vertex_count : m_vertex_count;
    
    e = count + offset;
    if (e > m_index_count) {
      Q_ASSERT(false);
    }
    
    for (i=offset; i!=e; ++i) {
      if (m_indices[i] > array_size) {
        Q_ASSERT(false);
      }
    }
    
    g.state   = m_current_state;
    g.is_line = is_line;
    g.count   = count;
    g.offset  = offset;
    
    m_groups.append (g);
    
    
    
    
    
    
    OpenGL::IndexArray indices;
    for (i=offset; i!=e; ++i) {
      if (m_indices[i] > array_size) {
        throw std::runtime_error(
          QString("Index %1 at position %2 out of bounds")
            .arg(m_indices[i])
            .arg(i)
            .toStdString()
        );
      }
      indices.append(m_indices[i]);
    }
    m_mesh->drawElements(
      new OpenGL::DrawElementsObj8(
        indices,
        is_line,
        OpenGL::Obj8RenderAttributes(
          m_current_state.lod_near,
          m_current_state.lod_far,
          m_current_state.draped
        )
      )
    );
  }
  
  
  
  QImage
  VisualModel::loadTexture(QString path, bool allow_empty) const
  {
    QImage retval;
    QStringList textures;
    
    textures = usableTexturePaths (path);
    
    if (textures.isEmpty()) {
      if (allow_empty) {
        retval = QImage(1, 1, QImage::Format_ARGB32);
        retval.fill(Qt::gray);
      } else {
        qCritical("Texture %s unusable.", qUtf8Printable(path));
      }
    } else {
      retval = QImage(textures.first());
    }
    
    return (retval);
  }
  
  
  int
  VisualModel::maxLod() const
  {
    int retval;
    
    if (m_groups.isEmpty()) {
      retval = 0;
    } else {
      retval  = m_groups.last().state.lod_far;
      retval -= (retval != 0)? 1 : 0;
    }
    
    return (retval);
  }
}
