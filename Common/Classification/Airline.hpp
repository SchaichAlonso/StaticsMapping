#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "IcaoRecord.hpp"

namespace Classification
{
  struct Airline : IcaoRecord
  {
    Q_OBJECT
    Q_PROPERTY (QString hubs    READ hubs     WRITE setHubs)
    Q_PROPERTY (QString hubsMissing READ hubsMissing WRITE setHubsMissing)
    Q_PROPERTY (QString comment READ comment  WRITE setComment)
    Q_PROPERTY (int     ceased  READ ceased   WRITE setCeased)
    Q_PROPERTY (int     founded READ founded  WRITE setFounded)
    Q_PROPERTY (QString parent  READ parent   WRITE setParent)
  public:
    enum PropertyName {
      IcaoProperty,
      NameProperty,
      HubsProperty,
      HubsMissingProperty,
      CommentProperty,
      CeasedProperty,
      FoundedProperty,
      ParentProperty
    };
    static const char *propertyByName (PropertyName);
    
    Airline (QString =QString(), QString =QString());
    Airline (Definitions *, const QJsonObject &);
    virtual ~Airline ();
    
    virtual QJsonObject toJson() const Q_DECL_OVERRIDE;
    virtual bool verifyPrimaryKey (PrimaryKey) const Q_DECL_OVERRIDE;
   
    static bool isFictiveIcaoCode (QString);
    bool isFictiveIcaoCode() const;
    
    QString allHubs () const;
    QStringList allHubsList () const;
    
    QString hubs () const;
    void setHubs (QString);
    
    QString hubsMissing () const;
    void setHubsMissing (QString);
    
    QStringList hubsList () const;
    void setHubsList (QStringList);
    
    QString comment () const;
    void setComment (QString);
    
    int ceased () const;
    void setCeased (int);
    
    int founded () const;
    void setFounded (int);
    
    QString parent () const;
    void setParent (QString);
    
  protected:
    virtual void fixReferences () Q_DECL_OVERRIDE;
    
  protected:
    static QStringList unique(QStringList);
    
    QStringList m_hubs, m_hubs_missing;
    QString m_comment, m_parent;
    int     m_founded, m_ceased;
  };
}
