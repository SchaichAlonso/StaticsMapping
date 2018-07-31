#include <QtCore/QtMath>
#include <QtGui/QGuiApplication>
#include <QtGui/QImage>
#include <QtGui/QPainter>

#include <Common/Classification/Airport.hpp>
#include <Common/Classification/Airline.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include "GlobalDistributionWidget.hpp"


GlobalDistributionWidget::Vertex::Vertex ()
  : coord ()
  , tex ()
{
}


GlobalDistributionWidget::Vertex::Vertex (const QVector3D &c, const QVector2D &t)
  : coord (c)
  , tex (t)
{
}


GlobalDistributionWidget::GlobalDistributionWidget (Classification::DefinitionsPointer definitions, QWidget *parent)
  : OpenGLWidget (parent)
  , m_earth(texture(QImage(Classification::Definitions::dataPath("earthmap1k.jpg"))))
  , m_zoom(2)
  , m_zoom_min(1.0)
  , m_radius(1.0)
  , m_vertices(sphereVertices(180, 360, m_radius))
  , m_indices(sphereIndices(180, 360))
  , m_airport_labels()
  , m_definitions(definitions)
{
  setMaxZoomDistanceToEarth (150);
  
  generateLabels ();
  
  setPerspective ();
}



GlobalDistributionWidget::~GlobalDistributionWidget ()
{
  m_earth.reset ();
}



GlobalDistributionWidget::TextChunk::TextChunk ()
  : text ()
  , x (-1)
  , y (-1)
  , w (-1)
  , h (-1)
  , d (-1)
  , highlight (false)
  , objects ()
{
}



GlobalDistributionWidget::TextChunk::TextChunk (QString text, int &x, int &y, const QFontMetrics &fm, int w)
  : text ()
  , x (x)
  , y (y)
  , w ()
  , h (fm.lineSpacing ())
  , d (fm.descent ())
  , highlight (false)
  , objects ()
{
  init (text, x, y, fm, w);
}



GlobalDistributionWidget::TextChunk::TextChunk (ObjectList objects, int &x, int &y, const QFontMetrics &fm, int w)
  : text ()
  , x (x)
  , y (y)
  , w ()
  , h (fm.lineSpacing ())
  , d (fm.descent ())
  , highlight (false)
  , objects (objects)
{
  QString text;
  int n;
  
  n = objects.size ();
  
  Q_ASSERT (n != 0);
  
  
  QString objname = objects.value(0)->name ();
  if (n > 1) {
    text = QString ("%1(%2)").arg(objname).arg(n);
  } else {
    text = objname;
  }
  
  init (text, x, y, fm, w);
}



void
GlobalDistributionWidget::TextChunk::init (QString text, int &x, int &y, const QFontMetrics &fm, int w)
{
  this->text = text;
  this->w    = fm.width (text);
  
  int bbwidth = fm.boundingRect(text).width();
  int feed = this->w + fm.width (QChar(' '));
  
  if (x + bbwidth < w) {
    x += feed;
  } else {
    x = feed;
    this->x = 0;
    this->y = y = y + this->h;
  }
}



GlobalDistributionWidget::AirportLabel::AirportLabel (
    Classification::AirportPointer apt,
    ObjectList objects
)
  : m_texcoords {
       QVector2D(0, 0),
       QVector2D(1, 0),
       QVector2D(0, 1),
       QVector2D(1, 1)
    }
  , m_width (-1)
  , m_height (-1)
  , m_margin (1)
  , m_image ()
  , m_texture ()
  , m_airport (apt)
  , m_chunks ()
{
  QImage pixel (1, 1, QImage::Format_ARGB32);
  QPainter painter(&pixel);
  QFontMetrics fm = painter.fontMetrics();
  
  m_width = 64 + 2 * fm.lineSpacing() * qLn(objects.size());
  
  int x = 0;
  int y = 0;
  m_chunks.append (TextChunk (apt->icao(), x, y, fm, m_width));
  
  /*
   * linebreak.
   */
  y += fm.lineSpacing ();
  x  = 0;
  
  QMultiMap<QString, Classification::ObjectPointer> groups;
  Q_FOREACH (Classification::ObjectPointer object, objects) {
    groups.insert (object->name(), object);
  }
  Q_FOREACH (QString name, groups.uniqueKeys()) {
    m_chunks.append (TextChunk (groups.values(name), x, y, fm, m_width));
  }
  
  m_height = y + fm.lineSpacing ();
}



