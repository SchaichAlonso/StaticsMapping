#include <Classification/Library.hpp>
#include <Classification/LibraryModel.hpp>

#include "LibraryTableDialog.hpp"


LibraryTableDialog::LibraryTableDialog (
    Classification::DefinitionsPointer d,
    QWidget *parent,
    Qt::WindowFlags flags)
  : TableDialog (d, d->libraryModel(), "Library Identifier", parent, flags)
{
}



LibraryTableDialog::~LibraryTableDialog ()
{
}



bool
LibraryTableDialog::verify (QString icao) const
{
  return (Classification::Library().verifyPrimaryKey(icao));
}



bool
LibraryTableDialog::checkDuplicate (QString key) const
{
  return (m_definitions->library(key).isNull());
}


void
LibraryTableDialog::insert (QString key)
{
  m_definitions->upsert (
      Classification::LibraryPointer (
          new Classification::Library (key)
      )
  );
}


int
LibraryTableDialog::drop (QString key)
{
  Classification::LibraryPointer ptr = m_definitions->library (key);
  return (m_definitions->drop (ptr));
}
