#include <QtWidgets/QMessageBox>
#include <Widgets/Application.hpp>

#include "MainWindow.hpp"


int
main (int argscnt, char **args)
{
  Widgets::Application app(argscnt, args);
  
  
  try {
    MainWindow w;
    w.show ();
    for (int i=1; i<argscnt; ++i) {
      w.loadObjFile (args[i]);
    }
    return app.exec();
  } catch (const std::exception &e) {
    QMessageBox::critical(Q_NULLPTR, "Error", e.what());
  }
}
