#include <Classification/Object.hpp>
#include <Classification/ObjectModel.hpp>

#include "ObjectTableDialog.hpp"


ObjectTableDialog::ObjectTableDialog (
    Classification::DefinitionsPointer d,
    QWidget *parent,
    Qt::WindowFlags flags)
  : TableDialog (d, d->objectModel(), "Composite Hash", parent, flags)
{
}



ObjectTableDialog::~ObjectTableDialog ()
{
}



bool
ObjectTableDialog::verify (QString key) const
{
  return (false);
  //return (Classification::Object().verifyPrimaryKey(key));
}



bool
ObjectTableDialog::checkDuplicate (QString key) const
{
  return (m_definitions->object(key).isNull());
}


void
ObjectTableDialog::insert (QString key)
{
  /*
   * XXX
   */
#if 0
  m_definitions->upsert (
      Classification::ObjectPointer (
          new Classification::Object (key)
      )
  );
#endif
}


int
ObjectTableDialog::drop (QString key)
{
  Classification::ObjectPointer ptr = m_definitions->object (key);
  return (m_definitions->drop (ptr));
}
