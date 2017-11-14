#pragma once

#include <QtCore/QCryptographicHash>
#include <QtCore/QString>

struct CryptoHash : QCryptographicHash
{
  CryptoHash ();
  ~CryptoHash ();
  
  static QString hash (const QByteArray &);
  
  QString result() const;
  static int resultLength ();
};
