#pragma once

#include <QtCore/QMap>
#include <QtCore/QMultiMap>
#include <QtCore/QObject>
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
  struct Afiliations : public QObject
  {
    Q_OBJECT
    
    Q_PROPERTY (int currentYear        READ year               WRITE setYear)
    Q_PROPERTY (int retireAge          READ retireAge          WRITE setRetireAge)
    Q_PROPERTY (int retireWeight       READ retireWeight       WRITE setRetireWeight)
    Q_PROPERTY (int relativeDownWeight READ relativeDownWeight WRITE setRelativeDownWeight)
    Q_PROPERTY (int relativeUpWeight   READ relativeUpWeight   WRITE setRelativeUpWeight)
    Q_PROPERTY (int resizeWeight       READ resizeWeight       WRITE setResizeWeight)
    Q_PROPERTY (int regionSize         READ regionSize         WRITE setRegionSize)
    
  
  public:
    enum PropertyName {
      CurrentYearProperty,
      RetireAgeProperty,
      RetireWeightProperty,
      
      RelativeDownProperty,
      RelativeUpProperty,
      ResizeProperty,
      RegionSizeProperty
    };
    static const char *propertyByName (PropertyName);
    QList<const char *> properties() const;
    
    
  public:
    enum EdgeType {
      ChildToParent = 0,
      ParentToChild = 1
    };
    
    typedef WeightedObject::Weight Weight;
    typedef QMap<AirlinePointer, Airport::DistanceInKM> DistanceToAirlines;
    typedef QMap<AirlinePointer, DistanceToAirlines> DistancesToAirlines;
    typedef QMap<AirlinePointer, EdgeType> EdgesAtNode;
    typedef QMap<AirlinePointer, EdgesAtNode> EdgesInGraph;
    typedef QMap<AirlinePointer, Weight> RelativeBonuses;
    
    Afiliations (DefinitionsPointer);
    virtual ~Afiliations ();
    
    RelativeBonuses relativeBonuses (AirlinePointer) const;
    EdgesInGraph edges() const;
    void    setRelativeUpWeight(Weight);
    void    setRelativeDownWeight(Weight);
    Weight  relativeUpWeight() const;
    Weight  relativeDownWeight() const;
    Weight  relative(AirlinePointer a, RelativeBonuses precomputed) const;
    
    DistancesToAirlines distances () const;
    void    setRegionSize(Airport::DistanceInKM size);
    Airport::DistanceInKM regionSize() const;
    Weight  regional(AirlinePointer a, AirlinePointer b) const;
    Airport midpoint(AirlinePointer) const;

    Weight  age(ObjectPointer o, AirlinePointer owner, AirlinePointer user) const;
    bool    introduced(ObjectPointer o, AirlinePointer owner, AirlinePointer user) const;
    int     introductionDate(ObjectPointer o, AirlinePointer owner, AirlinePointer user) const;
    int     retirementDate(ObjectPointer o, AirlinePointer owner, AirlinePointer user) const;
    int     retirementDate(AirlinePointer owner, AirlinePointer user) const;
    void    setRetireAge(int);
    int     retireAge() const;
    void    setRetireWeight(Weight);
    Weight  retireWeight() const;
    
    Weight  resizeWeight (int resizes) const;
    Weight  resizeWeight () const;
    void    setResizeWeight (Weight);
    
    WeightedObjectsByXPClass objectsAvailable (QString) const;
    WeightedObjectsByXPClass objectsAvailable (AirlinePointer) const;
    
    XPLibrary library () const;
    
    int  year () const;
    void setYear (int);
    
    bool enableByPurpose (ObjectPointer, AirlinePointer) const;
    void mergeObject (WeightedObjectsByXPClass &, XPClass, WeightedObject) const;
    void mergeObjects (WeightedObjectsByXPClass &, ObjectPointer, Weight) const;
    
  protected:
    DefinitionsPointer m_definitions;
    DistancesToAirlines  m_regional_distances;
    
    EdgesInGraph       m_edges;
    Weight             m_edge_traverse_weight[2];
    Weight             m_resize_weight;
    Weight             m_region_size;
    Weight             m_retired_weight;
    
    int                m_retired_age;
    int                m_current_year;
  };
}
