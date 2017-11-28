#include <QtCore/QUrl>

#include "Library.hpp"
#include "LibraryModel.hpp"
#include "Definitions.hpp"



Classification::LibraryModel::LibraryModel (Definitions *definitions)
: Model (definitions, Library())
{
}



Classification::LibraryModel::~LibraryModel ()
{
}


int
Classification::LibraryModel::column (int name) const
{
  Library::PropertyName pn = (Library::PropertyName)name;
  const char *string = Library::propertyByName (pn);
  return (Model::column (string));
}



Classification::QObjectPointer
Classification::LibraryModel::record (int row) const
{
  return (m_definitions->library (row));
}



int
Classification::LibraryModel::rowCount (const QModelIndex &) const
{
  if (m_definitions) {
    return (m_definitions->numberOfLibraries());
  }
  
  return (0);
}


bool
Classification::LibraryModel::setData (QObjectPointer ptr, const char *property, const QVariant &value)
{
  LibraryPointer p = qSharedPointerDynamicCast<Library>(ptr);
  Q_ASSERT (p);
  
  bool ok = Model::setData (ptr, property, value);
  if (ok) {
    m_definitions->upsert (p);
  }
  
  return (ok);
}
