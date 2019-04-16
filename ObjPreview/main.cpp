#include <QtWidgets/QMessageBox>

#include <Common/Widgets/Application.hpp>
#include <Common/Widgets/OpenGL/Screen.hpp>
#include <Common/Widgets/OpenGL/Obj8Scene.hpp>

int
main (int argscnt, char **args)
{
  Widgets::Application app(argscnt, args);
  
  OpenGL::Screen screen(OpenGL::Obj8ScenePointer(new OpenGL::Obj8Scene()));
  screen.show();
  
  for(int i=1; i!=argscnt; ++i) {
    QString filename(args[i]);
    try {
      OpenGL::Obj8ScenePointer scene(qSharedPointerDynamicCast<OpenGL::Obj8Scene>(screen.scene()));
      if (scene) {
        QVector3D offset((i-1)*50, 0, 0);
        OpenGL::ModelPointer model(scene->insertModel(filename));
        model->setPosition(offset);
      }
    } catch (const Obj8::Parser::SyntaxError &e) {
      QMessageBox::critical(Q_NULLPTR, "Exception", QString("SyntaxError in <%1:%2:%3>").arg(filename).arg(e.info().line()).arg(e.info().column()));
    } catch (const std::exception &e) {
      QMessageBox::critical(Q_NULLPTR, "Exception", QString("std::exception while parsing <%1>: <%2>").arg(filename).arg(e.what()));
    }
  }
  
  return (app.exec());
}
