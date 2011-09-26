#include "mapwidget.h"
#include "constants.h"
#include <QDebug>
#include <QPainter>

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
    geoEngine = new GeoEngine;
    connect(geoEngine,SIGNAL(dataReady(QByteArray,int)),this,SLOT(receiveData(QByteArray,int)));
    connect(geoEngine,SIGNAL(ready()),this,SLOT(updateMap()));
    geoEngine->init();
    // Vry : 6°19'42 49°11'22
    zoomLevel = 7;
    couche = CARTE_IGN;
    goToLongLat(6.32833,49.189444);
    moving = false;
    setMouseTracking(true);
    selectionOverlay = new Overlay(this);
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
}

void MapWidget::mousePressEvent ( QMouseEvent * event )
{
   switch(event->button())
   {
       case Qt::RightButton:
        moving = true;
        originalPos = event->pos();
        startingPos = event->pos();
        break;
       case Qt::LeftButton:
        selecting=true;
        firstCorner = event->pos();
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
    qDebug()<<zoomLevel;
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
        //qDebug()<<convertScreenToMapXY(event->pos());
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

void MapWidget::paintEvent(QPaintEvent *event)
{
    /*QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(QRect(firstCorner,secondCorner));
    painter.end();*/
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
