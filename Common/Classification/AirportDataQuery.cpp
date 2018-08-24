#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>
#include <QtNetwork/QNetworkRequest>

#include "AirportDataQuery.hpp"


namespace Classification
{
  AirportDataQuery::AirportDataQuery(DefinitionsPointer definitions, QStringList outstanding)
  : QNetworkAccessManager()
  , m_definitions(definitions)
  , m_outstanding(outstanding)
  , m_insert_only(true)
  {
    connect (
      this, &QNetworkAccessManager::finished,
      this, &AirportDataQuery::handleReply
    );
  }
  
  void
  AirportDataQuery::request()
  {
    if (m_outstanding.empty()) {
      emit completed();
    } else {
      request(m_outstanding.takeFirst());
    }
  }
  
  void
  AirportDataQuery::request(QString icao)
  {
    if (m_insert_only) {
      if (m_definitions->airport(icao)) {
        emit skipped(icao);
        return request();
      }
    }
    qDebug("Requesting <%s>", qUtf8Printable(icao));
    
    QNetworkRequest request(QUrl(QString("https://api.flightplandatabase.com/nav/airport/%1").arg(icao)));
    request.setRawHeader("Accept", "application/json");
    get(request);
  }
  
  void
  AirportDataQuery::handleReply(QNetworkReply *__reply)
  {
    QSharedPointer<QNetworkReply> reply(__reply, deleteReply);
    QByteArray data(reply->readAll());
    
    QJsonParseError error;
    QJsonDocument doc(QJsonDocument::fromJson(data, &error));
    
    AirportPointer apt;
    
    if (error.error == QJsonParseError::NoError) {
      QJsonObject obj(doc.object());
      
      bool usable(obj["ICAO"].isString());
      usable &= obj["name"].isString();
      usable &= obj["lat"].isDouble();
      usable &= obj["lon"].isDouble();
      
      if (usable) {
        apt = AirportPointer(
          new Airport(
            obj["ICAO"].toString(),
            obj["name"].toString(),
            obj["lat"].toDouble(),
            obj["lon"].toDouble()
          )
        );
      }
    }
    
    if (apt) {
      m_definitions->upsert(apt);
      emit synced(apt->icao());
    } else {
      qCritical("Poor reply.");
      
      Q_FOREACH(QNetworkReply::RawHeaderPair p, reply->rawHeaderPairs()){
        qInfo("Header %s: %s", qUtf8Printable(p.first), qUtf8Printable(p.second));
      }
      
      Q_FOREACH(QByteArray b, data.split('\n')) {
        qInfo("%s", b.constData());
      }
      
      emit failed(reply->url().fileName());
    }
    
    qInfo("%s", doc.toJson().constData());
    
    request();
  }
  
  void AirportDataQuery::deleteReply(QNetworkReply* reply)
  {
    reply->deleteLater();
  }
} 
 
