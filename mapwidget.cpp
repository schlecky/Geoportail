#include "mapwidget.h"
#include "constants.h"
#include <QDebug>

MapWidget::MapWidget(QWidget *parent) :
    QWidget(parent)
{
    geoEngine = new GeoEngine;
    connect(geoEngine,SIGNAL(dataReady(QByteArray,int)),this,SLOT(receiveData(QByteArray,int)));
    connect(geoEngine,SIGNAL(ready()),this,SLOT(startDownload()));
    geoEngine->init();
    // Vry : 6°19'42 49°11'22
    zoomLevel = 7;
    couche = CARTE_IGN;
    center = geoEngine->convertLongLatToXY(6.32833,49.189444);
    moving = false;
}


void MapWidget::startDownload()
{
    tilesRect = QRect(convertScreenToMapNum(rect().bottomLeft()),convertScreenToMapNum(rect().topRight()));
    qDebug()<<tilesRect;
    for(int i=tilesRect.left();i<=tilesRect.right();i++)
        for(int j=tilesRect.top();j<=tilesRect.bottom();j++)
        {
            Tile* tempTile = new Tile(this);
            tempTile->setNum(QPoint(i,j));
            QPoint p = convertMapToScreenXY(geoEngine->convertNumTileToXY(tempTile->getNum(),zoomLevel));
            tempTile->move(p);
            tempTile->show();
            qDebug()<<"ttt "<<tempTile->pos();
            downIds.insert(geoEngine->downloadImage(couche,tempTile->getNum().x(),tempTile->getNum().y(),zoomLevel),tempTile);
            tiles.append(tempTile);
        }
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
        downIds[id]->loadPixmapFromData(array,"JPG");
        downIds[id]->show();
    }
}

void MapWidget::mousePressEvent ( QMouseEvent * event )
{
   switch(event->button())
   {
       case Qt::LeftButton:
        moving = true;
        originalPos = event->pos();
        startingPos = event->pos();
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
    updateMap();
}

void MapWidget::mouseMoveEvent ( QMouseEvent * event )
{
    if(moving)
    {

        for(int i=0;i<tiles.size();i++)
        {
            tiles[i]->move(tiles[i]->pos()+event->pos()-originalPos);
        }
        originalPos = event->pos();
    }
}

void MapWidget::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button()==Qt::LeftButton)
    {
        moving = false;
        QPoint temp = -event->pos()+startingPos;
        temp.setY(-temp.y());
        center+=temp/xRatios[zoomLevel-1];
        updateMap();
    }

    if(event->button()==Qt::RightButton)
        qDebug()<<convertScreenToMapXY(event->pos());
}

void MapWidget::resizeEvent(QResizeEvent *)
{
    tilesRect = QRect();
    updateMap();
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
}
