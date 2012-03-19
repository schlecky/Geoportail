#ifndef OVERLAY_H
#define OVERLAY_H

#include "mapwidget.h"
#include <QWidget>
#include <QRect>
#include <QPolygon>
#include <constants.h>
#include <QListWidget>
#include <QDateTime>

class MapWidget;

typedef struct{
    QPointF coords;
    double elevation;
    QDateTime time;
} gpxPoint;

typedef QVector<gpxPoint> Trace ;


class GpxOverlay : public QWidget
{
    Q_OBJECT
    public:
        explicit GpxOverlay(MapWidget *parent = 0);
        void setListWidget(QListWidget* list = NULL);

    signals:
        void gotoLongLat(QPointF longLat);
    public slots :
        void loadTraceFromGPX(QString filename);
        void removeTraces();
        void showTrace(QListWidgetItem* trace);

    protected :
        virtual void paintEvent(QPaintEvent *);


    private :
        void drawDepart(QPolygon trace);
        QVector<Trace> traces;
        MapWidget* map;
        QListWidget* gpxList;
};



class Overlay : public QWidget
{
    Q_OBJECT
public:
    explicit Overlay(MapWidget *parent = 0);
    double dist();

public slots:
    void setSelection(QRect rect) {selection = rect.normalized();}
    QPolygon getSelection(){return selection;}
    void addPoint(QPoint point);
    void setSelectionType(SelectionType s) {selType = s;}
    void clear() {selection.clear();}
    void hideSelection(){selectionOn = false;}
    void showSelection(){selectionOn = true;}
    void showScale(){scaleOn = true;}
    void hideScale(){scaleOn = false;}
    void setScaleRatio(double ratio){scaleRatio = ratio;}

protected :
    virtual void paintEvent(QPaintEvent *);

private :
    QPolygon selection;
    SelectionType selType;
    bool selectionOn;
    bool scaleOn;
    double scaleRatio; //en m/pixel
    MapWidget* map;
    int scaleLength(); // calcule la longueur de l'échelle en pixels
    int scaleDist();   // la distance représentée par l'échelle en m
};

#endif // OVERLAY_H
