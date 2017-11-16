#include "Aircraft.hpp"
#include "Airline.hpp"
#include "Airport.hpp"
#include "Afiliates.hpp"
#include "Object.hpp"



Classification::Afiliations::Afiliations (DefinitionsPointer defs)
: m_definitions (defs)
, m_hub_distances (hubDistances())
, m_walk_up (5)
, m_walk_down (1)
, m_tree (tree ())
, m_max_plane_age (0)
, m_year (0)
{
}



Classification::Afiliations::~Afiliations ()
{
}



Classification::Airport::DistanceInKM
Classification::Afiliations::shortestDistance (
    AirportDistanceCache &cache,
    QStringList hubsa,
    QStringList hubsb
) const
{
  Airport::DistanceInKM closest = 1000000, current;
  AirportDistanceCache::ConstIterator i;
  
  Q_FOREACH (QString a, hubsa) {
    Q_FOREACH (QString b, hubsb) {
      
      AirportPair a_to_b (a, b);
      
      i = cache.constFind (a_to_b);
      if (i != cache.constEnd ()) {
        current = i.value ();
      } else {
        AirportPointer apta = m_definitions->airport (a);
        AirportPointer aptb = m_definitions->airport (b);
        
        if (apta and aptb) {
          AirportPair b_to_a (b, a);
          current = apta->distanceInKM (*aptb);
          cache.insert (b_to_a, current);
          cache.insert (a_to_b, current);
        } else {
          qCritical (
              "Lack of airport(s) %s/%s",
              qUtf8Printable(a),
              qUtf8Printable(b)
          );
          continue;
        }
      }
      
      closest = qMin(closest, current);
      if (closest <= 0) {
        return (0);
      }
    }
  }
  
  return (closest);
}


Classification::Afiliations::HubDistances
Classification::Afiliations::hubDistances () const
{
  HubDistances result;
  
  AirportDistanceCache cache;
  
  Definitions::Airlines airlines = m_definitions->airlines();
  
  Q_FOREACH (AirlinePointer a, airlines) {
    Q_FOREACH (AirlinePointer b, airlines) {
      
      AirlinePair a_to_b (a->icao(), b->icao());
      AirlinePair b_to_a (b->icao(), a->icao());
      
      if (result.contains(a_to_b))
        continue;
      
      Airport::DistanceInKM distance = shortestDistance (cache, a->hubsList(), b->hubsList());
      
      result.insert (a_to_b, distance);
      result.insert (b_to_a, distance);
    }
  }
  
  return (result);
}



Classification::Afiliations::Tree
Classification::Afiliations::tree () const
{
  Tree retval;
  
  Q_FOREACH (AirlinePointer airline, m_definitions->airlines()) {
    if (airline->parent().isEmpty())
      continue;
    if (airline->parent() == airline->icao()) {
      continue;
    }
    
    AirlinePair subsidy_to_owner (airline->icao(), airline->parent());
    AirlinePair owner_to_subsidy (airline->parent(), airline->icao());
    
    Q_ASSERT (not retval.contains (subsidy_to_owner));
    Q_ASSERT (not retval.contains (owner_to_subsidy));
    
    retval.insert (subsidy_to_owner, m_walk_up);
    retval.insert (owner_to_subsidy, m_walk_down);
  }
  
  return (retval);
}



Classification::Afiliations::Fees
Classification::Afiliations::fees (QString icao) const
{
  QList<QString> pending;
  Fees fees;
  
  fees.insert (icao, 0);
  pending.append (icao);
  
  while (not pending.isEmpty()) {
    
    QString current_icao  = pending.takeFirst ();
    
    Tree::ConstIterator i = m_tree.constBegin ();
    Tree::ConstIterator e = m_tree.constEnd ();
    
    for ( ; (i != e); ++i) {
      if (i.key().first == current_icao) {
        break;
      }
    }
    
    Fee current_fee = fees.value (current_icao);
    for ( ; (i != e) and (i.key().first == current_icao); ++i) {
      QString afiliate_icao = i.key().second;
      
      if (fees.contains (afiliate_icao)) {
        continue;
      }
      
      Fee afiliate_fee = current_fee + i.value ();
      
      fees.insert (afiliate_icao, afiliate_fee);
      pending.append (afiliate_icao);
    }
  }
  
  return (fees);
}



Classification::Afiliations::Fee
Classification::Afiliations::resizedFee (Fee basic, int sized) const
{
  return (basic + sized);
}