QSharedPointer<QOpenGLTexture>
GlobalDistributionWidget::AirportLabel::texture ()
{
  if (m_texture.isNull()) {
    int w = 2*m_margin + m_width;
    int h = 2*m_margin + m_height;
    int m = m_margin;
    
    QPalette palette = QGuiApplication::palette ();
    
    QImage surface (w, h, QImage::Format_ARGB32);
    //surface.fill (Qt::transparent);
    
    QPainter painter (&surface);
    painter.fillRect (0, 0, w, h, palette.brush(QPalette::Shadow));
    painter.fillRect (m, m, w-2*m, h-2*m, palette.brush(QPalette::Base));
    
    Q_FOREACH (const TextChunk &c, m_chunks) {
      if (c.highlight) {
        painter.fillRect (c.x, c.y, c.w, c.h, palette.brush(QPalette::Highlight));
        painter.setBrush (palette.brush(QPalette::HighlightedText));
      } else {
        painter.setBrush (palette.brush(QPalette::Text));
      }
      painter.drawText (c.x+m, c.y+m+c.h-c.d, c.text);
    }
    painter.end ();
    surface.save (m_chunks[0].text + ".png");
    m_texture.reset (new QOpenGLTexture (surface));
  }
  return (m_texture);
}



void
GlobalDistributionWidget::AirportLabel::setSelected (ObjectList selection)
{
  for (QList<TextChunk>::Iterator i=m_chunks.begin(); i!=m_chunks.end(); ++i) {
    bool oldval = i->highlight;
    bool newval = false;
    
    Q_FOREACH (Classification::ObjectPointer obj, i->objects) {
      newval = newval || selection.contains(obj);
    }
    
    i->highlight = newval;
    
    if (newval != oldval) {
      m_texture.reset ();
    }
  }
}



void
GlobalDistributionWidget::setSelectedObjects (ObjectList objects)
{
  for (AirportLabels::Iterator i=m_airport_labels.begin(); i!=m_airport_labels.end(); ++i) {
    i->setSelected (objects);
  }
}



void
GlobalDistributionWidget::setMaxZoomDistanceToEarth (Classification::Airport::DistanceInKM elevation)
{
  double r = Classification::Airport::earthRadiusInKM();
  
  m_zoom_min = (r + qMax(elevation,10)) / r;
}



QVarLengthArray<GlobalDistributionWidget::Vertex>
GlobalDistributionWidget::sphereVertices(int lons, int lats, double radius)
{
  QVarLengthArray<GlobalDistributionWidget::Vertex> retval;
  for (int lat=0; lat!=lats; ++lat) {
    for (int lon=0; lon!=lons; ++lon) {
      double slat = ((double)lat)/(lats-1);
      double slon = ((double)lon)/(lons-1);
      Vertex v;
      v.coord = radius * sphericToCarthesian(180*slat - 90, 360*slon - 180);
      v.tex[0] = slon;
      v.tex[1] = 1.0-slat;
      retval.append(v);
    }
  }
  return (retval);
}

QVarLengthArray<int>
GlobalDistributionWidget::sphereIndices(int lons, int lats)
{
  QVarLengthArray<int> indices;
  for (int lat=0; lat!=(lats-1); ++lat) {
    for (int lon=0; lon!=lons; ++lon) {
      if (lat != lats-2) {
        indices.append((lat+0)*lons + (lon+0)%lons);
        indices.append((lat+1)*lons + (lon+0)%lons);
        indices.append((lat+1)*lons + (lon+1)%lons);
      }
      
      if (lat != 0) {
        indices.append((lat+0)*lons + (lon+0)%lons);
        indices.append((lat+1)*lons + (lon+1)%lons);
        indices.append((lat+0)*lons + (lon+1)%lons);
      }
    }
  }
  return (indices);
}



void
GlobalDistributionWidget::generateLabels ()
{
  m_airport_labels.clear ();
  
  AirportPopulation all = m_definitions->airportPopulation ();
  Q_FOREACH (Classification::Airport::PrimaryKey key, all.keys()) {
    Classification::AirportPointer apt = m_definitions->airport (key);
    ObjectList current = all.values (key);
    m_airport_labels.append (AirportLabel(apt, current));
  }
}



double
GlobalDistributionWidget::velocity (double v) const
{
  double z = zoom ();
  if ((m_zoom_min <= z) and (z <= m_zoom_min+1)) {
    z -= m_zoom_min;
    v *= z;
  }
  return (v);
}



double
GlobalDistributionWidget::zoom () const
{
  return (m_zoom);
}



