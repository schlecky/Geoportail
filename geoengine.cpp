#include "geoengine.h"
#include <math.h>
#include <QDebug>
#include <QDirIterator>
#include <QTimer>
#include <QDomDocument>
#include <QDir>

GeoEngine::GeoEngine(GeoEngineMode m)
{
    manager = new QNetworkAccessManager(this);
    cache = new QNetworkDiskCache();

    mode = m;
    originXY = QPoint(-20037508,20037508);
    tilesDir = QDir::currentPath()+QString("/tiles");

    //http://gpp3-wxs.ign.fr/tyujsdxmzox31ituc2uw0qwl/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&LAYER=CADASTRALPARCELS.PARCELS&STYLE=bdparcellaire&FORMAT=image/png&TILEMATRIXSET=PM&TILEMATRIX=10&TILEROW=354&TILECOL=531&extParamId=aHR0cDovL3d3dy5nZW9wb3J0YWlsLmdvdXYuZnIvZG9ubmVlcw==
    //GET /tyujsdxmzox31ituc2uw0qwl/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&LAYER=ORTHOIMAGERY.ORTHOPHOTOS&STYLE=normal&FORMAT=image/jpeg&TILEMATRIXSET=PM&TILEMATRIX=10&TILEROW=353&TILECOL=530&extParamId=aHR0cDovL3d3dy5nZW9wb3J0YWlsLmdvdXYuZnIvZG9ubmVlcw== HTTP/1.1
    //http://gpp3-wxs.ign.fr/tyujsdxmzox31ituc2uw0qwl/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&LAYER=GEOGRAPHICALGRIDSYSTEMS.MAPS&STYLE=normal&FORMAT=image/jpeg&TILEMATRIXSET=PM&TILEMATRIX=12&TILEROW=1424&TILECOL=2081


    // Les constantes pour les differents types d'images
    csteCouche.resize(10);
    formatCouche.resize(10);
    csteCouche[CARTE_IGN] = QString("GEOGRAPHICALGRIDSYSTEMS.MAPS");
    formatCouche[CARTE_IGN] = QString("image/jpeg");

    csteCouche[PHOTOS] = QString("ORTHOIMAGERY.ORTHOPHOTOS");
    formatCouche[PHOTOS] = QString("image/jpeg");

    //GET /tyujsdxmzox31ituc2uw0qwl/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile&LAYER=GEOGRAPHICALGRIDSYSTEMS.CASSINI&STYLE=normal&FORMAT=image/jpeg&TILEMATRIXSET=PM&TILEMATRIX=12&TILEROW=1407&TILECOL=2116&
    csteCouche[CASSINI] = QString("GEOGRAPHICALGRIDSYSTEMS.CASSINI");// Cassini
    formatCouche[CASSINI] = QString("image/jpeg");

    csteCouche[CADASTRE] = QString("CADASTRALPARCELS.PARCELS");
    formatCouche[CADASTRE] = QString("image/png");

    csteCouche[TEST] = QString("8fG");
    formatCouche[TEST] = QString(".png");

    my_tabencryptxy.resize(62);
    for(int i =0; i<62; i++)
        my_tabencryptxy[i] = tabencryptxy[i];

    my_tabencryptnbr62x.resize(25);
    for(int i=0; i<25;i++)
        my_tabencryptnbr62x[i] = tabencryptnbr62x[i];

    my_tabencryptsignes.resize(4);
    for(int i=0;i<4;i++)
        my_tabencryptsignes[i] = tabencryptsignes[i];

    initialized = false;
}

void GeoEngine::saveCachedTiles(QProgressBar* progressbar)
{
    QNetworkDiskCache* cache = (QNetworkDiskCache*)(manager->cache());
    QDirIterator it1(cache->cacheDirectory()+QString("http/"));
    int nbFiles = 0;
    while(it1.hasNext())
    {
        nbFiles++;
        it1.next();
    }

    QDirIterator it(cache->cacheDirectory()+QString("http/"));
    progressbar->setMaximum(nbFiles);
    progressbar->setMinimum(0);
    progressbar->show();
    progressbar->raise();
    int numFile=0;
    while(it.hasNext())
    {
        progressbar->setValue(numFile++);
        it.next();
        if(it.fileInfo().isFile())
        {
            QNetworkCacheMetaData metaData = cache->fileMetaData(it.filePath());
            QIODevice* data = cache->data(metaData.url());
            if(data)
            {
                saveTileToDisk(metaData.url(),data->readAll());
                delete data;
            }
        }
    }
     progressbar->hide();
}

