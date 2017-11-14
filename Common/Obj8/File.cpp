#include <QtCore/QFileInfo>

#include <Common/CryptoHash.hpp>

#include "Parser/SyntaxError.hpp"
#include "TextureHashingVisitor.hpp"

#include "File.hpp"



Obj8::File::File ()
  : m_raw ()
  , m_hash_file ()
  , m_hash_texture ()
  , m_path ()
  , m_header ()
  , m_content ()
{
}



Obj8::File::File (QFile &file, bool __parse)
  : m_raw (file.readAll ())
  , m_hash_file (CryptoHash::hash(m_raw))
  , m_hash_texture ()
  , m_path (file.fileName())
  , m_header ()
  , m_content ()
{
  if (__parse)
    parse ();
}



Obj8::File::File (QString path, QByteArray raw, bool __parse)
  : m_raw (raw)
  , m_hash_file (CryptoHash::hash(m_raw))
  , m_hash_texture ()
  , m_path (path)
  , m_header ()
  , m_content ()
{
  if (__parse)
    parse ();
}



Obj8::File::~File ()
{
}



void
Obj8::File::accept (AbstractVisitor *visitor, bool traverse_all_branches)
{
  m_content.accept (visitor, traverse_all_branches);
}



void
Obj8::File::parse ()
{
  QString              str;
  Parser::LexerContext lc;
  
  str = QString::fromUtf8 (m_raw);
  
  m_header  = Header (str, &lc);
  m_content = RecordSequence (str, &lc);
  
  if (not lc.atEndOf (str)) {
    throw (Parser::SyntaxError (lc));
  }
  
  TextureHashingVisitor visitor (m_path);
  accept (&visitor, true);
  
  if (visitor.texturesHashed() == 0) {
    throw ("Failed to load textures.");
  }
  m_hash_texture = visitor.result ();
}



QString
Obj8::File::toString () const
{
  QString retval;
  
  retval  = m_header.toString ();
  retval += m_content.toString ();
  
  return (retval);
}



QString
Obj8::File::fileHash () const
{
  return (m_hash_file);
}



QString
Obj8::File::textureHash () const
{
  return (m_hash_texture);
}



int
Obj8::File::size () const
{
  int retval;
  
  retval = m_raw.size ();
  
  return (retval);
}



QString
Obj8::File::basename () const
{
  return (QFileInfo(m_path).fileName());
}
