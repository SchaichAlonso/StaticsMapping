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

#include "Obj8Visitor.hpp"

#include "DrawElementsObj8.hpp"
#include "Mesh.hpp"
#include "Obj8NamedLightFactory.hpp"
#include "Obj8Shader.hpp"

#include <QtPlugin>
Q_IMPORT_PLUGIN (QDDSPlugin);

namespace OpenGL
{
  Obj8Visitor::State::State()
  : draped(false)
  , lod_near(0)
  , lod_far(0)
  {
  }
  
  
  
  Obj8Visitor::Light::Light()
  : coordinates()
  , color()
  , enabled(false)
  {
  }
  
  
  
  Obj8Visitor::Light::Light(QVector3D coord, QVector3D color)
  : coordinates(coord)
  , color(color)
  , enabled(true)
  {
  }
  
  
  
  Obj8Visitor::Obj8Visitor(ModelPointer output, QString filename)
    : AbstractVisitor(filename)
    , m_symbol_table()
    , m_current_state()
    , m_light_factory()
    , m_model(output)
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
    , m_load_textures(filename.size() != 0)
  {
    setSatisfied("GLOBAL_LIGHTING", false);
    setSatisfied("GLOBAL_SHADOWS", false);
    setSatisfied("VERSION10", true);
    
    /*
     * x-plane 11.10
     */
    setSatisfied("SCENERY_SHADOWS", true);
  }
  
  
  
  Obj8Visitor::Obj8Visitor(ModelPointer reload)
  : Obj8Visitor(reload, QString())
  {
    Q_FOREACH(LightPointer light, reload->allLights()) {
      reload->removeLight(light);
    }
    reload->setMesh(MeshPointer(new Mesh()));
  }
  
  
  
  Obj8Visitor::~Obj8Visitor()
  {
  }
  
  
  
  void
  Obj8Visitor::reset()
  {
    m_current_state = State();
        
    m_indices.clear();
    m_groups.clear();
    m_lights.clear();
    
    m_vertex_count = m_line_vertex_count = m_light_count = m_index_count = -1;
    m_vertex_index = m_line_vertex_index = m_light_index = m_index_index = -1;
  }
  
  
  
  bool
  Obj8Visitor::satisfied(Obj8::StringRef symbol)
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
  Obj8Visitor::setSatisfied(Obj8::String symbol, bool value)
  {
    m_symbol_table[symbol] = value;
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::Geometry::LightNamed *t)
  {
    QVector3D position(t->m_x.toFloat(), t->m_y.toFloat(), t->m_z.toFloat());
    LightPointer light(m_light_factory.create(position, t->m_name.value()));
    if (light) {
      m_model->addLight(light);
    } else {
      qInfo("<%s> is not a known light name.", qUtf8Printable(t->m_name.value()));
    }
    
    m_lights.append(
      Light(
        QVector3D(
          t->m_x.toFloat(),
          t->m_y.toFloat(),
          t->m_z.toFloat()
        ),
        QVector3D(1,1,1)
      )
    );
  }
  
  
  void
  Obj8Visitor::visit(Obj8::Command::Geometry::Lights *t)
  {
    if (t->offset() + t->count() > m_light_count) {
      Q_ASSERT(false);
    }
    
    for(int i=0; i<t->count(); ++i) {
      m_lights[t->offset() + i].enabled = true;
    }
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::Geometry::Lines *t)
  {
    addGeometry(t->count(), t->offset(), true);
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::Geometry::Triangles *t)
  {
    addGeometry(t->count(), t->offset(), false);
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::State::Draped *d)
  {
    Q_UNUSED(d);
    m_current_state.draped = true;
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::State::NoDraped *l)
  {
    Q_UNUSED(l);
    m_current_state.draped = false;
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Command::State::Lod *l)
  {
    int n, f;
    
    n = l->nearDistance();
    f = l->farDistance();
    
    Q_ASSERT(n == m_current_state.lod_far);
    Q_ASSERT(n < f);
    
    m_current_state.lod_near = n;
    m_current_state.lod_far = f;
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Data::Index *i)
  {
    visit((Obj8::Data::Index10*)i);
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Data::Index10 *i)
  {
    Q_FOREACH (Obj8::Parameter::Integer const& n, i->indices()) {
      ++m_index_index;
      Q_ASSERT(m_index_index <= m_index_count);
      
      m_indices.append(n.toInt());
    }
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Data::LightVertex *v)
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
  Obj8Visitor::visit(Obj8::Data::LineVertex *v)
  {
    ++m_line_vertex_count;
    Q_ASSERT(m_line_vertex_index <= m_line_vertex_count);
    
    m_model->mesh()->setVertex(
      m_vertex_count + m_line_vertex_index,
      QVector3D(v->x.toDouble(), v->y.toDouble(), v->z.toDouble()),
      QVector4D(v->r.toDouble(), v->g.toDouble(), v->b.toDouble(), 1)
    );
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Data::Vertex *v)
  {
    ++m_vertex_index;
    Q_ASSERT(m_vertex_index <= m_vertex_count);
    
    m_model->mesh()->setVertex(
      m_vertex_index,
      QVector3D(v->x.toDouble(), v->y.toDouble(), v->z.toDouble()),
      QVector3D(v->nx.toDouble(), v->ny.toDouble(), v->nz.toDouble()),
      QVector2D(v->s.toDouble(), -v->t.toDouble())
    );
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Global::PointsCounts *p)
  {
    Q_ASSERT(m_vertex_count == -1);
    Q_ASSERT(m_line_vertex_count == -1);
    Q_ASSERT(m_light_count == -1);
    Q_ASSERT(m_index_count == -1);
    
    m_vertex_count = p->vertices();
    m_line_vertex_count = p->lineVertices();
    m_light_count = p->lights();
    m_index_count = p->indices();
    
    m_model->mesh()->setVertexCount(m_vertex_count + m_line_vertex_count);
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Global::Texture *t)
  {
    if (m_load_textures) {
      m_model->setTexture(0, loadTexture(t->path()));
    }
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Global::TextureDraped *t)
  {
    if (m_load_textures) {
      m_model->setTexture(1, loadTexture(t->path()));
    }
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Global::TextureLit *t)
  {
    if (m_load_textures) {
      m_model->setTexture(2, loadTexture(t->path()));
    }
  }
  
  
  
  void
  Obj8Visitor::visit(Obj8::Global::TextureNormal *t)
  {
    if (m_load_textures) {
      m_model->setTexture(3, loadTexture(t->path()));
    }
  }
  
  
  
  void
  Obj8Visitor::addGeometry(int count, int offset, bool is_line)
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
    
    m_groups.append(g);
    
    
    
    
    
    
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
    m_model->mesh()->drawElements(
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
  Obj8Visitor::loadTexture(QString path, bool allow_empty) const
  {
    QImage retval;
    QStringList textures;
    
    textures = usableTexturePaths(path);
    
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
  Obj8Visitor::maxLod() const
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
