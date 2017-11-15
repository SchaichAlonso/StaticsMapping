#include <QtCore/QFile>
#include <QtCore/QRegExp>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>

#include <Common/Classification/Airport.hpp>
#include <Common/Classification/Definitions.hpp>

#include "AptDatExtract.hpp"



#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

AptDatExtractDialog::AptDatExtractDialog (QWidget *parent, Qt::WindowFlags flags)
  : QDialog (parent, flags)
{
  createGui ();
}


AptDatExtractDialog::~AptDatExtractDialog ()
{
  
}


void
AptDatExtractDialog::createGui ()
{
  QVBoxLayout *vbox   = new QVBoxLayout;
  QHBoxLayout *layout = new QHBoxLayout;
  QHBoxLayout *bottom = new QHBoxLayout;
  m_xp_path           = new QLineEdit;
  QPushButton *browse = new QPushButton ("select");
  QPushButton *go     = new QPushButton ("go");
  
  layout->addWidget (new QLabel("XPlane Path:"));
  layout->addWidget (m_xp_path);
  layout->addWidget (browse);
  
  bottom->addStretch();
  bottom->addWidget (go);
  
  vbox->addLayout(layout);
  vbox->addLayout(bottom);

  setLayout (vbox);
  
  connect (browse, &QPushButton::released,
    [&]()
    {
      QString s = QFileDialog::getExistingDirectory ();
      if (not s.isNull()) {
        m_xp_path->setText (s);
      }
    }
  );
  
  connect (go, &QPushButton::released,
    [&]()
    {
      QDir d(m_xp_path->text());
      
      QString dst = Classification::Definitions::dataPath("airports.json");
      QString src = d.absoluteFilePath(
        "Resources/default scenery/default apt dat/Earth nav data/apt.dat");
      
      bool ok = extract (dst, src);
        
      if (ok) {
        QMessageBox::information(this, "success", QString("%1 written.").arg(dst));
      } else {
        QMessageBox::critical(this, "failure", "Failed to extract airport data.");
      }
    }
  );
}



bool
AptDatExtractDialog::extract (QString dst, QString src)
{
  QFile apddat (src);
  bool ok = apddat.open (QFile::ReadOnly);
  if (ok) {
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
    
    QFile file (dst);
    ok = file.open (QFile::WriteOnly | QFile::Truncate);
    
    if (ok) {
      QJsonDocument newdoc;
      newdoc.setObject (obj);
      file.write (newdoc.toJson ());
      file.close ();
    } else {
      qCritical ("Cannot open %s", qUtf8Printable(dst));
    }
  } else {
    qCritical ("Cannot open %s", qUtf8Printable(src));
  }
  
  return (ok);
}



int
main (int argsc, char **argsv)
{
  QApplication app (argsc, argsv);
  app.setQuitOnLastWindowClosed (true);
  
  AptDatExtractDialog x;
  x.show();
  
  return (app.exec());
}

