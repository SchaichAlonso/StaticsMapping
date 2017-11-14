#include <Obj8/Parser/LexerContext.hpp>
#include <Obj8/Parser/SyntaxError.hpp>
#include <Obj8/File.hpp>

#include "ObjectFileInstaller.hpp"
#include "Worker.hpp"

Worker::Worker (Scanner *scanner,
                Classification::DefinitionsPointer definitions,
                QString src,
                int texture_limit)
  : QRunnable ()
  , m_definitions (definitions)
  , m_src (src)
  , m_scanner (scanner)
  , m_texture_limit (texture_limit)
{
}


Worker::~Worker ()
{
}


void
Worker::run ()
{
  Obj8::File obj8 (m_src, content(), false);
  Classification::ObjectPointer metadata;
  
  /*
   * size already matched by primary thread.
   */
  bool chance = m_definitions->couldMatch (obj8.fileHash());
  if (not chance) {
    qDebug ("%s: unknown primary hash.", qUtf8Printable(m_src));
    return;
  }
  
  try {
    obj8.parse ();
  } catch (const Obj8::Parser::SyntaxError &error) {
    /*
     * There are no obj files in the database that fail to be parsed...
     */
    qFatal ("%s: Stopped reading at line %d, column %d.",
        qUtf8Printable(m_src), error.info().line(), error.info().column());
  }

  metadata = m_definitions->match (obj8.size(), obj8.fileHash(), obj8.textureHash());
  if (not metadata) {
    qDebug ("%s: unknown secondary hash.", qUtf8Printable(m_src));
    return;
  }
  
  QString dst = m_scanner->found (metadata);
  qDebug ("%s <- %s", qUtf8Printable(dst), qUtf8Printable(m_src));
  
  ObjectFileInstaller installer;
  installer.setFilename (m_src);
  installer.setTargetFilename (dst);
  installer.setRotation (metadata->rotate());
  installer.setTranslation (
        metadata->translate(0),
        metadata->translate(1),
        metadata->translate(2)
  );
  installer.setMaxTextureResolution (m_texture_limit);
  obj8.accept (&installer, true);
  
  QFile sink (dst);
  if (not sink.open (QFile::WriteOnly | QFile::Truncate)) {
    qFatal ("Could not write to \'%s\'.", qUtf8Printable(dst));
  }
  
  sink.write (obj8.toString().toUtf8());
  sink.close ();
}



QByteArray
Worker::content () const
{
  QFile file (m_src);
  QByteArray content;
  
  bool ok;
  ok = file.open (QFile::ReadOnly);
  if (not ok) {
    qFatal ("%s: Failed to read file.", qUtf8Printable(file.fileName()));
  }
  
  content = file.readAll ();
  
  return (content);
}
