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
}