void
GlobalDistributionWidget::zoom (bool in)
{
  qDebug () << "min=" << m_zoom_min << " z=" << m_zoom;
  
  m_zoom = m_zoom_min + (m_zoom - m_zoom_min) * (in? 0.9f : 1.1f);
  m_zoom = m_zoom < m_zoom_min? m_zoom_min : m_zoom;
}



void
GlobalDistributionWidget::draw ()
{
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
  
  glVertexPointer (3, GL_FLOAT, sizeof(m_vertices[0]), &m_vertices[0].coord[0]);
  glTexCoordPointer (2, GL_FLOAT, sizeof(m_vertices[0]), &m_vertices[0].tex[0]);
  
  glEnable (GL_TEXTURE_2D);
  
  m_earth->bind();
  
  glDrawElements (GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, &m_indices[0]);
  
  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
  
  /*
   * OSD
   */
  
  for (AirportLabels::Iterator i=m_airport_labels.begin(); i!=m_airport_labels.end(); ++i) {
    
    
    float max = (m_zoom_min - m_radius / 2);
    float min = (m_zoom_min - m_radius);
    float label_evelation = m_radius + (m_zoom_min - m_radius) / 2;
    
    i->draw (m_modelview, m_observer, height(), label_evelation, min, max);
  }
  
  glDisable (GL_TEXTURE_2D);
}


void
GlobalDistributionWidget::AirportLabel::rised (QVector4D *v, QMatrix4x4 modelview, QVector3D observer, float elevation, int display_height)
{
  float lat = m_airport->latitude();
  float lon = m_airport->longitude();
  
  QVector3D extruded = elevation * sphericToCarthesian (lat, lon);
  
  QVector3D l, r, u;
  float d;
  
  l = extruded - observer;
  d = l.length ();
  l = l.normalized ();
  u = QVector3D(modelview(1,0), modelview(1,1), modelview(1,2));
  r = QVector3D::crossProduct (u, l);
  u = QVector3D::crossProduct (l, r);
  r = QVector3D::crossProduct (u, l);
  
  QMatrix4x4 m (
      r[0], u[0], l[0], extruded[0],
      r[1], u[1], l[1], extruded[1],
      r[2], u[2], l[2], extruded[2],
         0,    0,    0,           1
  );
  
  float s = 0.5f * d / display_height;
  float x = s * m_width;
  float y = s * m_height;
  
  for (int i=0; i!=4; ++i) {
    v[i] = m * QVector4D (x*(1-2*m_texcoords[i][0]), y*(1-2*m_texcoords[i][1]), 0, 1);
  }
}

void
GlobalDistributionWidget::AirportLabel::ground (QVector4D *v, float rad)
{
  for (int i=0; i!=4; ++i) {
    float lat = m_airport->latitude()  - m_texcoords[i][1] * rad;
    float lon = m_airport->longitude() + m_texcoords[i][0] * rad;
    v[i] = QVector4D (sphericToCarthesian (lat, lon));
  }
}


void
GlobalDistributionWidget::AirportLabel::draw (QMatrix4x4 modelview, QVector3D observer, int display_height, float elevation, float dmin, float dmax)
{
  float rad = 5.0f * 360.0f / 40000.0f;
  
  
  QVector4D extruded[4];
  QVector4D grounded[4];
  
  rised (extruded, modelview, observer, elevation, display_height);
  ground (grounded, rad);
  
  QVector3D mid = sphericToCarthesian (m_airport->latitude(), m_airport->longitude());
  float d = (mid - observer).length();
  float u = (qMin(dmax, qMax(d, dmin)) - dmin) / (dmax - dmin);
  if (m_airport->icao() == "DIAP")
    qDebug() << "d=" << d << " u=" << u;
  
  QVector4D interpolated[4];
  for (int i=0; i!=4; ++i) {
    interpolated[i] = (1-u) * extruded[i] + u * grounded[i];
  }
  
  texture()->bind();

  int indices[] = {
    0, 2, 3,
    0, 3, 1
  };
  
  glBegin (GL_TRIANGLES);
    for (int i=0; i!=sizeof(indices)/sizeof(*indices); ++i) {
      glTexCoord2fv (&m_texcoords[indices[i]][0]);
      glVertex3fv (&interpolated[indices[i]][0]);
    }
  glEnd ();
  
 
  glBegin (GL_LINES);
    ::glDisable(GL_TEXTURE_2D);
    for (int i=0; i!=sizeof(extruded)/sizeof(*extruded); ++i) {
      glVertex3fv(&extruded[i][0]);
      glVertex3fv(&grounded[i][0]);
    }
    ::glEnable(GL_TEXTURE_2D);
  glEnd ();
}
