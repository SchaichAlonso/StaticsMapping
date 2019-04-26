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
      ModelPointer locale{positionIndicator(QColor(Qt::red), QColor(Qt::green), QColor(Qt::blue), 10, true)};
      ModelPointer squares{grid(QVector3D(0,0,1), QVector3D(1,0,0), 256)};
      ModelPointer ground{plane(QVector3D(0,0,1), QVector3D(1,0,0), 256)};
      
      locale->setFlags(0);
      squares->setFlags(Model::Lighting | Model::DepthMasked);
      ground->setFlags(Model::Lighting | Model::DepthMasked);
      
      insertModel(ground);
      insertModel(squares);
      insertModel(locale);
    }
  }
  
  Obj8Scene::~Obj8Scene()
  {
  }
  
  ModelWeakPointer Obj8Scene::insertModel(Obj8::FilePointer file)
  {
    ModelPointer model(new Model());
    QSharedPointer<OpenGL::Obj8Visitor> visitor(new OpenGL::Obj8Visitor(model, file->filename()));
    
    file->accept(visitor.data(), false);
    
    return insertModel(model);
  }
  
  ModelWeakPointer Obj8Scene::insertModel(QString filename)
  {
    QFile f(filename);
    
    if (!f.open(QFile::ReadOnly)) {
      throw std::runtime_error(QString("Cannot open <%1>").arg(filename).toStdString());
    }
    
    return insertModel(Obj8::FilePointer(new Obj8::File(f, true)));
  }
}
