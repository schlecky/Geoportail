#include "mapwidget.h"
#include "constants.h"
#include "geodl.h"
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QHash>
#include <QTimer>

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
    tilesRect = QRect();
    geoEngine = new GeoEngine(CONNECTED);
    connect(geoEngine,SIGNAL(dataReady(QByteArray,int)),
            this,SLOT(receiveData(QByteArray,int)));
    connect(geoEngine,SIGNAL(ready()),this,SLOT(engineReady()));
    connect(geoEngine,SIGNAL(geocodeReceived(QPointF)),this,SLOT(receiveGeocode(QPointF)));
    geoEngine->init();
    zoomLevel = 10;
    couche = CARTE_IGN;
    moving = false;
    setMouseTracking(true);
    selectionOverlay = new Overlay(this);
    gpxOverlay = new GpxOverlay(this);
    selectionOverlay->setScaleRatio(1/(factRatio*xRatios[zoomLevel-1]));

    progressBar.setParent(this);
    progressBar.show();
    progressBar.resize(width(),30);
    selection =  SEL_LIGNE;
    selectionOverlay->setSelectionType(SEL_LIGNE);
    connect(gpxOverlay,SIGNAL(gotoLongLat(QPointF)),this,SLOT(goToLongLat(QPointF)));
}


void MapWidget::engineReady()
{
    //Metz
    goToLongLat(6.17862,49.1133);
}

void MapWidget::goToLongLat(QPointF longLat)
{
    goToLongLat(longLat.x(),longLat.y());
}

void MapWidget::goToLongLat(double longitude, double latitude)
{
    center = geoEngine->convertLongLatToXY(longitude,latitude);
    tilesRect = QRect();
    for(int i=0; i<tiles.size();i++)
    {
       Tile* tile=tiles.takeAt(i);
       downIds.remove(downIds.key(tile)); //on ne doit plus telecharger la tuile si c'est en cours
       delete tile;
    }
    emit(coordChange(longitude,latitude));
    if(geoEngine->isInitialized())
        updateMap();
}

QPoint MapWidget::convertScreenToMapXY(QPoint pos)
{
    QPoint temp =pos-rect().bottomRight()/2;
    temp.setY(-temp.y());
    return center + geoEngine->convertPixToMapXY(temp,zoomLevel);
}

QRect MapWidget::convertScreenToMapXY(QRect rect)
{
    return QRect(convertScreenToMapXY(rect.topLeft()),
                 convertScreenToMapXY(rect.bottomRight())).normalized();
}

QPoint MapWidget::convertScreenToMapNum(QPoint pos)
{
    return geoEngine->convertXYToNumTile(convertScreenToMapXY(pos),zoomLevel);
}

QPoint MapWidget::convertMapToScreenXY(QPoint pos)
{
    QPoint temp = (pos-center)*xRatios[zoomLevel-1];
    temp.setY(-temp.y());
    return rect().bottomRight()/2+temp;
}

QPoint MapWidget::convertLongLatToScreenXY(QPointF longLat)
{
    return convertMapToScreenXY(geoEngine->convertLongLatToXY(longLat.x(),longLat.y()));
}

