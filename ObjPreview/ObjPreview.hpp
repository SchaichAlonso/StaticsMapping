#pragma once

#include <Common/Widgets/ObjScreen.hpp>
#include <Common/Widgets/VisualModel.hpp>

struct ObjPreview : Widgets::ObjScreen
{
  Q_OBJECT;
  
public:
  ObjPreview (QWidget * = Q_NULLPTR, Qt::WindowFlags = Qt::WindowFlags());
  virtual ~ObjPreview ();
  
  void loadObjFile (QString);
  
protected:
  virtual void dragEnterEvent (QDragEnterEvent *) Q_DECL_OVERRIDE;
  virtual void dropEvent (QDropEvent *) Q_DECL_OVERRIDE;
};
