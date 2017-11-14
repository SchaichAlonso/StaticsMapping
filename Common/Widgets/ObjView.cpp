#include <QtMath>
#include <QDir>
#include <QMatrix4x4>
#include <QOpenGLTexture>
#include <QWheelEvent>

#include "ObjView.hpp"


float zoom_levels[] {
  1,
  2,
  3,
  5,
  8,
  13,
  21,
  34,
  55,
  89,
  144
};


ObjView::ObjView (QWidget *parent)
  : OpenGLWidget (parent)
  , zoom_level (sizeof(zoom_levels)/sizeof(*zoom_levels)/2)
  , m_lod(0)
  , m_wireframe (false)
{
}



ObjView::~ObjView ()
{
}


void
ObjView::setLod (int lod)
{
  m_lod = lod;
  update ();
}



void
ObjView::setModel (Classification::ObjectPointer object, QSharedPointer<VisualModel> model)
{
  m_object = object;
  m_mdl    = model;
  
  if (m_mdl) {
    
    QOpenGLTexture::Filter min = QOpenGLTexture::LinearMipMapLinear;
    QOpenGLTexture::Filter mag = QOpenGLTexture::Linear;
    
    if (m_mdl->m_draped.isNull()) {
      m_draped.reset ();
    } else {
      m_draped.reset (new QOpenGLTexture (m_mdl->m_draped));
      m_draped->setMinMagFilters (min, mag);
    }
    
    if (m_mdl->m_lit.isNull()) {
      m_lit.reset ();
    } else {
      m_lit.reset (new QOpenGLTexture (m_mdl->m_lit));
      m_lit->setMinMagFilters (min, mag);
    }
    
    if (m_mdl->m_normal.isNull()) {
      m_normal.reset ();
    } else {
      m_normal.reset (new QOpenGLTexture (m_mdl->m_normal));
      m_normal->setMinMagFilters (min, mag);
    }
    
    if (m_mdl->m_texture.isNull()) {
      m_texture.reset ();
    } else {
      m_texture.reset (new QOpenGLTexture (m_mdl->m_texture));
      m_texture->setMinMagFilters (min, mag);
    }
  } else {
    m_draped.reset ();
    m_lit.reset ();
    m_normal.reset ();
    m_texture.reset ();
  }
}



void
ObjView::setWireframe (bool wireframe)
{
  m_wireframe = wireframe;
}



void
ObjView::zoom (bool in)
{
  int max = (int)(sizeof(zoom_levels)/sizeof(*zoom_levels));
  
  zoom_level += in? -1 : 1;
  
  zoom_level += (zoom_level == -1)?  1 : 0;
  zoom_level -= (zoom_level == max)? 1 : 0;
  
  Q_ASSERT (zoom_level >= 0);
  Q_ASSERT (zoom_level < max);
}



double
ObjView::zoom () const
{
  return (zoom_levels[zoom_level]);
}




QMatrix4x4
ObjView::modelView (bool object) const
{
  QMatrix4x4 retval;
  
  retval.setToIdentity ();
  
  if (object) {
    if (m_object) {
      float rotation;
      switch (m_object->rotate()) {
        case Classification::Object::RotateNull:
          rotation = 0;
          break;
        case Classification::Object::RotateCCW90:
          rotation = 90;
          break;
        case Classification::Object::RotateCCW180:
          rotation = 180;
          break;
        case Classification::Object::RotateCCW270:
          rotation = 270;
          break;
      }
      retval.rotate (rotation, 0, 1, 0);
      
      
      
      QVector3D locale = QVector3D (
        -m_object->translate(0).toDouble(),
        -m_object->translate(1).toDouble(),
        -m_object->translate(2).toDouble()
      );
      retval.translate (locale);
    }
  }
  
  return (retval);
}



void
ObjView::drawAxis ()
{
  GLubyte axis[3][3] = {
    { 255,   0,   0 },
    {   0, 255,   0 },
    {   0,   0, 255 }
  };
  
  QVector3D locale(0,0,0);
  glBegin (GL_LINES);
    for (int i=0; i!=3; ++i) {
      glColor3ubv (axis[i]);
      glVertex3f (
        locale[0] + axis[i][0],
        locale[1] + axis[i][1],
        locale[2] + axis[i][2]);
      glVertex3fv (&locale[0]);
    }
  glEnd ();
}



