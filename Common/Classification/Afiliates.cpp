#include <stdexcept>

#include "Aircraft.hpp"
#include "Airline.hpp"
#include "Airport.hpp"
#include "Afiliates.hpp"
#include "Object.hpp"



const char *
Classification::Afiliations::propertyByName (PropertyName name)
{
  switch (name)
  {
    case CurrentYearProperty: return ("currentYear");
    case RetireAgeProperty: return ("retireAge");
    case RetireWeightProperty: return ("retireWeight");
    
    case RelativeDownProperty: return ("relativeDownWeight");
    case RelativeUpProperty: return ("relativeUpWeight");
    case ResizeProperty: return ("resizeWeight");
    case RegionSizeProperty: return ("regionSize");
  }
}



QList<const char *>
Classification::Afiliations::properties () const
{
  QList<const char *> retval;
  const QMetaObject *obj = metaObject ();
  
  for (int i=0, k=obj->propertyCount(); i!=k; ++i) {
    QMetaProperty mp = obj->property(i);
    const char *name = mp.name();
    if (qstrcmp("objectName", name)) {
      retval.append (name);
    }
  }
  
  return (retval);
}



Classification::Afiliations::Afiliations (DefinitionsPointer defs)
: QObject ()
, m_definitions (defs)
, m_regional_distances (distances())
, m_edges (edges())
, m_edge_traverse_weight {0, 0}
, m_resize_weight (100)
, m_region_size (100)
, m_retired_weight (100)
, m_retired_age (30)
, m_current_year (0)
{
  setRelativeDownWeight(1);
  setRelativeUpWeight(1);
}



Classification::Afiliations::~Afiliations ()
{
}



Classification::Afiliations::DistancesToAirlines
Classification::Afiliations::distances () const
{
    DistancesToAirlines retval;
  
  Q_FOREACH (AirlinePointer a, m_definitions->airlines()) {
    QStringList a_hub_icaos = a->hubsList();
    Q_FOREACH (AirlinePointer b, m_definitions->airlines()) {
      QStringList b_hub_icaos = b->hubsList();
      Airport::DistanceInKM d;
      
      if (a != b) {
        if (a_hub_icaos.isEmpty() or b_hub_icaos.isEmpty()) {
          d = Airport::largestPossibleDistanceInKM();
        } else {
          Airport a_pivot = midpoint(a);
          Airport b_pivot = midpoint(b);
          d = a_pivot.distanceInKM(b_pivot);
        }
      } else {
        d = 0;
      }
      retval[a][b] = d;
    }
  }
  
  return (retval);
}



void
Classification::Afiliations::setRegionSize (Airport::DistanceInKM size)
{
  m_region_size = size;
}



Classification::Airport::DistanceInKM
Classification::Afiliations::regionSize () const
{
  return (m_region_size);
}



Classification::Afiliations::Weight
Classification::Afiliations::regional (AirlinePointer a, AirlinePointer b) const
{
  Airport::DistanceInKM distance = m_regional_distances[a][b];
  
  Weight buckets = distance / qMax(m_region_size, 1);
  
  return (m_region_size * (buckets+1));
}



Classification::Airport
Classification::Afiliations::midpoint (AirlinePointer airline) const
{
  Airport::Degrees lat=0, lon=0;
  
  QStringList icaos = airline->hubsList();
  Q_FOREACH (QString icao, icaos) {
    AirportPointer airport = m_definitions->airport(icao);
    Airport::Degrees clat = airport->latitude();
    Airport::Degrees clon = airport->longitude();
    
    clon = clon<0? 360-clon : clon;
    
    lat += clat;
    lon += clon;
  }
  
  lat /= icaos.size();
  lon /= icaos.size();
  
  return (Airport(QString(), QString(), lat, lon));
}



Classification::Afiliations::EdgesInGraph
Classification::Afiliations::edges () const
{
  EdgesInGraph edges;
  
  Definitions::Airlines all = m_definitions->airlines();
  Q_FOREACH (AirlinePointer airline, all) {
    if (airline->parent().isEmpty())
      continue;
    if (airline->parent() == airline->icao()) {
      continue;
    }
    
    Q_ASSERT (all.contains(airline->parent()));
    edges[all[airline->parent()]][airline] = ParentToChild;
    edges[airline][all[airline->parent()]] = ChildToParent;
  }
  
  return (edges);
}



