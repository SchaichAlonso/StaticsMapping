#pragma once

#ifndef _WIN32
#  include <sys/param.h>
#endif

#if __cplusplus < 201103
#  include <iso646.h>
#endif

#include <stddef.h>

#include <QString>

#ifndef nitems /* from sys/params, but not everywhere */
#  define nitems(x) (sizeof(x)/sizeof(*x))
#endif


#define MAPPING_ELEMENT(X, x, y) { X::x, y }
#define MAPPING_ELEMENT_SELF(X, x) { X::x, #x }
template<typename Name>
struct MappingElement
{
  const Name    name;
  const QString string;
};



template<typename Name, size_t table_size>
bool
name_to_string (const MappingElement<Name> (&table) [table_size], QString *string, Name const& name)
{
  size_t i;
  for (i=0; i!=table_size; ++i) {
    if (table[i].name == name) {
      if (string != NULL) {
        *string = table[i].string;
      }
      return (true);
    }
  }
  return (false);
}


template<typename Name, size_t table_size>
bool
string_to_name (const MappingElement<Name> (&table)[table_size], Name *name, QString const& string)
{
  size_t i;
  for (i=0; i!=table_size; ++i) {
    if (table[i].string == string) {
      if (name != NULL) {
        *name = table[i].name;
      }
      return (true);
    }
  }
  return (false);
}