void
ObjView::drawGrid ()
{
  glDisable (GL_DEPTH_TEST);
  glBegin (GL_LINES);
  for (int line=0, lines=512, offset=lines/2; line!=lines; ++line) {
      
    int alpha;
    alpha = ((line - offset) % 10) != 0 ? 24 : 64;
    alpha = (line != offset)? alpha : 192;
      
    glColor4ub (255, 255, 255, alpha);
      
    glVertex3i (line - offset, 0, -offset);
    glVertex3i (line - offset, 0,  offset);
      
    glVertex3i (-offset, 0, line - offset);
    glVertex3i ( offset, 0, line - offset);
  }
  glEnd ();
  glEnable (GL_DEPTH_TEST);
}



void
ObjView::drawObject ()
{
  glEnableClientState (GL_VERTEX_ARRAY);
  glEnableClientState (GL_COLOR_ARRAY);
  glEnableClientState (GL_NORMAL_ARRAY);
  glEnableClientState (GL_TEXTURE_COORD_ARRAY);
    
  size_t stride = sizeof(VisualModel::Vertex);
  VisualModel::Vertex *v = &m_mdl->m_vertices[0];
    
  glVertexPointer (3, GL_DOUBLE, stride, v->coordinates);
  glColorPointer(3, GL_DOUBLE, stride, v->color);
  glNormalPointer (GL_DOUBLE, stride, v->normal);
  glTexCoordPointer (2, GL_DOUBLE, stride, v->texcoord);
    
  QSharedPointer<QOpenGLTexture> tex_bound, tex_required;
    
  Q_FOREACH (const VisualModel::GeometryGroup &g, m_mdl->m_groups) {
      
    tex_required = g.state.draped? m_draped : m_texture;
    if (tex_required != tex_bound) {
      if (tex_required) {
        tex_required->bind ();
      }
      tex_bound = tex_required;
    }
      
    if (g.state.lod_near != g.state.lod_far) {
      if ((m_lod < g.state.lod_near) or (g.state.lod_far <= m_lod)) {
        continue;
      }
    }
      
    GLenum mode = g.is_line ? GL_LINES : GL_TRIANGLES;
      
    if (g.is_line or m_wireframe) {
      glDisable (GL_TEXTURE_2D);
      glDisable (GL_LIGHTING);
    } else {
      glEnable (GL_TEXTURE_2D);
      glEnable (GL_LIGHTING);
    }
      
    glDrawElements (mode, g.count, GL_UNSIGNED_INT, &m_mdl->m_indices[g.offset]);
  }
  
  glDisable (GL_TEXTURE_2D);
  
  glDisableClientState (GL_VERTEX_ARRAY);
  glDisableClientState (GL_COLOR_ARRAY);
  glDisableClientState (GL_NORMAL_ARRAY);
  glDisableClientState (GL_TEXTURE_COORD_ARRAY);
}



void
ObjView::draw ()
{
  glPushMatrix ();
  glMultMatrixf (modelView(false).constData());
  glDisable (GL_DEPTH_TEST);
  if (not m_ortho) {
    drawGrid();
  }
  drawAxis();
  glEnable (GL_DEPTH_TEST);
  glPopMatrix ();
  
  glPushMatrix ();
  glMultMatrixf (modelView(true).constData());
  
  if (m_wireframe) {
    glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    GLfloat ambient[4]={0.2f,0.2f,0.2f,1.0f};
    GLfloat diffuse[4]={0.8f,0.8f,0.8f,1.0f};
    GLfloat specular[4]={1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat mat_diffuse[4]={0.85f,0.85f,0.85f,1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    GLfloat LightPosition[] = {0.0f, 45.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
    glLightf (GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
    glLightf (GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0);
    glLightf (GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glMaterialf(GL_FRONT, GL_SHININESS, 128.0f / 10);
  }
  
  if (m_mdl) {
    drawObject ();
  }
  
  if (m_wireframe) {
    glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
  } else {
    glDisable (GL_LIGHTING);
  }
  glPopMatrix ();
}
