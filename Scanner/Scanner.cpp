#include <QtCore/QFileInfo>

#include <Classification/Object.hpp>

#include "Scanner.hpp"
#include "Worker.hpp"

Scanner::Scanner (QString dst, int max_texture_resolution)
  : QThreadPool ()
  , m_objs_lock ()
  , m_objs ()
  , m_all (Classification::Definitions::fromFile ())
  , m_found (new Classification::Definitions(*m_all))
  , m_dst (dst)
  , m_texture_limit (max_texture_resolution)
{
  if (not m_all) {
    return;
  }
  
  QFileInfo info (dst);
  if (not info.isDir()) {
    throw "not a directory";
  }
  
  setExpiryTimeout (-1);
}



Scanner::~Scanner ()
{
}



QString
Scanner::found (Classification::ObjectPointer obj)
{
  Classification::ObjectPointer copy (new Classification::Object(*obj));
  QString objname, basename;
  int n;
  
  objname = obj->name ();
  
  QMutexLocker locker (&m_objs_lock);
  
  n = m_objs.value (objname, 0);
  m_objs[objname] = n+1;
  
  basename = QString("%1-%2.obj").arg(objname).arg(n);
  
  copy->setFileName (basename);
  m_found->upsert (copy);
  
  locker.unlock();
  
  return (m_dst.absoluteFilePath(basename));
}



void
Scanner::scan (QString path)
{
  m_found->dropObjects ();
  
  Q_FOREACH (Classification::AircraftPointer a, m_all->aircrafts()) {
    m_found->upsert (a);
  }
  Q_FOREACH (Classification::AirlinePointer a, m_all->airlines()) {
    m_found->upsert (a);
  }
  for (int i=0; i!=m_all->numberOfLibraries(); ++i) {
    m_found->upsert (m_all->library(i));
  }
  
  scanImpl (path);
  
  waitForDone ();
  
  m_found->toFile (m_dst.absoluteFilePath("found.json"));
  
  qDebug ("done.");
}


void
Scanner::scanImpl (QString path)
{
  QDir dir(path);
  QDir::Filters flags;
  QDir::SortFlags sort;
  
  flags = QDir::AllEntries | QDir::NoDotAndDotDot;
  sort  = QDir::Name;
  
  Q_FOREACH (const QFileInfo &i, dir.entryInfoList (flags, sort)) {
    
    QString subpath = i.canonicalFilePath ();
    
    if (i.isDir ()) {
      scanImpl (subpath);
    }
    
    if (i.isFile() ) {
      if (i.isReadable()) {
        if (m_all->couldMatch(i.size())) {
          QThreadPool::start (new Worker (this, m_all, subpath, m_texture_limit));
        }
      }
    }
  }
}
