#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <Classification/Airport.hpp>
#include <Classification/Definitions.hpp>

namespace Classification
{
  struct AirportDataQuery : QNetworkAccessManager
  {
    Q_OBJECT
    
  public:
    AirportDataQuery(DefinitionsPointer definitions, QStringList outstanding);
    
    void request();
    
  signals:
    void completed();
    
    void synced(QString icao);
    void skipped(QString icao);
    void failed(QString icao);
    
  protected:
    void request(QString icao);
    void handleReply(QNetworkReply *reply);
    static void deleteReply(QNetworkReply *reply);
  
  protected:
    DefinitionsPointer m_definitions;
    
    QStringList m_outstanding;
    bool m_insert_only;
  };
} 
