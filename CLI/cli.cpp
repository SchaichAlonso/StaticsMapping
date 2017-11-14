#include <Common/Common.hpp>
#include <Common/Classification/Definitions.hpp>
#include <Common/Classification/ObjectFileInstaller.hpp>

#include <Common/Obj8/Header.hpp>
#include <Common/Obj8/RecordSequence.hpp>

#include <QDir>
#include <QFile>



void
browse (Classification::DefinitionsPointer defs, QString path, int recursions)
{
  if (recursions == 0) {
    return;
  }
  
  QDir dir(path);
  QDir::Filters flags = QDir::AllEntries | QDir::NoDotAndDotDot;
  
  Q_FOREACH (const QFileInfo &content, dir.entryInfoList (flags)) {
    
    QString path = content.canonicalFilePath();
    
    if (content.isDir()) {
      browse (defs, path, recursions - 1);
    }
    
    if (content.isFile()) {
      QFile file(path);
      Classification::ObjectPointer o = defs->match (file);
      if (o) {
        qDebug("found %s", qUtf8Printable(path));
      }
    }
  }
}

#ifdef _WIN32
const char *src_path="C:/Users/schaical/Documents/ruscenery/aircrafts/a-50.obj";
const char *dst_path="C:/tmp/foobar.obj";
#else
const char *src_path="/usr/home/alonso/Downloads/ruscenery/aircrafts/a-50.obj";
const char *dst_path="/tmp/foobar.obj";
#endif

#include <QtCore/QRunnable>
#include <QtCore/QThreadPool>

struct Worker : QRunnable
{
  Worker (QString path);
  virtual ~Worker ();
  
  void run () Q_DECL_OVERRIDE;
  
  QString m_path;
};

Worker::Worker (QString path)
  : QRunnable ()
  , m_path (path)
{
}


Worker::~Worker ()
{
}


void
Worker::run ()
{
  QFile file (m_path);
  bool ok;
  ok = file.open (QFile::ReadOnly);
  if (not ok) {
    qCritical ("%s: Failed to read file.", qUtf8Printable(m_path));
  }
  QString string = QString::fromUtf8(file.readAll());
  
  Obj8::Parser::LexerContext md;
  try {
    Obj8::Header hdr(string, &md);
    Obj8::RecordSequence seq(string, &md);
   
   ok = md.atEndOf(string);
  } catch (...) {
   ok = false;
  }
  
  if (not ok) {
    qDebug ("%s: Stopped reading at line %d, column %d.",
        qUtf8Printable(m_path), md.line(), md.column());
  }
}


void
performance_check (QThreadPool *pool, QString path) {
  QDir dir(path);
  QDir::Filters flags;
  QDir::SortFlags sort;
  
  flags = QDir::AllEntries | QDir::NoDotAndDotDot;
  sort  = QDir::Name;
  
  Q_FOREACH (const QFileInfo &i, dir.entryInfoList (flags, sort)) {
    
    QString path = i.canonicalFilePath ();
    
    if (i.isDir ()) {
      performance_check (pool, path);
    }
    
    if (i.isFile ()) {
      if (not path.endsWith(".obj"))
        continue;
      
      pool->start (new Worker (path));
    }
  }
}

int
main ()
{
  QThreadPool pool;
  
  pool.setExpiryTimeout (-1);
  //pool.setMaxThreadCount (1);
  performance_check (&pool, "C:/Users/schaical/Documents");
  performance_check (&pool, "/usr/home/alonso/Downloads/xplane");
  
  return 0;
  //performance_check("/usr/home/alonso/Downloads/MisterX_Library");
  
#if 0
  //QString path = "C:\\Users\\schaical\\Downloads\\THE-FRUIT-STAND Aircraft Library v2.2\\library.txt";
  QSharedPointer<Definitions> d;

  d = load_defintions ("data.json");

  //Library::load (path, &d);

  QFile file("C:\\Users\\schaical\\Downloads\\THE-FRUIT-STAND Aircraft Library v2.2\\Fruitstand-Aircraft\\Abudhabi_Amiri_B773\\B773_AUH.obj");
  
  browse(d, "C:\\Users\\schaical\\Downloads\\THE-FRUIT-STAND Aircraft Library v2.2", 10);
#else
  QFile src (src_path);
  QFile dst (dst_path);
  src.open (QFile::ReadOnly);
  dst.open (QFile::WriteOnly | QFile::Truncate);
  QString content = src.readAll();
  
  Obj8::Parser::LexerContext md;
  Obj8::Header hdr(content, &md);
  Obj8::RecordSequence seq(content, &md);
  if (not md.atEndOf(content)) {
    qFatal ("Stopped reading at %d:%d.", md.line(), md.column());
  }
  
  ObjectFileInstaller t;
  t.setFilename (src.fileName ());
  t.setTargetFilename (dst.fileName ());
  t.setTranslation (Obj8::Parser::Number(5), Obj8::Parser::Number(10), Obj8::Parser::Number(-10));
  seq.accept (&t, true);
  
  dst.write (qUtf8Printable (hdr.toString ()));
  dst.write (qUtf8Printable (seq.toString ()));
#endif
  
  return (EXIT_SUCCESS);
}
