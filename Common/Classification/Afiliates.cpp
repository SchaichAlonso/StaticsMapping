#include "Aircraft.hpp"
#include "Airline.hpp"
#include "Airport.hpp"
#include "Afiliates.hpp"
#include "Object.hpp"



Classification::Afiliations::Afiliations (DefinitionsPointer defs)
: m_definitions (defs)
, m_hub_distances (hubDistances())
, m_edges (edges ())
, m_edge_traverse_weight {1, 5}
, m_resize_linear_weight (1)
, m_resize_quadratic_weight (0)
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



Classification::Afiliations::Weights
Classification::Afiliations::weights (AirlinePointer airline) const
{
  QList<AirlinePointer> pending;
  Weights weights;
  
  weights.insert (airline, 0);
  pending.append (airline);
  
  while (not pending.isEmpty()) {
    
    AirlinePointer n = pending.takeFirst();
    Weight n_w = weights[n];
    
    EdgesAtNode n_edges = m_edges[n];
    for (EdgesAtNode::ConstIterator i=n_edges.constBegin(); i!=n_edges.constEnd(); ++i) {
      if (weights.contains(i.key())) {
        continue;
      }
      
      Weight i_fee = n_w + weight(i.value());
      weights.insert (i.key(), i_fee);
      
      pending.append (i.key());
    }
  }
  
  return (weights);
}



Classification::Afiliations::Weight
Classification::Afiliations::weight (EdgeType e) const
{
  return (m_edge_traverse_weight[e]);
}



void
Classification::Afiliations::setWeight (EdgeType e, Weight w)
{
  m_edge_traverse_weight[e] = w;
}



Classification::Afiliations::Weight
Classification::Afiliations::resizingWeight (int x) const
{
  Weight linear    = x * resizingLinearWeight();
  Weight quadratic = x * x * resizingQuadraticWeight();
  
  return (linear + quadratic);
}



Classification::Afiliations::Weight
Classification::Afiliations::resizingLinearWeight () const
{
  return (m_resize_linear_weight);
}



Classification::Afiliations::Weight
Classification::Afiliations::resizingQuadraticWeight () const
{
  return (m_resize_quadratic_weight);
}



void
Classification::Afiliations::setResizingLinearWeight (Weight w)
{
  m_resize_linear_weight = w;
}



void
Classification::Afiliations::setResizingQuadraticWeight (Weight w)
{
  m_resize_quadratic_weight = w;
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
      Weight w = initial + resizingWeight(resized);
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
    Weights w = weights(airline);
  
    Q_FOREACH (ObjectPointer obj, m_definitions->objects()) {
      if (outdated(obj, airline->primaryKey())) {
        continue;
      }
    
      AirlinePointer owner = m_definitions->airline(obj->livery());
      if (w.contains(owner)) {
        mergeObjects (result, obj, w.value(owner));
      }
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
