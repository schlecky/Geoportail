#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include "tile.h"
#include "geoengine.h"
#include "constants.h"

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);
    void setCouche(Couche c);

signals:

public slots:
    void receiveData(QByteArray,int);
    void startDownload();

private:
    //QList<Tile*> tiles;
    GeoEngine* geoEngine;

    QPoint center;      // les coordonnes du centre dans le referentiel carte
    int zoomLevel;
    Couche couche;
    QRect tilesRect;    // le rectangles des tuiles en numeros

    QPoint convertScreenToMapNum(QPoint pos);
    QPoint convertScreenToMapXY(QPoint pos);
    QPoint convertMapToScreenXY(QPoint pos);

    void updateMap();

    QList<Tile*> tiles;
    QMap<int,Tile*> downIds;


    bool moving;
    QPoint originalPos;
    QPoint startingPos;
protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void resizeEvent(QResizeEvent *);
    virtual void wheelEvent(QWheelEvent *);
};

#endif // MAPWIDGET_H
