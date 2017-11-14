#include <QtCore/QMetaEnum>

#include <Classification/Aircraft.hpp>
#include <Classification/Object.hpp>
#include <Classification/XPClass.hpp>

#include "XPClass.hpp"

void
Classification::Test::XPClass::vpaths ()
{
  QMetaEnum sizes    = QMetaEnum::fromType<Aircraft::Size> ();
  QMetaEnum types    = QMetaEnum::fromType<Aircraft::Engine> ();
  QMetaEnum purposes = QMetaEnum::fromType<Object::Purpose> ();
  
  for (int pidx=0; pidx!=purposes.keyCount(); ++pidx) {
    for (int tidx=0; tidx!=types.keyCount(); ++tidx) {
      for (int sidx=0; sidx!=sizes.keyCount(); ++sidx) {
        QList<Classification::XPClass> data;
        
        data = Classification::XPClass::create (
            (Object::Purpose)purposes.value(pidx),
            (Aircraft::Engine)types.value(tidx),
            (Aircraft::Size)sizes.value(sidx)
        );
        
        Q_FOREACH (const Classification::XPClass &d, data) {
          qDebug (
              "(%s,%s,%s) -> %s",
              purposes.key(pidx),
              types.key(tidx),
              sizes.key(sidx),
              qUtf8Printable(d.libraryVPath())
          );
        }
      }
    }
  }
}

QTEST_MAIN (Classification::Test::XPClass);
