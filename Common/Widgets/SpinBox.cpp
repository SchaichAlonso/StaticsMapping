#include <QLineEdit>

#include "SpinBox.hpp"

SpinBox::SpinBox (QWidget *parent)
  : QAbstractSpinBox (parent)
  , m_value ()
{
  setValue (m_value);
  text();
}



SpinBox::~SpinBox ()
{
}



const Obj8::Number &
SpinBox::value () const
{
  Obj8::Number retval;
  
  retval.fromString (lineEdit()->text());
  
  return (m_value);
}



void
SpinBox::setValue (const Obj8::Number &value)
{
  m_value = value;
  
  lineEdit()->setText(m_value.toString());
}



QValidator::State
SpinBox::validate (QString &input, int &pos) const
{
  Q_UNUSED (pos);
  
  if (input.isEmpty()) {
    return (QValidator::Intermediate);
  } else if (input == "-") {
    return (QValidator::Intermediate);
  }
  
  try {
    Obj8::Number().fromString (input);
  } catch (...) {
    return (QValidator::Invalid);
  }
  
  return (QValidator::Acceptable);
}



void
SpinBox::stepBy (int steps)
{
  setValue (value() + Obj8::Number(steps, 10));
}



SpinBox::StepEnabled
SpinBox::stepEnabled () const
{
  return (StepUpEnabled | StepDownEnabled);
}
