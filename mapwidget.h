#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QList>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QString>
#include <QMouseEvent>
#include <QPixmap>
#include <QProgressBar>
#include "tile.h"
#include "geoengine.h"
#include "constants.h"
#include "overlay.h"
#include <QProgressBar>

class Overlay;
class GpxOverlay;

class MapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MapWidget(QWidget *parent = 0);
    void setCouche(Couche c);
    void setSelectionType(SelectionType s);
    double getLongitude() {return geoEngine->convertToLongitude(center.x());}
    double getLatitude() {return geoEngine->convertToLatitude(center.y());}
    int getZoomLevelMin() {return zoomLevelMin[couche];}

    GpxOverlay* getGpxOverlay(){return gpxOverlay;}
    QPoint convertScreenToMapNum(QPoint pos);
    QPoint convertScreenToMapXY(QPoint pos);
    QRect convertScreenToMapXY(QRect rect);
    QPoint convertMapToScreenXY(QPoint pos);
    QPoint convertLongLatToScreenXY(QPointF pos);

signals:
    void coordChange(double longitude, double latitude);
    void mouseCoordChange(double, double);
    void setDLEnabled(bool);
    void zoomChanged(int);
    void afficheDist(double);
public slots:
    void receiveData(QByteArray,int);
   // void startDownload();
    void engineReady();
    void updateMap();
    void goToLongLat(double longit, double latit);
    void goToLongLat(QPointF longLat);
    void goToAddress(QString address);
    void setZoomLevel(int zoomLevel);

    //télécharge la zone selectionnee au niveau de zoom 'zoomLevel'
    void downloadSelection(int zoomLevel, bool split=false, int maxWidth=768, int maxHeight=768, bool tilesonly=false);
    // télécharge la zone séléctionnée et crée un Atlas.
    void exportAtlas(int zoomLevel);
    void saveCache() {geoEngine->saveCachedTiles(&progressBar);}
    void setAutoSave(bool a){if(geoEngine){geoEngine->setAutoSave(a);}}
    void setForceDL(bool force) {forceDL = force;}
    void setGeoEngineMode(GeoEngineMode mode) {if(geoEngine){geoEngine->setMode(mode);}}
    void receiveGeocode(QPointF geoCode);

    //charge une trace gpx
    void loadGPX();
    //supprime toutes les traces
    void removeTraces();


private:
    //QList<Tile*> tiles;
    GeoEngine* geoEngine;

    QPoint center;      // les coordonnes du centre dans le referentiel carte
    int zoomLevel;
    Couche couche;
    QRect tilesRect;    // le rectangles des tuiles en numeros

    void saveCalibrationToFile(QString filename,QRect mapXYRect, QSize mapSize);
    void hideTiles();
    void showTiles();

    Overlay* selectionOverlay;
    GpxOverlay * gpxOverlay;
    QList<Tile*> tiles;
    SelectionType selection;

    QMap<int,Tile*> downIds;
    QMap<int,QPoint> toSavePositions;
    //int nbTilesToDown;

    QProgressBar progressBar;

    QVector<QPixmap> savedMap;
    QVector<QRect> mapRect;
    QVector<QPoint> mapPos;   //position de la petite partie de carte dans la carte globale
    int nbXMaps;
    int nbYMaps;

    bool moving;
    bool selecting;
    bool splitMaps;
    bool autoSave;

    int mapsMaxWidth;
    int mapsMaxHeight;

    bool downTilesOnly;
    bool forceDL;

    QPoint originalPos;
    QPoint startingPos;

    QRect downloadedXYRect;

    QPoint firstCorner, secondCorner; // selection
protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void resizeEvent(QResizeEvent *);
    virtual void wheelEvent(QWheelEvent *);
    //virtual void paintEvent(QPaintEvent *);
};

#endif // MAPWIDGET_H