Classification::Afiliations::Fee
Classification::Afiliations::traverseDownFee () const
{
  return (m_walk_down);
}



Classification::Afiliations::Fee
Classification::Afiliations::traverseUpFee () const
{
  return (m_walk_up);
}



void
Classification::Afiliations::setMaxPlaneAge (int max)
{
  m_max_plane_age = max;
}



void
Classification::Afiliations::setYear (int year)
{
  m_year = year;
}



int
Classification::Afiliations::year () const
{
  return (m_year);
}



int
Classification::Afiliations::maxPlaneAge () const
{
  return (m_max_plane_age);
}



bool
Classification::Afiliations::outdated (ObjectPointer obj, Airline::PrimaryKey user_icao) const
{
  bool age_ok;
  
  if (0 < m_year) {
    int introduced = obj->introduced ();
    int retired = obj->retired ();
  
    AirlinePointer  owner    = m_definitions->airline(obj->livery());
    AirlinePointer  user     = m_definitions->airline(user_icao);
    AircraftPointer aircraft = m_definitions->aircraft(obj->aircraft());
  
    if (introduced <= 0) {
      int founded = qMax (owner->founded(), user->founded());
      introduced = qMax (founded, aircraft->introduced());
    }
    if (retired <= 0) {
      int ret_own = owner->ceased ();
      int ret_usr = user->ceased ();
      
      retired = (0 <= ret_own)? ret_own : ret_usr;
      if (0 < ret_usr) {
        retired = (retired == 0)? ret_usr : qMin (retired, ret_usr);
      }
    }
    
    if (retired <= 0) {
      retired = (m_max_plane_age <= 0)? (m_year) : (introduced + m_max_plane_age);
    }
    age_ok = (introduced <= m_year) and (m_year <= retired);
  } else {
    age_ok = true;
  }
  
  return (not age_ok);
}



void
Classification::Afiliations::mergeObject (
    ObjectsByXPClass &objects,
    CheapestByXPClass &prices,
    ObjectPointer obj,
    XPClass data,
    Fee fee
) const
{
  if (prices.contains (data)) {
    Fee current_cheapest = prices.value (data);
    if (current_cheapest < fee) {
      return;
    }
    if (fee < current_cheapest) {
      objects.remove (data);
      prices.insert (data, fee);
    }
  } else {
    prices.insert (data, fee);
  }
  
  objects.insert (data, obj);
}



void
Classification::Afiliations::mergeObjects (
    ObjectsByXPClass &objects,
    CheapestByXPClass &prices,
    ObjectPointer obj,
    Fee fee
) const
{
  
  QList<XPClass> object_by_xp_classes = 
      XPClass::create (m_definitions->aircraft(obj->aircraft()), obj);
  
  Q_FOREACH (XPClass data, object_by_xp_classes) {
    for (int resized=0; ; ++resized) {
      mergeObject (objects, prices, obj, data, resizedFee (fee, resized));
      if (data.couldBeLarger())
        data=data.nextLarger();
      else
        break;
    }
  }
}



Classification::ObjectsByXPClass
Classification::Afiliations::objectsAvailableToAirline (QString airline) const
{
  CheapestByXPClass prices;
  ObjectsByXPClass result;
  Fees fees;
  
  fees = this->fees (airline);
  
  Q_FOREACH (ObjectPointer obj, m_definitions->objects()) {
    if (outdated(obj, airline)) {
      continue;
    }
    
    QString owner = obj->livery ();
    if (fees.contains (owner)) {
      mergeObjects (result, prices, obj, fees.value (owner));
    }
  }
  
  return (result);
}



Classification::ObjectsByXPClass
Classification::Afiliations::objectsAvailable () const
{
  CheapestByXPClass prices;
  ObjectsByXPClass result;
  
  Q_FOREACH (ObjectPointer obj, m_definitions->objects()) {
    mergeObjects (result, prices, obj, 0);
  }
  
  return (result);
}



Classification::XPLibrary
Classification::Afiliations::library () const
{
  XPLibrary retval (m_definitions);
  
  Q_FOREACH (AirlinePointer airline, m_definitions->airlines()) {
    QString icao = airline->icao ();
    if (not airline->isFictiveIcaoCode(icao)) {
      retval.add (objectsAvailableToAirline(icao), icao);
    }
  }

  retval.add (objectsAvailable());
  
  return (retval);
}
