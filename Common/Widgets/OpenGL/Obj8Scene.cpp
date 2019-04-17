#include <QtCore/QFile>

#include "Obj8Scene.hpp"
#include "Obj8Shader.hpp"
#include "Obj8Visitor.hpp"

namespace OpenGL
{
  Obj8Scene::Obj8Scene(bool add_grid)
  : Scene(ShaderPointer(new Obj8Shader()))
  {
    if (add_grid) {
      insertPositionIndicator(QColor(Qt::red), QColor(Qt::green), QColor(Qt::blue), 5, false);
      ModelPointer squares{grid(QVector3D(0,0,1), QVector3D(1,0,0), 256)};
      ModelPointer ground{plane(QVector3D(0,0,1), QVector3D(1,0,0), 256)};
      
      squares->setFlags(squares->flags() & ~(Model::Texturing | Model::Lighting));
      ground->setFlags(squares->flags() & ~Model::Texturing);
      ground->setPosition(QVector3D(0,-0.1f,0));
      
      insertModel(ground);
      insertModel(squares);
    }
  }
  
  Obj8Scene::~Obj8Scene()
  {
  }
  
  ModelWeakPointer Obj8Scene::insertModel(Obj8::File *file)
  {
    ModelPointer model(new Model());
    QSharedPointer<OpenGL::Obj8Visitor> visitor(new OpenGL::Obj8Visitor(model, file->filename()));
    
    file->accept(visitor.data(), false);
    
    return insertModel(model);
  }
  
  ModelWeakPointer Obj8Scene::insertModel(Obj8::FilePointer file)
  {
    return insertModel(file.get());
  }
  
  ModelWeakPointer Obj8Scene::insertModel(QString filename)
  {
    QFile f(filename);
    
    if (!f.open(QFile::ReadOnly)) {
      throw std::runtime_error(QString("Cannot open <%1>").arg(filename).toStdString());
    }
    
    Obj8::File obj8(f, true);
    return insertModel(&obj8);
  }
}
