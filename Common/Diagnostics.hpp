#pragma once

#include <QMessageLogContext>
#include <QString>

struct Diagnostics
{
  static void output (QtMsgType, QMessageLogContext const&, QString const&);
};
