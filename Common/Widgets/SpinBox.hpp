#pragma once

#include <QAbstractSpinBox>

#include <Common/Obj8/Number.hpp>

struct SpinBox : QAbstractSpinBox
{
  SpinBox (QWidget * = Q_NULLPTR);
  virtual ~SpinBox ();
  
  const Obj8::Number &value () const;
  void setValue (const Obj8::Number &);
  
  virtual QValidator::State validate (QString &input, int &pos) const Q_DECL_OVERRIDE;
  
  virtual void stepBy (int) Q_DECL_OVERRIDE;
  virtual StepEnabled stepEnabled () const Q_DECL_OVERRIDE;
  
protected:
  Obj8::Number m_value;
  
  Q_OBJECT;
  Q_PROPERTY (Obj8::Number value READ value WRITE setValue);
};
