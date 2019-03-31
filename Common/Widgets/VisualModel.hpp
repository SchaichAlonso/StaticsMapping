#pragma once

#include <QtCore/QMap>
#include <QtCore/QVarLengthArray>

#include <QtGui/QImage>
#include <QtGui/QVector3D>

#include <Common/Obj8/AbstractVisitor.hpp>

#include "OpenGL/Model.hpp"

namespace Widgets
{
  struct ObjView;
  
  struct VisualModel : Obj8::AbstractVisitor
  {
    struct State
    {
      State();
      
      bool draped;
      int  lod_near;
      int  lod_far;
    };
    
    struct Vertex
    {
      Vertex();
      
      double coordinates[3];
      double color[3];
      double normal[3];
      double texcoord[2];
    };
    
    struct Light
    {
      Light();
      Light(QVector3D coords, QVector3D color);
      
      QVector3D coordinates;
      QVector3D color;
      bool      enabled;
    };
    
    struct GeometryGroup
    {
      State state;
      bool  is_line;
      
      int   count;
      int   offset;
    };
    
    VisualModel(QString filename= QString());
    virtual ~VisualModel();
    
    virtual void visit(Obj8::Command::Geometry::LightNamed *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::Geometry::Lights *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::Geometry::Lines *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::Geometry::Triangles *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::State::Draped *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::State::NoDraped *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Command::State::Lod *) Q_DECL_OVERRIDE;
    
    virtual void visit(Obj8::Data::Index *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Data::Index10 *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Data::LightVertex *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Data::LineVertex *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Data::Vertex *) Q_DECL_OVERRIDE;
    
    virtual void visit(Obj8::Global::PointsCounts *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Global::Texture *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Global::TextureDraped *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Global::TextureLit *) Q_DECL_OVERRIDE;
    virtual void visit(Obj8::Global::TextureNormal *) Q_DECL_OVERRIDE;
    
    virtual void reset() Q_DECL_OVERRIDE;
    virtual bool satisfied(Obj8::StringRef) Q_DECL_OVERRIDE;
    void setSatisfied(Obj8::String, bool);
    
    int maxLod() const;
    
  protected:
    QImage loadTexture(QString, bool =false) const;
    void   addGeometry(int, int, bool);
    
  protected:
    typedef QMap<Obj8::String, bool> SymbolTable;
    
    SymbolTable m_symbol_table;
    State       m_current_state;
    
  protected:
    OpenGL::MeshPointer m_mesh;
    OpenGL::ModelPointer m_model;
    
  protected:
    friend ObjView;
    
    QVarLengthArray<Vertex>         m_vertices;
    QVarLengthArray<int>            m_indices;
    QVarLengthArray<GeometryGroup>  m_groups;
    QVarLengthArray<Light>          m_lights;
    
    int m_vertex_count, m_line_vertex_count, m_light_count, m_index_count;
    int m_vertex_index, m_line_vertex_index, m_light_index, m_index_index;
  };
}