void GeoEngine::saveTileToDisk(QUrl url, QByteArray dat)
{
    //ne sauve pas une image vide
    if(dat.size()==0)
        return;
    TuileParams params = extractParamsFromUrl(url.toString());
    QFileInfo fileinfo = convertTileToFileInfo(params);

    QDir dir(fileinfo.absolutePath());
    if(!dir.exists())
        dir.mkpath(dir.path());

    QFile cacheFile(fileinfo.filePath());
    cacheFile.open(QIODevice::WriteOnly);
    cacheFile.write(dat);
    cacheFile.close();

}

TuileParams GeoEngine::extractParamsFromUrl(QString filename)
{
    TuileParams params;
    QRegExp rx("http:.*LAYER=([^&]*).*TILEMATRIX=([^&]*)&TILEROW=([^&]*)&TILECOL=([^&]*)$");
    if(rx.exactMatch(filename))
    {
        if(rx.cap(1)== QString("GEOGRAPHICALGRIDSYSTEMS.MAPS"))
            params.couche = CARTE_IGN;
        else if(rx.cap(1)==QString("ORTHOIMAGERY.ORTHOPHOTOS"))
            params.couche = PHOTOS;
        else if(rx.cap(1)== QString("GEOGRAPHICALGRIDSYSTEMS.CASSINI"))
            params.couche = CASSINI;
        else if(rx.cap(1)==QString("CADASTRALPARCELS.PARCELS"))
            params.couche = CADASTRE;
        params.zoomlevel = rx.cap(2).toInt();
        params.y = rx.cap(3).toInt();
        params.x = rx.cap(4).toInt();
    }
    return params;
}

QFileInfo GeoEngine::convertTileToFileInfo(TuileParams params)
{
    QString path=tilesDir+QString("/");
    switch(params.couche)
    {
        case CARTE_IGN:
            path += QString("CARTE_IGN/");
            break;
        case PHOTOS:
            path+= QString("PHOTOS/");
            break;
        case CASSINI :
            path+= QString("CASSINI/");
            break;
        case CADASTRE :
            path+=QString("CADASTRE/");
            break;
        default :
            return QString("E");
    }
    path.append(QString("%1/").arg(QString().sprintf("%02d",params.zoomlevel)));
    path.append(QString("%1/").arg(QString().sprintf("%03d",int(params.x/100))));
    path.append(QString("%1/").arg(QString().sprintf("%03d",params.x%100)));
    path.append(QString("%1/").arg(QString().sprintf("%03d",int(params.y/100))));
    path.append(QString("img%1-%2.%3").arg(params.x).arg(params.y).arg(QString(formatCouche[params.couche]).remove(0,6)));
    return QFileInfo(path);
}

// projection de long/lat vers mercator
QPoint GeoEngine::convertLongLatToXY(double longi, double lati)
{
    int x = longi*pi/180*r;
    int y = log(tan((90 + lati) * pi / 360.0 )) * r;
    return QPoint(x,y);
}

// projection inverse mercator -> longlat
double GeoEngine::convertToLongitude(double x)
{
    return x/r*180/pi;
}

double GeoEngine::convertToLatitude(double x)
{
    double lat = x/r*180/pi;
    return 180/pi*(2*atan(exp(lat*pi/180))-pi/2);
}

QPoint GeoEngine::convertXYToNumTile(QPoint xy, int zoomLevel)
{
    return QPoint(floor((xy.x()-originXY.x())/resolutions[zoomLevel]/tileSize),
                  -ceil((xy.y()-originXY.y())/resolutions[zoomLevel]/tileSize));
}

QPoint GeoEngine::convertNumTileToXY(QPoint xy, int zoomLevel)
{
    return originXY+QPoint(floor(xy.x()*tileSize*resolutions[zoomLevel]),
                           -ceil(xy.y()*tileSize*resolutions[zoomLevel]));
}

QPoint GeoEngine::convertPixToMapXY(QPoint pix,int zoomLevel)
{
    return pix*resolutions[zoomLevel];
}

