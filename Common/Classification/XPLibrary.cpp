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
Classification::XPLibrary::add (ObjectsByXPClass objs, QString airline)
{
  for (ObjectsByXPClass::ConstIterator i=objs.constBegin(); i!=objs.constEnd(); ++i) {
    QString vpath = i.key().libraryVPath (airline);
    QString rpath = i.value()->fileName ();
    m_map.insert (vpath, rpath);
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
