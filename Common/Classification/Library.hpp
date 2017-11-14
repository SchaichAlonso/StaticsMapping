#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QString>

#include "Classification.hpp"
#include "Record.hpp"

namespace Classification
{
  struct Library : Record
  {
    Q_OBJECT
    Q_PROPERTY (QString key     MEMBER m_key CONSTANT);
    Q_PROPERTY (QString name    READ name    WRITE setName);
    Q_PROPERTY (QString version READ version WRITE setVersion);
    Q_PROPERTY (QString url     READ url     WRITE setUrl);
  public:
    enum PropertyName {
      KeyProperty,
      NamePropery,
      VersionProperty,
      UrlProperty
    };
    static const char *propertyByName (PropertyName p);
    
  public:
    Library (QString =QString());
    Library (Definitions *, const QJsonObject &);
    virtual ~Library ();
    
    virtual QJsonObject toJson () const Q_DECL_OVERRIDE;
    
    virtual const char *primaryKeyProperty () const Q_DECL_OVERRIDE;
    virtual bool verifyPrimaryKey (PrimaryKey) const Q_DECL_OVERRIDE;
    
    QString name () const;
    void setName (QString);
    
    QString version () const;
    void setVersion (QString);
    
    QString url () const;
    void setUrl (QString);
    
  protected:
    QString m_key;
    QString m_name;
    QString m_version;
    QString m_url;
  };
}
