#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtCore/QRegExp>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <Common/Classification/Airport.hpp>
#include <Common/Classification/Definitions.hpp>

int
main (int argsc, char **argsv)
{
  QCoreApplication app (argsc, argsv);
  
  Q_ASSERT (argsc == 2);
  QFile apddat (argsv[1]);
  bool ok = apddat.open (QFile::ReadOnly);
  if (not ok) {
    qFatal ("Cannot open %s", argsv[1]);
  }
  
  QRegExp rec_expr ("^1\\s+\\d+\\s+\\d+\\s+\\d+\\s+(\\w+)\\s+(.*)$");
  QRegExp lat_expr ("^1302\\s+datum_lat\\s+([-0-9.]+).*$");
  QRegExp lon_expr ("^1302\\s+datum_lon\\s+([-0-9.]+).*$");
  
  QString rec, name, lat, lon;
  QJsonArray array;
  
  while (not apddat.atEnd()) {
    QString line = QString::fromUtf8 (apddat.readLine ());
    
    if (rec_expr.indexIn(line) != -1) {
      rec = rec_expr.cap (1);
      name = rec_expr.cap (2);
      name.replace ("\n", "");
      name.replace ("\r", "");
      lat.clear ();
      lon.clear ();
    }
    
    if (lat_expr.indexIn(line) != -1) {
      lat = lat_expr.cap (1);
    }
    
    if (lon_expr.indexIn(line) != -1) {
      lon = lon_expr.cap (1);
    }
    
    if (not (rec.isEmpty() or lat.isEmpty() or lon.isEmpty())) {
      Classification::Airport apt(rec, name, lat.toDouble(), lon.toDouble());
      
      qDebug ("Seen %s", qUtf8Printable(rec));
      array.append (apt.toJson());
      
      rec.clear ();
      lat.clear ();
      lon.clear ();
    }
  }
  apddat.close ();
  
  QJsonObject obj;
  obj.insert ("airports", array);
  
  QFile file (Classification::Definitions::dataPath("airports.json"));
  ok = file.open (QFile::WriteOnly | QFile::Truncate);
  
  if (ok) {
    QJsonDocument newdoc;
    newdoc.setObject (obj);
    file.write (newdoc.toJson ());
    file.close ();
  }
  
  return (EXIT_SUCCESS);
}