Classification::Afiliations::RelativeBonuses
Classification::Afiliations::relativeBonuses (AirlinePointer airline) const
{
  QList<AirlinePointer> pending;
  RelativeBonuses weights;
  
  weights.insert (airline, 0);
  pending.append (airline);
  
  while (not pending.isEmpty()) {
    
    AirlinePointer n = pending.takeFirst();
    Weight n_weight = weights[n];
    
    EdgesAtNode peers = m_edges[n];
    Q_FOREACH(AirlinePointer peer, peers.keys()) {
      
      Weight peer_weight_old = weights.value(peer, std::numeric_limits<Weight>::max());
      Weight peer_weight_new = n_weight + m_edge_traverse_weight[peers[peer]];
      
      if (peer_weight_new < peer_weight_old) {
        weights[peer] = qMin(peer_weight_old, peer_weight_new);
        pending.append(peer);
      }
    }
  }
  
  return (weights);
}



Classification::Afiliations::Weight
Classification::Afiliations::relative (AirlinePointer a, RelativeBonuses precomputed) const
{
  Weight n, m;
  
  if (precomputed.contains(a)) {
    n = Airport::largestPossibleDistanceInKM();
    m = precomputed.value(a);
    m = qMax(m, 1);
  } else {
    n = 0;
    m = 1;
  }
  
  return (-n/m);
}



Classification::Afiliations::Weight
Classification::Afiliations::age (
  ObjectPointer obj,
  AirlinePointer owner,
  AirlinePointer user
) const
{
  Weight age;
  
  if (0 < m_current_year) {
    int retired = retirementDate (obj, owner, user);
    
    if ((retired == -1) or (m_current_year < retired)) {
      age = 0;
    } else {
      age = (m_current_year - retired) * m_retired_weight;
    }
  } else {
    age = 0;
  }
  
  return (age);
}



bool
Classification::Afiliations::introduced (
  ObjectPointer o,
  AirlinePointer owner,
  AirlinePointer user
) const
{
  bool introduced;
  
  if (0 < m_current_year) {
    int date = introductionDate (o, owner, user);
    introduced = date <= m_current_year;
  } else {
    introduced = true;
  }
  
  return (introduced);
}



int
Classification::Afiliations::introductionDate (
  ObjectPointer obj,
  AirlinePointer owner,
  AirlinePointer user
) const
{
  int date = obj->introduced();
  
  if (date <= 0) {
    AircraftPointer aircraft = m_definitions->aircraft(obj->aircraft());
    date = aircraft->introduced();
  }
  
  int founded = qMax(owner->founded(), user->founded());
  date = qMax(founded, date);
  
  return (date);
}



int
Classification::Afiliations::retirementDate (
  ObjectPointer obj,
  AirlinePointer owner,
  AirlinePointer user
) const
{
  int date = obj->retired();
  
  int ceased = retirementDate (owner, user);
  
  if (ceased > 0) {
    if ((date <= 0) || (ceased < date)) {
      date = ceased;
    }
  }
  
  if (date <= 0) {
    if ((m_current_year > 0) && (m_retired_age > 0)) {
      int introduction = introductionDate(obj, owner, user);
      if (introduction > 0) {
        date = introduction + m_retired_age;
      }
    }
  }
  
  return (date);
}



int
Classification::Afiliations::retirementDate (
  AirlinePointer owner,
  AirlinePointer user
) const
{
  int ret_own = owner->ceased ();
  int ret_usr = user->ceased ();
    
  int ceased = (ret_own < ret_usr)? ret_own : ret_usr;
  if (ceased <= 0) {
    ceased =   (ret_own < ret_usr)? ret_usr : ret_own;
  }
  
  return (ceased);
}




void
Classification::Afiliations::setRetireAge (int age)
{
  m_retired_age = age;
}



int
Classification::Afiliations::retireAge () const
{
  return (m_retired_age);
}



void
Classification::Afiliations::setRetireWeight (Weight w)
{
  m_retired_weight = w;
}



Classification::Afiliations::Weight
Classification::Afiliations::retireWeight() const
{
  return (m_retired_weight);
}



void
Classification::Afiliations::setRelativeUpWeight (Weight w)
{
  if (w < 0) {
    throw (std::runtime_error("Value out of Range."));
  }
  m_edge_traverse_weight[ChildToParent] = w;
}



