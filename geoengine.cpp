#include "geoengine.h"
#include <math.h>
#include <QDebug>
#include <QDirIterator>
#include <QTimer>
#include <QDomDocument>

GeoEngine::GeoEngine(GeoEngineMode m)
{
    manager = new QNetworkAccessManager(this);
    cache = new QNetworkDiskCache();

    mode = m;

    tilesDir = QString("tiles");

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

    my_tabencryptxy.resize(62);
    for(int i =0; i<62; i++)
        my_tabencryptxy[i] = tabencryptxy[i];

    my_tabencryptnbr62x.resize(25);
    for(int i=0; i<25;i++)
        my_tabencryptnbr62x[i] = tabencryptnbr62x[i];

    my_tabencryptsignes.resize(4);
    for(int i=0;i<4;i++)
        my_tabencryptsignes[i] = tabencryptsignes[i];
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
            //qDebug()<<it.fileName();
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
    QFileInfo info(url.toString());

    //qDebug()<<directory+QString("/")+info.fileName();
    QString relative = convertTileToDir(extractParamsFromFilename(info.fileName()));
    if(relative == QString("E"))
        return;
    QFileInfo fileinfo(locateFile(tilesDir,info.fileName()));
    QDir dir(fileinfo.absolutePath());
    if(!dir.exists())
        dir.mkpath(dir.path());
    if(!fileinfo.exists())
    {
        QFile cacheFile(fileinfo.filePath());
        cacheFile.open(QIODevice::WriteOnly);
        cacheFile.write(dat);
        cacheFile.close();
    }
}


QString GeoEngine::locateFile(QString baseDir, QString filename)
{
    return baseDir+QString("/")+convertTileToDir(extractParamsFromFilename(filename))+filename;
}

QString GeoEngine::convertTileToDir(TuileParams params)
{
    QString dir;
    switch(params.couche)
    {
        case CARTE_IGN:
            dir += QString("CARTE_IGN/");
            break;
        case PHOTOS:
            dir+= QString("PHOTOS/");
            break;
        case CASSINI :
            dir+= QString("CASSINI/");
            break;
        case CADASTRE :
            dir+=QString("CADASTRE/");
            break;
        default :
            return QString("E");
    }
    dir.append(QString("%1/").arg(QString().sprintf("%02d",params.zoomlevel)));
    dir.append(QString("%1/").arg(params.signes));
    dir.append(QString("%1/").arg(QString().sprintf("%03d",int(params.x/100))));
    dir.append(QString("%1/").arg(QString().sprintf("%03d",params.x%100)));
    dir.append(QString("%1/").arg(QString().sprintf("%03d",int(params.y/100))));
    return dir;
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
    return QPoint(floor(xy.x()*xRatios[zoomLevel-1]/tileSize),
                  floor(xy.y()*xRatios[zoomLevel-1]/tileSize));
}

QPoint GeoEngine::convertNumTileToXY(QPoint xy, int zoomLevel)
{
    return QPoint(int(xy.x()*tileSize/xRatios[zoomLevel-1]),
                  int((xy.y()+1)*tileSize/xRatios[zoomLevel-1]));
}

QPoint GeoEngine::convertPixToMapXY(QPoint pix,int zoomLevel)
{
    return pix/xRatios[zoomLevel-1];
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

int GeoEngine::decryptNbBase(QString nb, int base)
{
    int fact = 1;
    int result = 0;
    for(int i=nb.size()-1;i>=0;i--)
    {
        result+=my_tabencryptxy.indexOf(nb[i].toAscii())*fact;
        fact*=base;
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
            QByteArray tileData = reply->readAll();
            if(autoSave)
                saveTileToDisk(reply->url(),tileData);
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
            QPointF coords = QPointF(coordsStr.at(0).toFloat(),coordsStr.at(1).toFloat());
            emit(geocodeReceived(coords));
        }
    }
    /* Clean up. */
    reply->deleteLater();

}

//todo : rajouter une option pour forcer le téléchargement
int GeoEngine::downloadImage(Couche couche, int x, int y, int zoomLevel)
{
    QFileInfo info(genereUrl(couche, x, y, zoomLevel).toString());
    info = QFileInfo(locateFile(tilesDir,info.fileName()));
    if(!info.exists())
    {
        if(mode==OFFLINE)
            info = QFileInfo(QString("noImg.jpg"));
        else if (mode==CONNECTED)
        {
            QNetworkRequest request(genereUrl(couche, x, y, zoomLevel));
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

TuileParams GeoEngine::extractParamsFromFilename(QString filename)
{
    TuileParams params;
    filename.remove(QString("maps"));
    filename.remove(filename.size()-4,4); //remove extension
    params.couche = (Couche)csteCouche.indexOf(filename.left(3));
    filename.remove(0,3);
    params.zoomlevel = my_tabencryptnbr62x.indexOf(filename[0].toAscii());
    filename.remove(0,1);
    params.signes = my_tabencryptsignes.indexOf(filename[0].toAscii());
    filename.remove(0,1);
    int tailleX = my_tabencryptnbr62x.indexOf(filename[0].toAscii());
    filename.remove(0,1);
    params.x = decryptNbBase(filename.left(tailleX),62);
    filename.remove(0,tailleX);
    params.y = decryptNbBase(filename,62);
    return params;
}

void GeoEngine::getCoord(QString address)
{
    QNetworkRequest request(QString("http://maps.google.com/maps/geo?output=kml&q=%1").arg(address));
    geocodeReply = manager->get(request);
}

