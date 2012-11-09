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

typedef struct{
    QPointF coords;
    double rayon;
} Circle;

typedef QVector<gpxPoint> Trace ;

class Overlay : public QWidget
{
    Q_OBJECT
public :
    explicit Overlay(MapWidget *parent=0);
public slots:
    //setScaleRatio(double ratio){scaleRatio = ratio;}
protected :
    virtual void paintEvent(QPaintEvent*){}
    MapWidget* map;
private :

};


class ScaleOverlay : public Overlay
{
    Q_OBJECT
public :
    explicit ScaleOverlay(MapWidget *parent=0);

public slots :
    void showScale(){scaleOn = true;}
    void hideScale(){scaleOn = false;}

protected :
    virtual void paintEvent(QPaintEvent *);

private :
    bool scaleOn;
    int scaleLength(); // calcule la longueur de l'échelle en pixels
    int scaleDist();   // la distance représentée par l'échelle en m
};


class SelectionOverlay : public Overlay
{
    Q_OBJECT
public:
    explicit SelectionOverlay(MapWidget *parent = 0);
    double dist();

public slots:
    void setSelection(QRect rect) {selection = rect.normalized();}
    QPolygon getSelection(){return selection;}
    void addPoint(QPoint point);
    void setSelectionType(SelectionType s) {selType = s;}
    void clear() {selection.clear();}
    void hideSelection(){selectionOn = false;}
    void showSelection(){selectionOn = true;}
    //void setScaleRatio(double ratio){scaleRatio = ratio;}

protected :
    virtual void paintEvent(QPaintEvent *);

private :
    QPolygon selection;
    SelectionType selType;
    bool selectionOn;
};

class CircleOverlay : public Overlay
{
    Q_OBJECT
    public:
        explicit CircleOverlay(MapWidget *parent = 0);
    public slots:
        void addCircle(QPointF coords, double rayon);
        void clearCircles();
        void showCrosshair(){crosshair = true;update();}
        void hideCrosshair(){crosshair = false;update();}
        void toggleCrosshair(bool cross){crosshair = cross;update();}
    protected :
        virtual void paintEvent(QPaintEvent *);
    private:
        QPolygon circleToPolygon(Circle circ); // Convertit un cercle en polygon tracé à l'écran
        QVector<Circle> circles;
        bool crosshair;
};

class GpxOverlay : public Overlay
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
        void drawDepart(QPolygon trace, QPainter* painter);
        QVector<Trace> traces;
        MapWidget* map;
        QListWidget* gpxList;
};

#endif // OVERLAY_H
