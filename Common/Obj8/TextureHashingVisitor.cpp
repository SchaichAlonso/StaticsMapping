#include <QtCore/QFile>

#include <Common/Obj8/Global/Texture.hpp>
#include <Common/Obj8/Global/TextureDraped.hpp>
#include <Common/Obj8/Global/TextureLit.hpp>
#include <Common/Obj8/Global/TextureNormal.hpp>

#include "TextureHashingVisitor.hpp"



Obj8::TextureHashingVisitor::TextureHashingVisitor (QString f)
: AbstractVisitor (f)
, m_hash ()
, m_processed_textures (0)
{
}



Obj8::TextureHashingVisitor::~TextureHashingVisitor ()
{
}



void
Obj8::TextureHashingVisitor::visit (Global::Texture *t)
{
  String path = t->path ();
  if (path.isEmpty()) {
    ++m_processed_textures;
  } else {
    hashTexture (path);
  }
}



void
Obj8::TextureHashingVisitor::visit (Global::TextureDraped *t)
{
  hashTexture(t->path());
}



void
Obj8::TextureHashingVisitor::visit (Global::TextureLit *t)
{
  hashTexture(t->path());
}



void
Obj8::TextureHashingVisitor::visit (Global::TextureNormal *t)
{
  hashTexture(t->path());
}



void
Obj8::TextureHashingVisitor::reset ()
{
  m_hash.reset ();
  m_processed_textures = 0;
}



bool
Obj8::TextureHashingVisitor::satisfied (StringRef)
{
  return (true);
}


int
Obj8::TextureHashingVisitor::texturesHashed () const
{
  return (m_processed_textures);
}


QString
Obj8::TextureHashingVisitor::result () const
{
  return (m_hash.result());
}



void
Obj8::TextureHashingVisitor::hashTexture (QString path)
{
  QStringList texture_paths = usableTexturePaths (path);
  
  Q_FOREACH (QString texture_path, texture_paths) {
    QFile f (texture_path);
    
    if (not f.open (QFile::ReadOnly)) {
      qDebug ("Skipping unreadable Texture %s.", qUtf8Printable(texture_path));
      continue;
    }
    
    qint64  size = f.size ();
    uchar  *mmap = f.map (0, size);
    char   *data = reinterpret_cast<char *>(mmap);
    
    for (qint64 i=0, blksize=(1<<20), cursiz; i<size; i+=cursiz) {
      cursiz = qMin (blksize, size-i);
      m_hash.addData (data+i, cursiz);
    }
    f.unmap (mmap);
    f.close ();
    
    ++m_processed_textures;
    
    /*
     * Do not process more than one texture.
     */
    break;
  }
}
