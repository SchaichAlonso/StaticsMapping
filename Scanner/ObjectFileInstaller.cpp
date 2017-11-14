#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QImage>

#include <Obj8/Command/Action/SmokeBlack.hpp>
#include <Obj8/Command/Action/SmokeWhite.hpp>

#include <Obj8/Command/Geometry/LightCustom.hpp>
#include <Obj8/Command/Geometry/LightNamed.hpp>
#include <Obj8/Command/Geometry/LightParam.hpp>
#include <Obj8/Command/Geometry/LightSpillCustom.hpp>

#include <Obj8/Data/LightVertex.hpp>
#include <Obj8/Data/LineVertex.hpp>
#include <Obj8/Data/Vertex.hpp>

#include <Obj8/Global/Texture.hpp>
#include <Obj8/Global/TextureDraped.hpp>
#include <Obj8/Global/TextureLit.hpp>
#include <Obj8/Global/TextureNormal.hpp>


#include "Common.hpp"
#include "ObjectFileInstaller.hpp"


#include <QtPlugin>
Q_IMPORT_PLUGIN (QDDSPlugin);



ObjectFileInstaller::ObjectFileInstaller ()
  : AbstractVisitor ()
  , m_rotate (Classification::Object::RotateNull)
  , m_target_filename ()
  , m_texture_limit (-1)
{
  reset ();
}



ObjectFileInstaller::~ObjectFileInstaller ()
{
}


void
ObjectFileInstaller::reset ()
{
  m_translation[0] = m_translation[1] = m_translation[2] = Obj8::Parser::Number();
  m_rotate = Classification::Object::RotateNull;
}



bool
ObjectFileInstaller::satisfied (Obj8::StringRef)
{
  /*
   * we don't care.
   */
  return (false);
}



void
ObjectFileInstaller::setMaxTextureResolution (int limit)
{
  m_texture_limit = limit;
}


void
ObjectFileInstaller::setRotation (Classification::Object::Rotate rotate)
{
  m_rotate = rotate;
}



void
ObjectFileInstaller::setTranslation (const Obj8::Parser::Number &x,
                                    const Obj8::Parser::Number &y,
                                    const Obj8::Parser::Number &z)
{
  m_translation[0] = x;
  m_translation[1] = y;
  m_translation[2] = z;
}



void
ObjectFileInstaller::setTargetFilename (Obj8::StringRef filename)
{
  m_target_filename = filename;
}



Obj8::StringRef
ObjectFileInstaller::targetFilename () const
{
  return (m_target_filename);
}



void
ObjectFileInstaller::visit (Obj8::Command::Action::SmokeBlack *s)
{
  applyTransformation (&s->m_x, &s->m_y, &s->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Command::Action::SmokeWhite *s)
{
  applyTransformation (&s->m_x, &s->m_y, &s->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Command::Geometry::LightCustom *v)
{
  applyTransformation (&v->m_x, &v->m_y, &v->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Command::Geometry::LightNamed *v)
{
  applyTransformation (&v->m_x, &v->m_y, &v->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Command::Geometry::LightParam *v)
{
  applyTransformation (&v->m_x, &v->m_y, &v->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Command::Geometry::LightSpillCustom *v)
{
  applyTransformation (&v->m_x, &v->m_y, &v->m_z);
}



void
ObjectFileInstaller::visit (Obj8::Data::LightVertex *v)
{
  applyTransformation (&v->x, &v->y, &v->z);
}



void
ObjectFileInstaller::visit (Obj8::Data::LineVertex *v)
{
  applyTransformation (&v->x, &v->y, &v->z);
}



void
ObjectFileInstaller::visit (Obj8::Data::Vertex *v)
{
  applyTransformation (&v->x, &v->y, &v->z);
}



void
ObjectFileInstaller::visit (Obj8::Global::Texture *t)
{
  installTexture ("-texture", t);
}



void
ObjectFileInstaller::visit (Obj8::Global::TextureDraped *t)
{
  installTexture ("-draped", t);
}



void
ObjectFileInstaller::visit (Obj8::Global::TextureLit *t)
{
  installTexture ("-lit", t);
}



void
ObjectFileInstaller::visit (Obj8::Global::TextureNormal *t)
{
  installTexture ("-normal", t);
}



void
ObjectFileInstaller::applyTransformation (
    Obj8::Parser::Number *x0,
    Obj8::Parser::Number *x1,
    Obj8::Parser::Number *x2
) const
{
  Obj8::Parser::Number x[3];
  switch (m_rotate) {
    case Classification::Object::RotateNull:
      x[0] =  *x0;
      x[2] =  *x2;
      break;
      
    case Classification::Object::RotateCCW90:
      x[0] = -(*x2);
      x[2] =  *x0;
      break;
    
    case Classification::Object::RotateCCW180:
      x[0] = -*x0;
      x[2] = -*x2;
      break;
    
    case Classification::Object::RotateCCW270:
      x[0] =  *x2;
      x[2] = -*x0;
      break;
  }
  x[1] = *x1;
  
  *x0 = x[0] - m_translation[0];
  *x1 = x[1] - m_translation[1];
  *x2 = x[2] - m_translation[2];
}



void
ObjectFileInstaller::installTexture (Obj8::String suffix, Obj8::AbstractTexture *t)
{
  Obj8::String prefix;
  
  prefix = targetFilename ();
  
 if (prefix.endsWith (".obj")) {
   prefix.truncate (prefix.size()-4);
  }
 
  QStringList files = usableTexturePaths (t->path());
  
  Q_FOREACH (QString file, files) {
    installTextureImpl (t, prefix + suffix, file);
  }
}



bool
ObjectFileInstaller::installTextureImpl (
    Obj8::AbstractTexture *t,
    Obj8::String dst_prefix,
    Obj8::String src)
{
  QImage img;
  
  bool ok = img.load (src);
  if (ok) {
    bool scale = (m_texture_limit > 0) and (img.width() > m_texture_limit);
    
    QString ext;
    if (scale) {
      img = img.scaled (m_texture_limit, m_texture_limit);
      ext = ".png";
    } else {
      ext = src.mid(src.lastIndexOf("."));
    }
    
    QString dst = dst_prefix + ext;
    
    if (scale) {
      ok = img.save (dst);
    } else {
      ok = QFile::copy (src, dst);
    }
    
    if (ok) {
      t->setPath (QDir(dst).dirName());
    } else {
      qWarning ("Could not write %s", qUtf8Printable(dst));
    }
  } else {
    qWarning ("Could not read %s", qUtf8Printable(src));
  }
  
  return (ok);
}
