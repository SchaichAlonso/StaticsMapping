#include "WeightedObject.hpp"



namespace Classification
{
  WeightedObject::WeightedObject(ObjectPointer object, Weight weight)
    : object(object)
    , weight(weight)
  {
  }



  WeightedObject::~WeightedObject()
  {
  }
  
  
  
  bool WeightedObject::operator== (const WeightedObject &other) const
  {
    if (object != other.object)
      return (false);
    
    if (weight != other.weight)
      return (false);
    
    return (true);
  }
}
