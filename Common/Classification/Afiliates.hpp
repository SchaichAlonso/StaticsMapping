#pragma once

#include <QtCore/QMap>
#include <QtCore/QMultiMap>
#include <QtCore/QPair>
#include <QtCore/QString>

#include "Airline.hpp"
#include "Airport.hpp"
#include "Definitions.hpp"
#include "XPClass.hpp"
#include "XPLibrary.hpp"

namespace Classification
{
  struct Afiliations
  {
    typedef int Fee;
    typedef QPair<Airline::PrimaryKey, Airline::PrimaryKey> AirlinePair;
    typedef QPair<Airport::PrimaryKey, Airport::PrimaryKey> AirportPair;
    typedef QMap<AirportPair, Airport::DistanceInKM> AirportDistanceCache;
    typedef QMap<AirlinePair, Airport::DistanceInKM> HubDistances;
    typedef QMap<AirlinePair, Fee> Tree;
    typedef QMap<QString, Fee> Fees;
    
    typedef QMap<XPClass, Fee> CheapestByXPClass;
    typedef QMultiMap<Fee, ObjectPointer> ObjectsByFee;
    
    Afiliations (DefinitionsPointer);
    virtual ~Afiliations ();
    
    HubDistances hubDistances () const;
    Tree tree () const;
    Fees fees (QString) const;
    ObjectsByXPClass objectsAvailableToAirline (QString) const;
    ObjectsByXPClass objectsAvailable () const;
    
    XPLibrary library () const;
    
    Fee resizedFee (Fee, int) const;
    Fee traverseUpFee () const;
    Fee traverseDownFee () const;
    
    int  maxPlaneAge () const;
    int  year () const;
    void setMaxPlaneAge (int);
    void setYear (int);
    bool outdated (ObjectPointer, Airline::PrimaryKey) const;
    
    Airport::DistanceInKM shortestDistance (AirportDistanceCache &, QStringList, QStringList) const;
    
    void mergeObject  (ObjectsByXPClass &, CheapestByXPClass &, ObjectPointer, XPClass, Fee) const;
    void mergeObjects (ObjectsByXPClass &, CheapestByXPClass &, ObjectPointer, Fee) const;
    
  protected:
    DefinitionsPointer m_definitions;
    HubDistances       m_hub_distances;
    Tree               m_tree;
    Fee                m_walk_up;
    Fee                m_walk_down;
    
    int                m_max_plane_age;
    int                m_year;
  };
}
