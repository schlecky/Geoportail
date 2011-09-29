#ifndef GEOENGINE_H
#define GEOENGINE_H
#include <QWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkDiskCache>
#include <QUrl>
#include <QVector>
#include <QString>
#include <QQueue>
#include "constants.h"

typedef enum {
    CONNECTED,
    OFFLINE
} GeoEngineMode;

class GeoEngine : public QWidget
{
    Q_OBJECT
public:
    GeoEngine(GeoEngineMode mode=CONNECTED);
    void init(QString dir="");                  // si on est en mode offline il faut un repertoire
    QPoint convertLongLatToXY(double longi, double lati);
    double convertToLongitude(double x);
    double convertToLatitude(double y);
    QPoint convertXYToNumTile(QPoint xy, int zoomLevel);
    QPoint convertNumTileToXY(QPoint xy, int zoomLevel);
    QPoint convertPixToMapXY(QPoint pix,int zoomLevel);
    int downloadImage(Couche couche, int x, int y, int zoomLevel);      // telecharge une image et renvoie l'identification du téléchargement
    bool isInitialized(){return initialized;}
    void saveCachedTilesToDir(QString directory);
    TuileParams extractParamsFromFilename(QString filename);

signals :
        void dataReady(QByteArray data, int id);        // les données pour l'id sont pretes
        void ready();

private slots:
        void requestFinished(QNetworkReply*);
        void sendFirstData();

private:
    QUrl genereUrl(Couche couche, int x, int y, int zoomLevel);
    QString encryptNbBase(int nb, int base);
    int decryptNbBase(QString nb, int base);
    QString convertTileToDir(TuileParams params);
    QString locateFile(QString baseDir,QString filename);
    QNetworkAccessManager* manager;
    QNetworkDiskCache* cache;
    bool connected;
    int sendFile(QString file);
    QNetworkReply* initReply;
    QUrl _originalUrl;
    QUrl _urlRedirectedTo;

    QUrl redirectUrl(const QUrl& possibleRedirectUrl,
                     const QUrl& oldRedirectUrl) const;


    QVector<QString> csteCouche;
    QVector<QString> formatCouche;
    QVector<char>   my_tabencryptxy;
    QVector<char>   my_tabencryptnbr62x;
    QVector<char>   my_tabencryptsignes;

    QQueue<QByteArray> toSend;
    QQueue<int> toSendId;

    int compteur;

    QList<QNetworkReply*> imageRequests;
    QString tilesDir;
    GeoEngineMode mode;
    bool initialized;
};

#endif // GEOENGINE_H
