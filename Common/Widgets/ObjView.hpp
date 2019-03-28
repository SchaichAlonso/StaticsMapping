#pragma once

#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>

#include <Common/Classification/Object.hpp>

#include "OpenGL/Scene.hpp"
#include "OpenGL/Shader.hpp"
#include "OpenGL/Model.hpp"

#include "OpenGLWidget.hpp"
#include "VisualModel.hpp"

namespace Widgets
{
  struct ObjView : OpenGLWidget
  {
    ObjView (QWidget * = Q_NULLPTR);
    virtual ~ObjView ();
    
    void setLod (int);
    void setModel (Classification::ObjectPointer, QSharedPointer<VisualModel>);
    void setWireframe (bool);
    
  protected:
    OpenGL::ScenePointer scene() const;
    OpenGL::ModelPointer axis() const;
    OpenGL::ModelPointer ground() const;
    
    QMatrix4x4 modelView(bool object) const;
    
    virtual double zoom () const Q_DECL_OVERRIDE;
    virtual void   zoom (bool) Q_DECL_OVERRIDE;
    virtual void   draw () Q_DECL_OVERRIDE;
    
    OpenGL::ScenePointer m_scene;
    OpenGL::CameraPointer m_camera;
    
    QSharedPointer<VisualModel>    m_mdl;
    
    Classification::ObjectPointer  m_object;
    
    int  zoom_level;
    int  m_lod;
    bool m_wireframe;
  };
}
