#pragma once

#include <QtWidgets/QTextEdit>

struct TextEdit : QTextEdit
{
  explicit TextEdit(QWidget *parent = Q_NULLPTR);
  explicit TextEdit(const QString &text, QWidget *parent = Q_NULLPTR);
  virtual ~TextEdit();
  
  void setText(const QString &text);
  
protected Q_SLOTS:
  void handleTextChanged ();
  
Q_SIGNALS:
  void textEdited();
  void textChanged();
  
public:
  Q_OBJECT
};
