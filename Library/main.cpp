#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtWidgets/QApplication>

#include "Wizard.hpp"

int
main (int argsc, char **argsv)
{
  QApplication app(argsc, argsv);
  
  QCommandLineParser parser;
  parser.addOption(QCommandLineOption("dst", "scanner attrib", "dst-path"));
  parser.addOption(QCommandLineOption("src", "scanner attrib", "src-path"));
  parser.addOption(QCommandLineOption("reuse", "reuse attrib", "found.json-path"));
  parser.addHelpOption ();
  if (!parser.parse(app.arguments())) {
    qFatal("%s", qUtf8Printable(parser.errorText()));
  }
  
  app.setQuitOnLastWindowClosed(true);
  
  Wizard wiz(parser.value("dst"), parser.value("src"), parser.value("reuse"));
  wiz.show();
  
  return (app.exec());
}
