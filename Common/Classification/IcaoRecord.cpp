#include "IcaoRecord.hpp"


Classification::IcaoRecord::IcaoRecord (const IcaoRecord &o)
  : Record (o)
  , m_icao (o.m_icao)
  , m_name (o.m_name)
{
}



Classification::IcaoRecord::IcaoRecord (QString icao, QString name)
  : Record ()
  , m_icao (icao)
  , m_name (name)
{
}



Classification::IcaoRecord::IcaoRecord (Definitions *q, const QJsonObject &o)
  : Record (q, o)
  , m_icao (o.value("icao").toString())
  , m_name (o.value("name").toString())
{
}



Classification::IcaoRecord::~IcaoRecord ()
{
}



bool
Classification::IcaoRecord::verifyIcaoCode (QString icao) const
{
  return (verifyPrimaryKey (icao));
}



bool
Classification::IcaoRecord::hasFictiveIcaoCode () const
{
  return (not verifyIcaoCode(icao()));
}



QString
Classification::IcaoRecord::icao () const
{
  return (m_icao);
}



void
Classification::IcaoRecord::setIcao (QString icao)
{
  m_icao = icao;
}



QString
Classification::IcaoRecord::name () const
{
  return (m_name);
}



void
Classification::IcaoRecord::setName (QString name)
{
  m_name = name;
}



const char *
Classification::IcaoRecord::primaryKeyProperty () const
{
  return ("icao");
}



QJsonObject
Classification::IcaoRecord::toJson () const
{
  QJsonObject o = Record::toJson ();
  
  o.insert ("icao", m_icao);
  o.insert ("name", m_name);
  
  return (o);
}
