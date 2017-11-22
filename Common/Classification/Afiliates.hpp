#pragma once

#include <QtCore/QMap>
#include <QtCore/QMultiMap>
#include <QtCore/QPair>
#include <QtCore/QString>

#include "Airline.hpp"
#include "Airport.hpp"
#include "Definitions.hpp"
#include "WeightedObject.hpp"
#include "XPClass.hpp"
#include "XPLibrary.hpp"

namespace Classification
{
  struct Afiliations
  {
    enum EdgeType {
      ChildToParent = 0,
      ParentToChild = 1
    };
    
    typedef WeightedObject::Weight Weight;
    typedef QPair<Airline::PrimaryKey, Airline::PrimaryKey> AirlinePair;
    typedef QPair<Airport::PrimaryKey, Airport::PrimaryKey> AirportPair;
    typedef QMap<AirportPair, Airport::DistanceInKM> AirportDistanceCache;
    typedef QMap<AirlinePair, Airport::DistanceInKM> HubDistances;
    typedef QMap<AirlinePointer, EdgeType> EdgesAtNode;
    typedef QMap<AirlinePointer, EdgesAtNode> EdgesInGraph;
    typedef QMap<AirlinePointer, Weight> Weights;
    
    Afiliations (DefinitionsPointer);
    virtual ~Afiliations ();
    
    HubDistances hubDistances () const;
    EdgesInGraph edges() const;
    Weights weights (AirlinePointer) const;
    Weight  weight(EdgeType) const;
    void    setWeight(EdgeType, Weight);
    
    Weight  resizingWeight (int resizes) const;
    Weight  resizingLinearWeight () const;
    Weight  resizingQuadraticWeight () const;
    void    setResizingLinearWeight (Weight);
    void    setResizingQuadraticWeight (Weight);
    
    WeightedObjectsByXPClass objectsAvailable (QString) const;
    WeightedObjectsByXPClass objectsAvailable (AirlinePointer) const;
    
    XPLibrary library () const;
    
    int  maxPlaneAge () const;
    int  year () const;
    void setMaxPlaneAge (int);
    void setYear (int);
    bool outdated (ObjectPointer, Airline::PrimaryKey) const;
    
    Airport::DistanceInKM shortestDistance (AirportDistanceCache &, QStringList, QStringList) const;
    
    void mergeObject (WeightedObjectsByXPClass &, XPClass, WeightedObject) const;
    void mergeObjects (WeightedObjectsByXPClass &, ObjectPointer, Weight) const;
    
  protected:
    DefinitionsPointer m_definitions;
    HubDistances       m_hub_distances;
    
    EdgesInGraph       m_edges;
    Weight             m_edge_traverse_weight[2];
    Weight             m_resize_linear_weight;
    Weight             m_resize_quadratic_weight;
    
    int                m_max_plane_age;
    int                m_year;
  };
}
