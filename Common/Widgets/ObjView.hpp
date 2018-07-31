#pragma once

#include <QtGui/QOpenGLTexture>

#include <Common/Classification/Object.hpp>

#include "OpenGLWidget.hpp"
#include "VisualModel.hpp"

struct ObjView : OpenGLWidget
{
  ObjView (QWidget * = Q_NULLPTR);
  virtual ~ObjView ();
  
  void setLod (int);
  void setModel (Classification::ObjectPointer, QSharedPointer<VisualModel>);
  void setWireframe (bool);
  
protected:
  QMatrix4x4 modelView(bool object) const;
  void drawAxis ();
  void drawGrid ();
  void drawObject ();
  
  virtual double zoom () const Q_DECL_OVERRIDE;
  virtual void   zoom (bool) Q_DECL_OVERRIDE;
  virtual void   draw () Q_DECL_OVERRIDE;
  
  QSharedPointer<VisualModel>    m_mdl;
  OpenGLTexturePointer           m_draped, m_lit, m_normal, m_texture;
  
  Classification::ObjectPointer  m_object;
  
  int  zoom_level;
  int  m_lod;
  bool m_wireframe;
};
