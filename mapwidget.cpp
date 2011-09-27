#include "mapwidget.h"
#include "constants.h"
#include <QDebug>
#include <QPainter>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QHash>

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
    tilesRect = QRect();
    geoEngine = new GeoEngine;
    connect(geoEngine,SIGNAL(dataReady(QByteArray,int)),this,SLOT(receiveData(QByteArray,int)));
    connect(geoEngine,SIGNAL(ready()),this,SLOT(updateMap()));
    geoEngine->init();
    // Vry : 6°19'42 49°11'22
    zoomLevel = 7;
    couche = CARTE_IGN;
    moving = false;
    setMouseTracking(true);
    selectionOverlay = new Overlay(this);

    progressBar.setParent(this);
    progressBar.show();
    progressBar.resize(width(),30);
    goToLongLat(6.32833,49.189444);
}

void MapWidget::goToLongLat(double longitude, double latitude)
{
    center = geoEngine->convertLongLatToXY(longitude,latitude);
    emit(coordChange(longitude,latitude));
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
    return QRect(convertScreenToMapXY(rect.topLeft()),convertScreenToMapXY(rect.bottomRight())).normalized();
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

void MapWidget::receiveData(QByteArray array, int id)
{
    if(downIds.contains(id))
    {
        downIds[id]->loadPixmapFromData(array);
        downIds[id]->show();
    }

    if(toSavePositions.contains(id))
    {
        //qDebug()<<toSavePositions[id];
        QPainter painter(&savedMap[mapPos.indexOf(QPoint(toSavePositions[id].x()/mapsMaxWidth,
                                                         toSavePositions[id].y()/mapsMaxHeight))]);
        //qDebug()<<QPoint(toSavePositions[id].x()/mapsMaxWidth,
        //                 toSavePositions[id].y()/mapsMaxHeight);
        //qDebug()<<mapPos.indexOf(QPoint(toSavePositions[id].x()/mapsMaxWidth,
        //                                toSavePositions[id].y()/mapsMaxHeight));
        QPixmap temp;
        temp.loadFromData(array);
        painter.drawPixmap(QPoint(toSavePositions[id].x()%mapsMaxWidth,toSavePositions[id].y()%mapsMaxHeight),temp);
        painter.end();
        toSavePositions.remove(id);
        progressBar.setValue(progressBar.maximum()-toSavePositions.size());
        //qDebug()<<progressBar.value()<<"/"<<progressBar.maximum();

        if(toSavePositions.isEmpty())
        {
            QString filename = QFileDialog::getSaveFileName();
            if(!filename.isEmpty())
            {
                int i;
                for (i = 0; i< savedMap.size(); i++)
                {
                     QString fname = filename;
                     fname.replace(QString(".jpg"),QString("")).append(QString("%1-%2.jpg").arg(mapPos[i].x()).arg(mapPos[i].y()));
                     savedMap[i].save(fname);
                     saveCalibrationToFile(fname.replace(QString(".jpg"),QString(".map")),mapRect[i],savedMap[i].size());
                }
            }
            progressBar.hide();
        }
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
            selectionOverlay->hide();
            emit(setDLEnabled(false));
            break;
        case Qt::LeftButton:
            selecting=true;
            firstCorner = event->pos();
            selectionOverlay->show();
            emit(setDLEnabled(true));
            break;
        default:
            break;
   }
}

void MapWidget::wheelEvent(QWheelEvent * event)
{
    zoomLevel -= event->delta()/120;
    if(zoomLevel<1)
        zoomLevel=1;
    tilesRect = QRect();
    while(tiles.size())
        delete tiles.takeFirst();
    downIds.clear();
    selectionOverlay->hide();
    emit(setDLEnabled(false));
    updateMap();
}

void MapWidget::mouseMoveEvent ( QMouseEvent * event )
{
    QPoint xy = convertScreenToMapXY(event->pos());
    emit(mouseCoordChange(geoEngine->convertToLongitude(xy.x()),geoEngine->convertToLatitude(xy.y())));

    if(moving)
    {
        for(int i=0;i<tiles.size();i++)
        {
            tiles[i]->move(tiles[i]->pos()+event->pos()-originalPos);
        }
        originalPos = event->pos();
    }

    if(selecting)
    {
        secondCorner = event->pos();
        selectionOverlay->setSelection(QRect(firstCorner,secondCorner));
        selectionOverlay->update();
    }
}

void MapWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button()==Qt::RightButton)
    {
        moving = false;
        QPoint temp = -event->pos()+startingPos;
        temp.setY(-temp.y());
        center+=temp/xRatios[zoomLevel-1];
        updateMap();
        emit(coordChange(geoEngine->convertToLongitude(center.x()),geoEngine->convertToLatitude(center.y())));
    }

    if(event->button()==Qt::LeftButton)
    {
        selecting = false;
        repaint();
    }
}

void MapWidget::resizeEvent(QResizeEvent *event)
{
    tilesRect = QRect();
    updateMap();
    selectionOverlay->resize(event->size());
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
        updateMap();
    }
}


void MapWidget::downloadSelection(int zoomLevel,bool split, int maxWidth, int maxHeight)
{
    QRect selectedXYRect = convertScreenToMapXY(selectionOverlay->getSelection());
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
    if(QMessageBox::question(this,tr("Telechargement"),QString("Taille totale de l'image : %1x%2").arg(nx).arg(ny),QMessageBox::Ok,QMessageBox::Cancel)==QMessageBox::Ok)
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

        for(int i=selectedTilesRect.topLeft().x();i<=selectedTilesRect.bottomRight().x();i++)
            for(int j=selectedTilesRect.topLeft().y();j<=selectedTilesRect.bottomRight().y();j++)
            {
                toSavePositions.insert(geoEngine->downloadImage(CARTE_IGN,i,j,zoomLevel),QPoint((i-selectedTilesRect.topLeft().x())*tileSize,
                                                                                                ny-tileSize-(j-selectedTilesRect.topLeft().y())*tileSize));
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
                downIds.insert(geoEngine->downloadImage(couche,tempTile->getNum().x(),tempTile->getNum().y(),zoomLevel),tempTile);
                tiles.append(tempTile);
            }
        }
    for(int i=0; i<tiles.size();i++)
    {
        if(!newTilesRect.contains(tiles[i]->getNum()))
        {
           Tile* tile=tiles.takeAt(i);
           downIds.remove(downIds.key(tile)); //on ne doit plus telecharger la tuile si c'est en cours
           delete tile;
        }
    }
    tilesRect = newTilesRect;
    if(selectionOverlay)
        selectionOverlay->raise();
}
