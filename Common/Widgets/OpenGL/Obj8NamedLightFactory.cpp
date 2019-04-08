#include <QtCore/QJsonObject>
#include <QtGui/QMatrix4x4>

#include <Common/DataPath.hpp>
#include <Common/Json.hpp>

#include "Obj8NamedLightFactory.hpp"

namespace OpenGL {
  
  Obj8NamedLightFactory::Obj8NamedLightFactory()
  : m_lights(init())
  {
  }
  
  
  LightPointer Obj8NamedLightFactory::create(QVector3D position, QString name)
  {
    LightPointer retval;
    
    if(m_lights.contains(name)) {
      QMatrix4x4 translate;
      translate.translate(position);
      retval.reset(new Light{translate, *m_lights[name]});
    }
    
    return retval;
  }
  
  
  Obj8NamedLightFactory::KnownLights Obj8NamedLightFactory::init()
  {
    QJsonObject doc(Json::readJsonFile(DataPath::existingPath("obj8-lights.json")).object());
    
    KnownLights cache;
    
    Q_FOREACH(QString name, doc.keys()) {
      QJsonObject light{doc[name].toObject()};
      cache.insert(name, LightPointer(new Light{light}));
    }
    
    return (cache);
  }
}
