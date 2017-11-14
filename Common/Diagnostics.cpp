#include <stdio.h>
#include <stdlib.h>

#include "Diagnostics.hpp"

void
Diagnostics::output(QtMsgType level,
                    QMessageLogContext const& ctx,
                    QString const& string)
{

  fprintf (stderr, "%s:%d: %s\n", ctx.file, ctx.line, qPrintable(string));

  if (level == QtFatalMsg)
    abort ();
}
