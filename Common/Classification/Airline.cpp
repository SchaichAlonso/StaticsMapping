#include <stdexcept>

#include <QRegExp>

#include "Airline.hpp"
#include "Definitions.hpp"


const char *
Classification::Airline::propertyByName (PropertyName p)
{
  switch (p) {
    case IcaoProperty: return ("icao");
    case NameProperty: return ("name");
    case HubsProperty: return ("hubs");
    case HubsMissingProperty: return ("missinghubs");
    case CommentProperty: return ("comment");
    case CeasedProperty: return ("ceased");
    case FoundedProperty: return ("founded");
    case ParentProperty: return ("parent");
  }
  
  abort ();
}



Classification::Airline::Airline (QString icao, QString name)
: IcaoRecord (icao, name)
, m_hubs ()
, m_hubs_missing ()
, m_comment ()
, m_parent (icao)
, m_founded (0)
, m_ceased (0)
{
}


Classification::Airline::Airline (Definitions *d, const QJsonObject &o)
: IcaoRecord (d, o)
, m_hubs ()
, m_hubs_missing ()
, m_comment (o.value("comment").toString())
, m_parent (o.value("parent").toString())
, m_founded (o.value("founded").toInt())
, m_ceased (o.value("ceased").toInt())
{
  setHubs (o.value("hubs").toString());
}



Classification::Airline::~Airline ()
{
}



QJsonObject
Classification::Airline::toJson () const
{
  QJsonObject obj = IcaoRecord::toJson ();
  
  obj.insert ("hubs", allHubs());
  obj.insert ("comment", m_comment);
  obj.insert ("founded", m_founded);
  obj.insert ("ceased", m_ceased);
  obj.insert ("parent", m_parent);

  return (obj);
}



bool
Classification::Airline::verifyPrimaryKey (PrimaryKey str) const
{
  QRegExp expr;
  bool    retval;
  
  expr.setPattern("[A-Z]{3}");
  
  retval = expr.exactMatch (str);
  
  if (retval == false) {
    retval = isFictiveIcaoCode (str);
  }
  
  return (retval);
}



bool
Classification::Airline::isFictiveIcaoCode (QString str)
{
  QRegExp expr;
  bool    retval;
  
  expr.setPattern("XXX-[A-Za-z0-9]*");
  
  retval = expr.exactMatch (str);
  
  return (retval);
}



QString
Classification::Airline::allHubs() const
{
  return (allHubsList().join(" "));
}



QStringList
Classification::Airline::allHubsList() const
{
  return (unique(m_hubs + m_hubs_missing));
}




QString
Classification::Airline::hubs () const
{
  return (hubsList().join(" "));
}



void
Classification::Airline::setHubs (QString hubs)
{
  setHubsList (hubs.split(" ", QString::SkipEmptyParts));
}




QString
Classification::Airline::hubsMissing () const
{
  return (m_hubs_missing.join(" "));
}



void
Classification::Airline::setHubsMissing(QString hubs)
{
  m_hubs_missing = hubs.split(" ", QString::SkipEmptyParts);
}




QStringList
Classification::Airline::hubsList () const
{
  return (m_hubs);
}



void
Classification::Airline::setHubsList (QStringList area)
{
  if (m_q) {
    m_hubs.clear ();
    Q_FOREACH (QString airport, area) {
      if (airport.isEmpty())
        continue;
      if (m_q->airport(airport)) {
        m_hubs.append(airport);
      } else {
        qCritical ("Removing hub %s from %s",
           qUtf8Printable(airport),
           qUtf8Printable(primaryKey())
        );
        m_hubs_missing.append(airport);
      }
    }
    m_hubs = unique(m_hubs);
    m_hubs_missing = unique(m_hubs_missing);
  } else {
    m_hubs = area;
  }
}



QString
Classification::Airline::comment () const
{
  return (m_comment);
}



void
Classification::Airline::setComment (QString comment)
{
  m_comment = comment;
}



int
Classification::Airline::ceased () const
{
  return (m_ceased);
}



void
Classification::Airline::setCeased (int ceased)
{
  m_ceased = ceased;
}



int
Classification::Airline::founded () const
{
  return (m_founded);
}


void
Classification::Airline::setFounded (int founded)
{
  m_founded = founded;
}



QString
Classification::Airline::parent () const
{
  return (m_parent);
}



void
Classification::Airline::setParent (QString parent)
{
  if (m_q) {
    if (not (parent.isEmpty() or m_q->airline(parent))) {
      throw (std::runtime_error("Poor parent airline picked"));
    }
  }
  m_parent = parent;
}



QStringList
Classification::Airline::unique (QStringList list)
{
  QMap<QString,int> map;
  Q_FOREACH(QString string, list) {
    map[string] = 1;
  }
  
  return (map.keys());
}



void
Classification::Airline::fixReferences ()
{
  Q_CHECK_PTR (m_q);
  setHubs (hubs());
}
