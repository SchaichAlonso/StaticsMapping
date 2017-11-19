#include "Object.hpp"
#include "XPLibrary.hpp"


Classification::XPLibrary::XPLibrary (DefinitionsPointer defs)
: m_map ()
, m_definitions (defs)
{
}



Classification::XPLibrary::~XPLibrary ()
{
}



void
Classification::XPLibrary::add (XPClass xp_class, ObjectPointer obj, QString airline)
{
  QString vpath = xp_class.libraryVPath (airline);
  QString rpath = obj->fileName ();
  m_map.insert (vpath, rpath);
}




void
Classification::XPLibrary::add (WeightedObjectsByXPClass objs, QString airline)
{
  for (WeightedObjectsByXPClass::ConstIterator i=objs.constBegin(), e=objs.constEnd(); i!=e; ++i) {
    add (i.key(), i.value().object, airline);
  }
}



QString
Classification::XPLibrary::toString (bool extend_only) const
{
  QString retval = "A\n800\nLIBRARY\n\n";
  
  QString last;
  for (QMultiMap<QString,QString>::ConstIterator i=m_map.constBegin(); i!=m_map.constEnd(); ++i) {
    QString vpath = i.key();
    QString rpath = i.value();
    QString instr;
    
    if (extend_only or (last == vpath)) {
      instr = "EXPORT_EXTEND";
    } else {
      instr = "EXPORT";
      last = vpath;
    }
    
    retval.append (QString("%1 %2 %3\n").arg(instr).arg(vpath).arg(rpath));
  }
  
  return (retval);
}
