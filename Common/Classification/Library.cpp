#include <QtCore/QUrl>

#include "Library.hpp"



const char *
Classification::Library::propertyByName (PropertyName p)
{
  switch (p) {
    case KeyProperty: return ("key");
    case NamePropery: return ("name");
    case VersionProperty: return ("version");
    case UrlProperty: return ("url");
  }
  
  abort ();
}


Classification::Library::Library (QString primary_key)
  : Record ()
  , m_key (primary_key)
  , m_name ()
  , m_version ()
  , m_url ()
{
}


Classification::Library::Library (Definitions *q, const QJsonObject &json)
  : Record (q, json)
  , m_key (json.value("key").toString())
  , m_name (json.value("name").toString())
  , m_version (json.value("version").toString())
  , m_url (json.value("url").toString())
{
}


Classification::Library::~Library ()
{
}



QJsonObject
Classification::Library::toJson () const
{
  QJsonObject json = Record::toJson ();
  
  json.insert ("name", m_name);
  json.insert ("key", m_key);
  json.insert ("version", m_version);
  json.insert ("url", m_url);
  
  return (json);
}


const char *
Classification::Library::primaryKeyProperty () const
{
  return (propertyByName(KeyProperty));
}


bool
Classification::Library::verifyPrimaryKey (PrimaryKey) const
{
  return (true);
}


QString
Classification::Library::name () const
{
  return (m_name);
}


void
Classification::Library::setName (QString name)
{
  m_name = name;
}



QString
Classification::Library::version () const
{
  return (m_version);
}



void
Classification::Library::setVersion (QString version)
{
  m_version = version;
}



QString
Classification::Library::url () const
{
  return (m_url);
}



void
Classification::Library::setUrl (QString url)
{
  QUrl u (url);
  if (not u.isValid()) {
    throw ("broken url");
  }
  
  m_url = url;
}
