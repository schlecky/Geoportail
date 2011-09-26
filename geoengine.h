#ifndef GEOENGINE_H
#define GEOENGINE_H
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkDiskCache>
#include <QUrl>
#include <QVector>
#include <QString>
#include "constants.h"

class GeoEngine : public QWidget
{
    Q_OBJECT
public:
    GeoEngine();
    void init();
    QPoint convertLongLatToXY(double longi, double lati);
    QPoint convertXYToNumTile(QPoint xy, int zoomLevel);
    QPoint convertNumTileToXY(QPoint xy, int zoomLevel);
    QPoint convertPixToMapXY(QPoint pix,int zoomLevel);
    int downloadImage(Couche couche, int x, int y, int zoomLevel);      // telecharge une image et renvoie l'identification du téléchargement
    bool isInitialized(){return initialized;}

signals :
        void dataReady(QByteArray data, int id);        // les données pour l'id sont pretes
        void ready();

private slots:
        void requestFinished(QNetworkReply*);

private:
    QUrl genereUrl(Couche couche, int x, int y, int zoomLevel);
    QString encryptNbBase(int nb, int base);
    QNetworkAccessManager* manager;
    QNetworkDiskCache* cache;
    bool connected;
    QNetworkReply* initReply;
    QUrl _originalUrl;
    QUrl _urlRedirectedTo;

    QUrl redirectUrl(const QUrl& possibleRedirectUrl,
                     const QUrl& oldRedirectUrl) const;


    QVector<QString> csteCouche;

    QList<QNetworkReply*> imageRequests;

    bool initialized;
};

#endif // GEOENGINE_H
