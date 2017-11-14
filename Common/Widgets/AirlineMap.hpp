#pragma once

#include <QtCore/QMultiMap>
#include <QtCore/QSize>
#include <QtCore/QSharedPointer>
#include <QtGui/QOpenGLTexture>
#include <QtGui/QVector2D>

#include <Common/Classification/Airport.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/Object.hpp>

#include "OpenGLWidget.hpp"

struct GlobalDistributionWidget : OpenGLWidget
{ 
  typedef Classification::Definitions::AirportPopulation AirportPopulation;
  typedef QList<Classification::ObjectPointer> ObjectList;
  
  GlobalDistributionWidget (Classification::DefinitionsPointer, QWidget * =Q_NULLPTR);
  virtual ~GlobalDistributionWidget ();
  
  void setSelectedObjects (ObjectList);
  void setMaxZoomDistanceToEarth (Classification::Airport::DistanceInKM);
  
protected:
  void generateSphere (int, int);
  void generateLabels ();
  
  virtual void initializeGL () Q_DECL_OVERRIDE;
  
  virtual double velocity (double) const Q_DECL_OVERRIDE;
  virtual double zoom () const Q_DECL_OVERRIDE;
  virtual void   zoom (bool) Q_DECL_OVERRIDE;
  virtual void draw () Q_DECL_OVERRIDE;
  
  QSharedPointer<QOpenGLTexture> m_earth;
  
  double m_zoom;
  double m_zoom_min;
  double m_radius;
  
protected:
  /*
   * TODO
   *   deduplicate
   */
  struct Vertex
  {
    Vertex ();
    Vertex (const QVector3D &, const QVector2D &);
    
    QVector3D coord;
    QVector2D tex;
  };
  QVarLengthArray<Vertex> m_vertices;
  QVarLengthArray<int>    m_indices;
  
protected:
  struct TextChunk
  {
    QString    text;
    int        x, y;
    int        w, h, d;
    bool       highlight;
    ObjectList objects;
    
    TextChunk ();
    TextChunk (QString,    int &, int &, const QFontMetrics &, int);
    TextChunk (ObjectList, int &, int &, const QFontMetrics &, int);
    
    void init (QString, int &, int &, const QFontMetrics &, int);
  };
  
  struct AirportLabel
  {
    AirportLabel (Classification::AirportPointer, ObjectList);
    
    QSharedPointer<QOpenGLTexture> texture();
    void setSelected (ObjectList);
    
    void draw (QMatrix4x4, QVector3D, int, float, float, float);
    void rised (QVector4D *, QMatrix4x4, QVector3D, float, int);
    void ground (QVector4D *, float);
    
    QVector2D m_texcoords[4];
    
    int    m_width, m_height;
    int    m_margin;
    
    QSharedPointer<QImage> m_image;
    QSharedPointer<QOpenGLTexture> m_texture;
    Classification::AirportPointer m_airport;
    
    QList<TextChunk> m_chunks;
  };
  typedef QList<AirportLabel> AirportLabels;
  
  AirportLabels m_airport_labels;
  Classification::DefinitionsPointer m_definitions;
};

