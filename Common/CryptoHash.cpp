#include <limits.h>

#include "CryptoHash.hpp"

CryptoHash::CryptoHash ()
: QCryptographicHash (QCryptographicHash::Sha3_256)
{
}


CryptoHash::~CryptoHash ()
{
}


QString
CryptoHash::hash (const QByteArray &b)
{
  CryptoHash h;
  h.addData (b);
  return (h.result());
}



QString
CryptoHash::result () const
{
  QByteArray b = QCryptographicHash::result ();
  return (b.toHex().toLower());
}



int
CryptoHash::resultLength ()
{
  return (256 / CHAR_BIT);
}
