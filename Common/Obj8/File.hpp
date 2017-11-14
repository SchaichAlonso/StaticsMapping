#pragma once

#include <QByteArray>
#include <QFile>

#include "Header.hpp"
#include "Obj8.hpp"
#include "RecordSequence.hpp"

namespace Obj8
{
  struct File
  {
    File ();
    File (QFile &, bool);
    File (QString, QByteArray, bool);
    ~File ();
    
    void    accept (AbstractVisitor *, bool);
    void    parse ();
    String  toString () const;
    
    QString    fileHash () const;
    QString    textureHash () const;
    int        size () const;
    
    QString    basename () const;
    
  protected:
    QByteArray     m_raw;
    QString        m_hash_file;
    QString        m_hash_texture;
    QString        m_path, m_basename;
    Header         m_header;
    RecordSequence m_content;
  };
}