void MapWidget::receiveData(QByteArray array, int id)
{
    if(downIds.contains(id))
    {
        downIds[id]->loadPixmapFromData(array);
        downIds[id]->show();
    }

    if(toSavePositions.contains(id))
    {
        if(!downTilesOnly)
        {
            QPainter painter(&savedMap[mapPos.indexOf(QPoint(toSavePositions[id].x()/mapsMaxWidth,
                                                             toSavePositions[id].y()/mapsMaxHeight))]);
            QPixmap temp;
            temp.loadFromData(array);
            painter.drawPixmap(QPoint(toSavePositions[id].x()%mapsMaxWidth,
                                      toSavePositions[id].y()%mapsMaxHeight),temp);
            painter.end();
        }
        toSavePositions.remove(id);
        progressBar.setValue(progressBar.maximum()-toSavePositions.size());

        if(toSavePositions.isEmpty())
        {
            if(!downTilesOnly)
            {
                QString filename =
                        QFileDialog::getSaveFileName(this,QString("Enregistrer sous"),
                                                     QString("maps"),QString("*.jpg"));
                if(!filename.isEmpty())
                {
                    if(!filename.endsWith(QString(".jpg")))
                        filename.append(".jpg");
                    int i;
                    for (i = 0; i< savedMap.size(); i++)
                    {
                         QString fname = filename;
                         fname.replace(QString(".jpg"),QString("")).append(QString("%1-%2.jpg").arg(mapPos[i].x()).arg(mapPos[i].y()));
                         savedMap[i].save(fname);
                         saveCalibrationToFile(fname.replace(QString(".jpg"),QString(".map")),
                                               mapRect[i],savedMap[i].size());
                    }
                }
            }
            progressBar.hide();
        }
    }
}




void MapWidget::wheelEvent(QWheelEvent * event)
{
    int zoom = zoomLevel - event->delta()/120;
    if(zoom<zoomLevelMin[couche])
        zoom=zoomLevelMin[couche];
    else
        setZoomLevel(zoom);
}

void MapWidget::setZoomLevel(int zoom)
{
    zoomLevel = zoom;
    tilesRect = QRect();
    while(tiles.size())
        delete tiles.takeFirst();
    downIds.clear();
    selectionOverlay->hideSelection();
    selectionOverlay->setScaleRatio(1/(factRatio*xRatios[zoomLevel-1]));
    emit(setDLEnabled(false));
    emit(zoomChanged(zoomLevel));
    updateMap();
}

void MapWidget::hideTiles()
{
    for(int i=0;i<tiles.size();i++)
    {
        tiles[i]->hide();
    }
}

void MapWidget::showTiles()
{
    for(int i=0;i<tiles.size();i++)
    {
        tiles[i]->show();
    }
}

void MapWidget::mousePressEvent ( QMouseEvent * event )
{
   switch(event->button())
   {
        case Qt::NoButton:
            break;
        case Qt::MiddleButton:
            break;
        case Qt::RightButton:
            moving = true;
            originalPos = event->pos();
            startingPos = event->pos();
            selectionOverlay->hideSelection();
            emit(setDLEnabled(false));
            gpxOverlay->hide();
            break;
        case Qt::LeftButton:
            selecting=true;
            selectionOverlay->clear();
            selectionOverlay->addPoint(event->pos());
            selectionOverlay->showSelection();
            if(selection == SEL_TELECHARGEMENT)
            {
                firstCorner = event->pos();
                emit(setDLEnabled(true));
            }
            break;
        default:
            break;
   }
}

void MapWidget::mouseMoveEvent ( QMouseEvent * event )
{
    QPoint xy = convertScreenToMapXY(event->pos());
    emit(mouseCoordChange(geoEngine->convertToLongitude(xy.x()),
                          geoEngine->convertToLatitude(xy.y())));

    if(moving)
    {
        for(int i=0;i<tiles.size();i++)
        {
            tiles[i]->move(tiles[i]->pos()+event->pos()-originalPos);
        }
        QPoint temp = -event->pos()+originalPos;
        temp.setY(-temp.y());
        center+=temp/xRatios[zoomLevel-1];
        originalPos = event->pos();
    }

    if(selecting)
    {
        secondCorner = event->pos();
        selectionOverlay->addPoint(event->pos());
        selectionOverlay->update();
        emit(afficheDist(selectionOverlay->dist()));
    }
}

void MapWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button()==Qt::RightButton)
    {
        moving = false;
        emit(coordChange(geoEngine->convertToLongitude(center.x()),
                         geoEngine->convertToLatitude(center.y())));
        gpxOverlay->show();
        //�vite que la version windows plante ?
        QTimer::singleShot(1,this,SLOT(updateMap()));
    }

    if(event->button()==Qt::LeftButton)
    {
        selecting = false;
        if(selectionOverlay->getSelection().size()<2)
            emit(setDLEnabled(false));
        repaint();
    }
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    tilesRect = QRect();
    updateMap();
    selectionOverlay->resize(event->size());
    gpxOverlay->resize(event->size());
    event->accept();
}

void MapWidget::setCouche(Couche c)
{
    if(c!=couche)
    {
        couche =c;
        tilesRect = QRect();
        while(tiles.size())
            delete tiles.takeFirst();
        downIds.clear();
        if(zoomLevel<zoomLevelMin[couche])
            zoomLevel=zoomLevelMin[couche];
        updateMap();
    }
}


void MapWidget::downloadSelection(int zoomLevel,bool split, int maxWidth, int maxHeight, bool tilesOnly)
{
    QPolygon sel = selectionOverlay->getSelection();
    QRect selectedXYRect = convertScreenToMapXY(QRect(sel[0],sel[1]));
    QRect selectedTilesRect = QRect(geoEngine->convertXYToNumTile(selectedXYRect.topLeft(),zoomLevel),
                                    geoEngine->convertXYToNumTile(selectedXYRect.bottomRight(),zoomLevel)).normalized();

    downloadedXYRect = QRect(geoEngine->convertNumTileToXY(selectedTilesRect.topLeft()+QPoint(0,-1),zoomLevel),
                             geoEngine->convertNumTileToXY(selectedTilesRect.bottomRight()+QPoint(1,0),zoomLevel)).normalized();

    int nx = (selectedTilesRect.width())*tileSize;  // largeur en pixels
    int ny = (selectedTilesRect.height())*tileSize; // hauteur en pixels
    int nbTilesToDown = selectedTilesRect.width()*selectedTilesRect.height();

    // nombre de tuiles entier pour chaque carte
    maxWidth = maxWidth - maxWidth%tileSize;
    maxHeight = maxHeight - maxHeight%tileSize;

    //Si on n'enregistre qu'une carte
    if(!split)
    {
        maxWidth = nx;  // la taille max est la taille de l'image
        maxHeight = ny;
    }

    QPoint dxy = geoEngine->convertPixToMapXY(QPoint(maxWidth,maxHeight),zoomLevel);
    int dx =  dxy.x(); //Largeur d'une partie de carte en coordonnées carte
    int dy =  dxy.y(); //Hauteur d'une partie de carte en coordonnées carte

    toSavePositions.clear();
    if(QMessageBox::question(this,tr("Telechargement"),
                             QString("Taille totale de l'image : %1x%2").arg(nx).arg(ny),
                             QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok)
    {
        progressBar.setMaximum(nbTilesToDown);
        progressBar.show();
        progressBar.setValue(0);
        progressBar.raise();

        splitMaps = split;
        mapsMaxWidth = maxWidth;
        mapsMaxHeight = maxHeight;
        savedMap.clear();
        mapRect.clear();
        mapPos.clear();
        downTilesOnly = tilesOnly;
        if(!tilesOnly)
        {
            // Prépare les cartes splitées
            for(int i=0;i<nx/maxWidth;i++)
                for(int j=0;j<ny/maxHeight;j++)
                {
                    savedMap.append(QPixmap(maxWidth,maxHeight));
                    mapRect.append(QRect(downloadedXYRect.topLeft()+QPoint(i*dx,downloadedXYRect.height()%dy+(ny/maxHeight-j-1)*dy),
                                         QSize(dx,dy)));
                    mapPos.append(QPoint(i,j));
                }
            if(nx%maxWidth!=0)
                for(int j=0;j<ny/maxHeight;j++)
                {
                    savedMap.append(QPixmap(nx%maxWidth,maxHeight));
                    mapRect.append(QRect(downloadedXYRect.topLeft()+QPoint(nx/maxWidth*dx,downloadedXYRect.height()%dy+(ny/maxHeight-j-1)*dy),
                                         QSize(downloadedXYRect.width()%dx,dy)));
                    mapPos.append(QPoint(nx/maxWidth,j));
                }
            if(ny%maxHeight!=0)
                for(int i=0;i<nx/maxWidth;i++)
                {
                    savedMap.append(QPixmap(maxWidth,ny%maxHeight));
                    mapRect.append(QRect(downloadedXYRect.bottomLeft()+QPoint(i*dx,0),
                                         QSize(dx,downloadedXYRect.height()%dy)));
                    mapPos.append(QPoint(i,ny/maxHeight));
                }
            if((ny%maxHeight!=0) && (nx%maxWidth!=0))
            {
                savedMap.append(QPixmap(nx%maxWidth,ny%maxHeight));
                mapRect.append(QRect(downloadedXYRect.topLeft()+QPoint(nx/maxWidth*dx,0),
                                     QSize(downloadedXYRect.width()%dx,downloadedXYRect.height()%dy)));
                mapPos.append(QPoint(nx/maxWidth,ny/maxHeight));
            }
        }
        for(int i=selectedTilesRect.topLeft().x();i<=selectedTilesRect.bottomRight().x();i++)
            for(int j=selectedTilesRect.topLeft().y();j<=selectedTilesRect.bottomRight().y();j++)
            {
                int id = geoEngine->downloadImage(CARTE_IGN,i,j,zoomLevel,forceDL);
                    toSavePositions.insert(id,QPoint(
                                               (i-selectedTilesRect.topLeft().x())*tileSize,
                                               ny-tileSize-(j-selectedTilesRect.topLeft().y())*tileSize)
                                           );
            }
    }
}


void MapWidget::saveCalibrationToFile(QString filename,QRect mapXYRect, QSize mapSize)
{
    double lat_i = geoEngine->convertToLatitude(mapXYRect.bottomRight().y());
    double lat_f = geoEngine->convertToLatitude(mapXYRect.topLeft().y());
    double long_i = geoEngine->convertToLongitude(mapXYRect.topLeft().x());
    double long_f = geoEngine->convertToLongitude(mapXYRect.bottomRight().x());
    int mapWidth = mapSize.width();
    int mapHeight = mapSize.height();

    QFile fichier(filename);
    if(fichier.open(QFile::WriteOnly))
    {
        QFileInfo fi(filename);
        QTextStream stream(&fichier);
        stream<< "OziExplorer Map Data File Version 2.2\n";
        stream<< "Carte geoportail\n";
        stream<< fi.fileName().replace(QString(".map"),QString(".jpg"))<<"\n";
        stream<< "1 ,Map Code,\n";
        stream<< "WGS 84,, 0.0000, 0.0000,WGS 84\n";
        stream<< "Reserved 1\n";
        stream<< "Reserved 2\n";
        stream<< "Magnetic Variation,,,E\n";
        stream<< "Map Projection,Mercator,PolyCal,No,AutoCalOnly,No,BSBUseWPX,No\n";
        stream<< "Point01,xy,0,0,in, deg,"<<int(lat_i)<<","<<(lat_i-int(lat_i))*60<<",N,"<<int(long_i)<<","<<(long_i-int(long_i))*60<<",E, grid,,,,N\n";
        stream<< "Point02,xy,"<<mapWidth<<",0,in, deg,"<<int(lat_i)<<","<<(lat_i-int(lat_i))*60 <<",N,"<<int(long_f)<<","<<(long_f-int(long_f))*60 <<",E, grid,,,,N\n";
        stream<< "Point03,xy,"<<mapWidth<<","<<mapHeight<<",in, deg,"<< int(lat_f)<<","<<(lat_f-int(lat_f))*60 <<",N,"<<int(long_f)<<","<<(long_f-int(long_f))*60 <<",E, grid,,,,N\n";
        stream<< "Point04,xy,0,"<<mapHeight<<",in, deg,"<< int(lat_f)<<","<<(lat_f-int(lat_f))*60 <<",N,"<<int(long_i)<<","<<(long_i-int(long_i))*60 <<",E, grid,,,,N\n";
        stream<< "Projection Setup,,,,,,,,,,\n";
        stream<< "Map Feature = MF ; Map Comment = MC     These follow if they exist\n";
        stream<< "Track File = TF      These follow if they exist\n";
        stream<< "Moving Map Parameters = MM?    These follow if they exist\n";
        stream<< "MM0,No\n";
        stream<< "MMPNUM,4\n";
        stream<< "MMPXY,1,0,0\n";
        stream<< "MMPXY,2,"<<mapWidth<<",0\n";
        stream<< "MMPXY,3,"<<mapWidth<<","<<mapHeight<<"\n";
        stream<< "MMPXY,4,0,"<<mapHeight<<"\n";
        stream<< "MMPLL,1, "<<long_i<<", "<<lat_i<<"\n";
        stream<< "MMPLL,2, "<<long_f<<", "<<lat_i<<"\n";
        stream<< "MMPLL,3, "<<long_f<<", "<<lat_f<<"\n";
        stream<< "MMPLL,4, "<<long_i<<", "<<lat_f<<"\n";
        stream<< "MOP,Map Open Position,0,0\n";
        stream<< "IWH,Map Image Width/Height,"<<mapWidth<<","<<mapHeight<<"\n";
        stream<< "MM1B,1.5559";
        fichier.close();
    }
}

void MapWidget::updateMap()
{
    QRect newTilesRect = QRect(convertScreenToMapNum(rect().bottomLeft()),convertScreenToMapNum(rect().topRight()));
    for(int i=newTilesRect.left();i<=newTilesRect.right();i++)
        for(int j=newTilesRect.top();j<=newTilesRect.bottom();j++)
        {
            if(!tilesRect.contains(QPoint(i,j)))
            {
                Tile* tempTile = new Tile(this);
                tempTile->setNum(QPoint(i,j));
                QPoint p = convertMapToScreenXY(geoEngine->convertNumTileToXY(tempTile->getNum(),zoomLevel));
                tempTile->move(p);
                downIds.insert(geoEngine->downloadImage(couche,tempTile->getNum().x(),
                                                        tempTile->getNum().y(),zoomLevel,forceDL),tempTile);
                tiles.append(tempTile);
            }
        }
    for(int i=0; i<tiles.size();)
    {
        if(!newTilesRect.contains(tiles[i]->getNum()))
        {
           Tile* tile=tiles.takeAt(i);
           downIds.remove(downIds.key(tile)); //on ne doit plus telecharger la tuile si c'est en cours
           delete tile;
        }
        else
            i++;
    }
    tilesRect = newTilesRect;
    if(selectionOverlay)
        selectionOverlay->raise();
    gpxOverlay->raise();
}

void MapWidget::setSelectionType(SelectionType s)
{
    selection = s;
    selectionOverlay->setSelectionType(s);
}

void MapWidget::receiveGeocode(QPointF geoCode)
{
    goToLongLat(geoCode.x(),geoCode.y());
}

void MapWidget::goToAddress(QString address)
{
    geoEngine->getCoord(address);
}

void MapWidget::exportAtlas(int zoomLevel)
{

}

void MapWidget::loadGPX()
{
    QString filename = QFileDialog::getOpenFileName(this,QString("Choisir un fichier"),
                                        QString("."),QString("*.gpx"));
    if(!filename.isEmpty())
        gpxOverlay->loadTraceFromGPX(filename);

}

void MapWidget::removeTraces()
{
    gpxOverlay->removeTraces();
    gpxOverlay->update();
}
