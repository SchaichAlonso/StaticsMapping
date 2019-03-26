#include <QtCore/QtMath>

#include <Common/DataPath.hpp>

#include "OpenGL/Mesh.hpp"
#include "OpenGL/Model.hpp"
#include "OpenGL/Object.hpp"
#include "OpenGL/Obj8Shader.hpp"
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


namespace Widgets
{
  ObjView::ObjView(QWidget *parent)
  : OpenGLWidget(parent)
  , m_scene(scene())
  , zoom_level(sizeof(zoom_levels)/sizeof(*zoom_levels)/2)
  , m_lod(0)
  , m_wireframe(false)
  {
  }
  
  
  
  ObjView::~ObjView()
  {
  }
  
  
  void
  ObjView::setLod(int lod)
  {
    m_lod = lod;
    update ();
  }
  
  
  
  void
  ObjView::setModel(
    Classification::ObjectPointer object,
    QSharedPointer<VisualModel> model
  )
  {
    if (m_mdl) {
      m_scene->removeModel(m_mdl->m_model);
    }
    
    m_object = object;
    m_mdl    = model;
    
    if (m_mdl) {
      
      for (int i=0; i!=m_mdl->m_lights.size(); ++i) {
        OpenGL::LightPointer l(
          new OpenGL::Light(
            m_mdl->m_lights[i].coordinates,
            m_mdl->m_lights[i].color
          )
        );
        
        if (m_mdl->m_lights[i].enabled) {
          m_scene->addLight(l);
        }
      }
      
      /*
      for (int i=0; i<360; i+=30) {
        OpenGL::ModelPointer clone(new OpenGL::Model(*m_mdl->m_model.data()));
        clone->move(50 * QVector3D(qSin(3.14 * i / 180), 0, qCos(3.14 * i / 180)));
        clone->setOrientation(0, 0, i);
        m_scene->addModel(clone);
      }*/
      
      m_scene->addModel(m_mdl->m_model);
      
    }
  }
  
  
  
  void
  ObjView::setWireframe(bool wireframe)
  {
    m_wireframe = wireframe;
  }
  
  
  OpenGL::ScenePointer
  ObjView::scene() const
  {
    OpenGL::ScenePointer scene{
      new OpenGL::Scene{
        OpenGL::ShaderPointer{
          new OpenGL::Shader{
            DataPath::existingPath("obj8-vert.glsl"),
            DataPath::existingPath("obj8-frag.glsl")
          }
        }
      }
    };
    scene->addModel(axis());
    scene->addModel(ground());
    return (scene);
  }
  
  
  OpenGL::ModelPointer
  ObjView::axis() const
  {
    OpenGL::MeshPointer axis(new OpenGL::Mesh);
    
    QVector3D midpoint(0,0,0);
    QVector3D vaxis[] = {
      QVector3D(1, 0, 0),
      QVector3D(0, 1, 0),
      QVector3D(0, 0, 1)
    };
    
    axis->drawElements(
      new OpenGL::DrawElements(
        QVector<int>()
          << axis->addVertex(vaxis[0], vaxis[0])
          << axis->addVertex(midpoint, vaxis[0])
          << axis->addVertex(vaxis[1], vaxis[1])
          << axis->addVertex(midpoint, vaxis[1])
          << axis->addVertex(vaxis[2], vaxis[2])
          << axis->addVertex(midpoint, vaxis[2]),
        GL_LINES
      )
    );
    
    return OpenGL::ModelPointer{
      new OpenGL::Model{
        
        axis,
        OpenGL::Model::DepthMasked
      }
    };
  }
  
  
  OpenGL::ModelPointer
  ObjView::ground() const
  {
    OpenGL::MeshPointer ground(new OpenGL::Mesh);
    
    QVector4D gray(0.5f,0.5f,0.5f,1.0f);
    ground->drawElements(
      new OpenGL::DrawElements(
        QVector<int>()
          << ground->addVertex(QVector3D(-256, 0, -256), gray)
          << ground->addVertex(QVector3D(-256, 0,  256), gray)
          << ground->addVertex(QVector3D( 256, 0,  256), gray)
          << ground->addVertex(QVector3D(-256, 0, -256), gray)
          << ground->addVertex(QVector3D( 256, 0,  256), gray)
          << ground->addVertex(QVector3D( 256, 0, -256), gray),
        GL_TRIANGLES
      )
    );
    
    OpenGL::IndexArray indices;
    for(int line=0, lines=512, offset=lines/2; line!=lines; ++line) {
      int alpha;
      alpha = ((line - offset) % 10) != 0 ? 24 : 64;
      alpha = (line != offset)? alpha : 192;
      
      QVector4D color(1, 1, 1, alpha/255.0f);
      
      indices << ground->addVertex(QVector3D(line - offset, 0.1f, -offset), color);
      indices << ground->addVertex(QVector3D(line - offset, 0.1f,  offset), color);
      
      indices << ground->addVertex(QVector3D(-offset, 0.1f, line - offset), color);
      indices << ground->addVertex(QVector3D( offset, 0.1f, line - offset), color);
    }
    ground->drawElements(
      new OpenGL::DrawElements(indices, GL_LINES)
    );
    
    return (OpenGL::ModelPointer(new OpenGL::Model(ground, OpenGL::Model::Lighting)));
  }
  
  
  
  void
  ObjView::zoom(bool in)
  {
    int max = (int)(sizeof(zoom_levels)/sizeof(*zoom_levels));
    
    zoom_level += in? -1 : 1;
    
    zoom_level += (zoom_level == -1)?  1 : 0;
    zoom_level -= (zoom_level == max)? 1 : 0;
    
    Q_ASSERT (zoom_level >= 0);
    Q_ASSERT (zoom_level < max);
    
    m_scene->camera()->zoom(in);
  }
  
  
  
  double
  ObjView::zoom() const
  {
    return (zoom_levels[zoom_level]);
  }
  
  
  
  
  QMatrix4x4
  ObjView::modelView(bool object) const
  {
    QMatrix4x4 retval;
    
    retval.setToIdentity();
    
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
        
        
        
        QVector3D locale(
           -m_object->translate(0).toDouble(),
           -m_object->translate(1).toDouble(),
           -m_object->translate(2).toDouble()
        );
        retval.translate(locale);
      }
    }
    
    return (retval);
  }
  
  
  
  void
  ObjView::draw()
  {
    OpenGL::CameraPointer cam(m_scene->camera());
    cam->setPitch(m_pitch);
    cam->setYaw(m_yaw);
    cam->setPosition(QVector3D(0,0,0));
    //cam->setPosition(cam->zoom() * sphericToCarthesian(m_pitch, m_yaw));
    cam->setPosition(QVector3D(0, 0, cam->zoom()));
    cam->setScreenDimensions(size());
    
    QMatrix4x4 model(m_modelview * modelView(true));
    
    if (m_wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    m_scene->draw();
    
    if (m_wireframe) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
  }
}
