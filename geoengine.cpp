#include "geoengine.h"
#include <math.h>
#include <QDebug>

GeoEngine::GeoEngine()
{
    manager = new QNetworkAccessManager(this);
    cache = new QNetworkDiskCache();
    cache->setCacheDirectory(QString(".cache"));
    manager->setCache(cache);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this, SLOT(requestFinished(QNetworkReply*)));

    // Les constantes pour les differents types d'images
    csteCouche.resize(10);
    formatCouche.resize(10);
    csteCouche[CARTE_IGN] = QString("8u6");
    formatCouche[CARTE_IGN] = QString(".jpg");

    csteCouche[PHOTOS] = QString("UxG");
    formatCouche[PHOTOS] = QString(".jpg");

    csteCouche[CASSINI] = QString("CkS");// Cassini
    formatCouche[CASSINI] = QString(".jpg");

    csteCouche[CADASTRE] = QString("8TX");
    formatCouche[CADASTRE] = QString(".png");

    csteCouche[TEST] = QString("8fG");
    formatCouche[TEST] = QString(".png");
    //qDebug()<<genereUrl(PHOTOS,3700,42717,4).toString();
}

QPoint GeoEngine::convertLongLatToXY(double longi, double lati)
{
    int x = int(longi*pi/180 * r * cos(phi0));
    int y = int(lati*pi/180  * r);
    return QPoint(x,y);
}

double GeoEngine::convertToLongitude(double x)
{
    return x/(r*cos(phi0))*180/pi;
}

double GeoEngine::convertToLatitude(double x)
{
    return x/r*180/pi;
}

QPoint GeoEngine::convertXYToNumTile(QPoint xy, int zoomLevel)
{
    return QPoint(int(xy.x()*xRatios[zoomLevel-1]/tileSize),int(xy.y()*xRatios[zoomLevel-1]/tileSize));
}

QPoint GeoEngine::convertNumTileToXY(QPoint xy, int zoomLevel)
{
    return QPoint(int(xy.x()*tileSize/xRatios[zoomLevel-1]),int((xy.y()+1)*tileSize/xRatios[zoomLevel-1]));
}

QPoint GeoEngine::convertPixToMapXY(QPoint pix,int zoomLevel)
{
    return pix/xRatios[zoomLevel-1];
}

void GeoEngine::init()
{
    initialized = false;
    initReply = manager->get(QNetworkRequest(QUrl("http://www.geoportail.fr")));
    qDebug()<<"Initialisation...";
}

QString GeoEngine::encryptNbBase(int nb, int base)
{
    QString result("");
    while(nb!=0)
    {
        result.prepend(tabencryptxy[nb%base]);
        nb = int(nb/base);
    }
    return result;
}


QUrl GeoEngine::genereUrl(Couche couche, int x, int y, int zoomLevel)
{
    // adresse de base
    QString url("http://visu-2d.geoportail.fr/geoweb/maps");
    // ajoute la couche
    url.append(csteCouche[couche]);
    url.append(tabencryptnbr62x[zoomLevel]);
    // gere les signes
    int nb=0;
    if(x<0)
    {
        nb+=2;
        x=-x;
    }
    if(y<0)
    {
        nb+=1;
        y=-y;
    }
    url.append(tabencryptsignes[nb]);
    QString xEncrypt = encryptNbBase(x,62);
    QString yEncrypt = encryptNbBase(y,62);
    // ajout la taille de la coordonnée x
    url.append(tabencryptnbr62x[xEncrypt.size()]);
    url.append(xEncrypt);
    url.append(yEncrypt);
    //url.append(".jpg");
    url.append(formatCouche[couche]);
    return QUrl(url);
}

QUrl GeoEngine::redirectUrl(const QUrl& possibleRedirectUrl,
                        const QUrl& oldRedirectUrl) const {
        QUrl redirectUrl;
        /*
         * Check if the URL is empty and
         * that we aren't being fooled into a infinite redirect loop.
         * We could also keep track of how many redirects we have been to
         * and set a limit to it, but we'll leave that to you.
         */
        if(!possibleRedirectUrl.isEmpty() &&
           possibleRedirectUrl != oldRedirectUrl) {
                redirectUrl = possibleRedirectUrl;
        }
        return redirectUrl;
}

void GeoEngine::requestFinished(QNetworkReply* reply)
{
    /*
     * Reply is finished!
     * We'll ask for the reply about the Redirection attribute
     * http://doc.trolltech.com/qnetworkrequest.html#Attribute-enum
     */
    QVariant possibleRedirectUrl =
             reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

    /* We'll deduct if the redirection is valid in the redirectUrl function */
    _urlRedirectedTo = this->redirectUrl(possibleRedirectUrl.toUrl(),
                                         _urlRedirectedTo);

    /* If the URL is not empty, we're being redirected. */
    if(!_urlRedirectedTo.isEmpty())
    {
           /* We'll do another request to the redirection url. */
        if(reply == initReply)
            initReply = manager->get(QNetworkRequest(_urlRedirectedTo));
        else

            manager->get(QNetworkRequest(_urlRedirectedTo));
    }
    else
    { /*
         * We weren't redirected anymore
         * so we arrived to the final destination...
         */
        /* ...so this can be cleared. */
        _urlRedirectedTo.clear();
        if(reply == initReply)
        {
            emit(ready());
            initialized = true;
        }
        if(imageRequests.contains(reply))
        {
            emit(dataReady(reply->readAll(),int(reply)));
        }
    }
    /* Clean up. */
    reply->deleteLater();

}

int GeoEngine::downloadImage(Couche couche, int x, int y, int zoomLevel)
{
    QNetworkRequest request(genereUrl(couche, x, y, zoomLevel));
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferCache);
    imageRequests.append(manager->get(request));
    return int(imageRequests.back());
}