void
Classification::Afiliations::setRelativeDownWeight (Weight w)
{
  if (w < 0) {
    throw (std::runtime_error("Value out of Range."));
  }
  m_edge_traverse_weight[ParentToChild] = w;
}



Classification::Afiliations::Weight
Classification::Afiliations::relativeUpWeight() const
{
  return (m_edge_traverse_weight[ChildToParent]);
}



Classification::Afiliations::Weight
Classification::Afiliations::relativeDownWeight() const
{
  return (m_edge_traverse_weight[ParentToChild]);
}



Classification::Afiliations::Weight
Classification::Afiliations::resizeWeight (int x) const
{
  Weight retval = x * resizeWeight();
  
  return (retval);
}



Classification::Afiliations::Weight
Classification::Afiliations::resizeWeight () const
{
  return (m_resize_weight);
}



void
Classification::Afiliations::setResizeWeight (Weight w)
{
  m_resize_weight = w;
}



void
Classification::Afiliations::setYear (int year)
{
  m_current_year = year;
}



int
Classification::Afiliations::year () const
{
  return (m_current_year);
}



bool
Classification::Afiliations::enableByPurpose(ObjectPointer obj, AirlinePointer airline) const
{
#if 0
  int o = (int) obj->purpose();
  int a = (int) airline->purposes();
  
  int enabled = a & o;
  
  return (enabled != 0);
#endif
  return (true);
}



void
Classification::Afiliations::mergeObject (
  WeightedObjectsByXPClass &objects,
  XPClass xpclass,
  WeightedObject object
) const
{
  if (objects.contains(xpclass)) {
    Weight current_cheapest = objects.value(xpclass).weight;
    Weight candidate_price = object.weight;
    
    if (candidate_price <= current_cheapest) {
      if (candidate_price < current_cheapest) {
        objects.remove(xpclass);
      }
      objects.insert (xpclass, object);
    }
  } else {
    objects.insert (xpclass, object);
  }
}



void
Classification::Afiliations::mergeObjects (
  WeightedObjectsByXPClass &objects,
  ObjectPointer object,
  Weight initial
) const
{
  QList<XPClass> xp_classes = 
      XPClass::create (m_definitions->aircraft(object->aircraft()), object);
  
  Q_FOREACH (XPClass xp_class, xp_classes) {
    for (int resized=0; ; ++resized) {
      Weight w = initial + resizeWeight(resized);
      mergeObject (objects, xp_class, WeightedObject(object, w));
      if (xp_class.couldBeLarger())
        xp_class=xp_class.nextLarger();
      else
        break;
    }
  }
}



Classification::WeightedObjectsByXPClass
Classification::Afiliations::objectsAvailable (QString airline) const
{
  return (objectsAvailable(m_definitions->airline(airline)));
}



Classification::WeightedObjectsByXPClass
Classification::Afiliations::objectsAvailable (AirlinePointer airline) const
{
  WeightedObjectsByXPClass result;
  
  if (airline) {
    RelativeBonuses bonuses = relativeBonuses(airline);
  
    Q_FOREACH (ObjectPointer obj, m_definitions->objects()) {
      
      AirlinePointer owner = m_definitions->airline(obj->livery());
      if (owner->hasFictiveIcaoCode()) {
        continue;
      }
      
      if (not introduced(obj, owner, airline)) {
        continue;
      }
      
      Weight w = age(obj, owner, airline);
      
      if (owner != airline) {
        Weight w_regional = regional (airline, owner);
        Weight w_relative = relative (owner, bonuses);
        
        w += qMax(w_regional + w_relative, 1);
      }
      mergeObjects (result, obj, w);
    }
  } else {
    Q_FOREACH (ObjectPointer obj, m_definitions->objects()) {
      mergeObjects (result, obj, 0);
    }
  }
  
  return (result);
}



Classification::XPLibrary
Classification::Afiliations::library () const
{
  XPLibrary retval (m_definitions);
  
  Q_FOREACH (AirlinePointer airline, m_definitions->airlines()) {
    if (not airline->hasFictiveIcaoCode()) {
      retval.add (objectsAvailable(airline), airline->icao());
    }
  }

  retval.add (objectsAvailable(AirlinePointer()));
  
  return (retval);
}
