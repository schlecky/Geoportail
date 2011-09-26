#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QRect>
#include <QMouseEvent>
#include <QPixmap>
#include "tile.h"
#include "geoengine.h"
#include "constants.h"
#include "overlay.h"

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);
    void setCouche(Couche c);
    double getLongitude() {return geoEngine->convertToLongitude(center.x());}
    double getLatitude() {return geoEngine->convertToLatitude(center.y());}

signals:
    void coordChange(double longitude, double latitude);
    void mouseCoordChange(double, double);

public slots:
    void receiveData(QByteArray,int);
   // void startDownload();
    void updateMap();
    void goToLongLat(double longit, double latit);
    void downloadSelection(int zoomLevel);  //telecharge la zone selectionnee au niveau de zoom 'zoomLevel'

private:
    //QList<Tile*> tiles;
    GeoEngine* geoEngine;

    QPoint center;      // les coordonnes du centre dans le referentiel carte
    int zoomLevel;
    Couche couche;
    QRect tilesRect;    // le rectangles des tuiles en numeros

    QPoint convertScreenToMapNum(QPoint pos);
    QPoint convertScreenToMapXY(QPoint pos);
    QRect convertScreenToMapXY(QRect rect);
    QPoint convertMapToScreenXY(QPoint pos);

    Overlay* selectionOverlay;
    QList<Tile*> tiles;

    QMap<int,Tile*> downIds;
    QMap<int,QPoint> toSavePositions;

    QPixmap savedMap;

    bool moving;
    bool selecting;

    QPoint originalPos;
    QPoint startingPos;

    QPoint firstCorner, secondCorner; // selection
protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void resizeEvent(QResizeEvent *);
    virtual void wheelEvent(QWheelEvent *);
    virtual void paintEvent(QPaintEvent *);
};

#endif // MAPWIDGET_H