QPoint GeoEngine::convertMapXYToPix(QPoint xy,int zoomLevel)
{
    return xy/resolutions[zoomLevel];
}

void GeoEngine::init()
{
    switch(mode)
    {
        case CONNECTED:
            cache->setCacheDirectory(QString(".cache"));
            manager->setCache(cache);
            connect(manager,SIGNAL(finished(QNetworkReply*)),
                    this, SLOT(requestFinished(QNetworkReply*)));
            initialized = false;
            initReply = manager->get(QNetworkRequest(QUrl("http://www.geoportail.fr")));
            qDebug()<<"Mode connecté";
            break;
         case OFFLINE:
            qDebug()<<"Mode off-line";
            compteur = 0;
            break;
    }

}

QUrl GeoEngine::genereUrl(Couche couche, int x, int y, int zoomLevel)
{
    QString url("http://gpp3-wxs.ign.fr/tyujsdxmzox31ituc2uw0qwl/geoportail/wmts?SERVICE=WMTS&VERSION=1.0.0&REQUEST=GetTile");
    url.append(QString("&LAYER=%1").arg(csteCouche[couche]));
    url.append(QString("&STYLE=normal"));
    url.append(QString("&FORMAT=%1").arg(formatCouche[couche]));
    url.append(QString("&TILEMATRIXSET=PM"));
    url.append(QString("&TILEMATRIX=%1").arg(zoomLevel));
    url.append(QString("&TILEROW=%1").arg(y));
    url.append(QString("&TILECOL=%1").arg(x));
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
        if((reply == initReply) && !initialized)
        {
            initialized = true;
            emit(ready());
        }
        if(imageRequests.contains(reply))
        {
            QByteArray tileData = reply->readAll();
            if(autoSave)
            {
                saveTileToDisk(reply->url(),tileData);
            }
            emit(dataReady(tileData,int(reply)));
        }
        if(reply == geocodeReply)
        {
            QByteArray temp = reply->readAll();
            QDomDocument kmlDoc;
            kmlDoc.setContent(temp);
            QDomElement n = kmlDoc.documentElement().firstChildElement(QString("Response")).
                    firstChildElement(QString("Placemark")).
                    firstChildElement(QString("Point")).firstChildElement(QString("coordinates"));
            QString coordStr = n.firstChild().nodeValue();
            QStringList coordsStr = coordStr.split(',');
            if(coordsStr.size()>1)
            {
                QPointF coords = QPointF(coordsStr.at(0).toFloat(),coordsStr.at(1).toFloat());
                emit(geocodeReceived(coords));
            }
        }
    }
    /* Clean up. */
    reply->deleteLater();

}

int GeoEngine::downloadImage(Couche couche, int x, int y, int zoomLevel,bool forceDl)
{
    TuileParams params;
    params.couche=couche;
    params.x = x;
    params.y = y;
    params.zoomlevel = zoomLevel;
    QFileInfo info = convertTileToFileInfo(params);
    if(!info.exists() || ((mode==CONNECTED) && forceDl))
    {
        if(mode==OFFLINE)
            info = QFileInfo(QString("noImg.jpg"));
        else if (mode==CONNECTED)
        {
            QNetworkRequest request(genereUrl(couche, x, y, zoomLevel));
            request.setRawHeader("Referer", "http://www.geoportail.gouv.fr/swf/geoportal-visu-1.1.3.swf");
            request.setRawHeader("Host", "gpp3-wxs.ign.fr");
            request.setAttribute(QNetworkRequest::CacheLoadControlAttribute,
                                 QNetworkRequest::PreferCache);
            imageRequests.append(manager->get(request));
            return int(imageRequests.back());
        }
    }
    QFile fich(info.filePath());
    fich.open(QIODevice::ReadOnly);
    toSendId.enqueue(compteur++);
    toSend.enqueue(fich.readAll());
    fich.close();
    QTimer::singleShot(10,this,SLOT(sendFirstData()));
    return toSendId.back();
}

void GeoEngine::sendFirstData()
{
    emit(dataReady(toSend.dequeue(),toSendId.dequeue()));
}

void GeoEngine::getCoord(QString address)
{
    QNetworkRequest request(QString("http://maps.google.com/maps/geo?output=kml&q=%1").arg(address));
    geocodeReply = manager->get(request);
}
