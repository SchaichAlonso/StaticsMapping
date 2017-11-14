#include <QtCore/QMimeData>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDropEvent>
#include <QtWidgets/QMessageBox>

#include <Common/Obj8/File.hpp>

#include "ObjPreview.hpp"


ObjPreview::ObjPreview (QWidget *parent, Qt::WindowFlags flags)
  : ObjScreen (parent, flags)
{
  setAcceptDrops (true);
}



ObjPreview::~ObjPreview ()
{
}



void
ObjPreview::loadObjFile (QString path)
{
  QFile f(path);
  if (not f.open (QFile::ReadOnly)) {
    QMessageBox::critical (
      this,
      "Fatal",
      QString("Cannot read %1")
        .arg(path)
    );
    return;
  }
  
  try {
    Obj8::File obj8 (f, true);
    QSharedPointer<VisualModel> model (new VisualModel (path));
    
    obj8.accept (model.data(), false);
    
    setModel (Classification::ObjectPointer(), model);
    
  } catch (const Obj8::Parser::SyntaxError &error) {
    
    Obj8::Parser::LexerContext lc = error.info ();
    
    QMessageBox::critical (
      this,
      "Fatal",
      QString("Error at %1:%2.")
        .arg(lc.line())
        .arg(lc.column())
    );
  }
}


void
ObjPreview::dragEnterEvent (QDragEnterEvent *event)
{
  event->acceptProposedAction();
}


void
ObjPreview::dropEvent (QDropEvent *event)
{
  QString data, path;
  QUrl    url;
  
  data = event->mimeData()->text ();
  url  = QUrl::fromUserInput (data);
  
  path = url.toLocalFile();
  
  if (QFile::exists (path)) {
    loadObjFile (path);
  } else {
    qWarning ("Could not process %s", qUtf8Printable(path));
  }
}
