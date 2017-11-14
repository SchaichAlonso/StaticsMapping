#include "TextEdit.hpp"



TextEdit::TextEdit (QWidget *parent)
  : QTextEdit (parent)
{
  connect (this, &QTextEdit::textChanged, this, &TextEdit::handleTextChanged);
}


TextEdit::TextEdit (const QString &text, QWidget *parent)
  : QTextEdit (text, parent)
{
  connect (this, &QTextEdit::textChanged, this, &TextEdit::handleTextChanged);
}



TextEdit::~TextEdit ()
{
}



void
TextEdit::setText (const QString &text)
{
  blockSignals (true);
  QTextEdit::setText (text);
  blockSignals (false);
  
  emit textChanged ();
}



void
TextEdit::handleTextChanged ()
{
  emit textChanged ();
  emit textEdited ();
}
