#include "overlay.h"
#include <math.h>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDomDocument>
#include <QFileInfo>
#include <QRadialGradient>

Overlay::Overlay(MapWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    scaleOn = true;
    scaleRatio = 0.5;
    map=parent;
}

int Overlay::scaleDist()
{
    int dist = floor(110*scaleRatio);
    int puissance = floor(pow(10,floor(log(dist)/log(10))));
    int nb = floor(dist/pow(10,floor(log(dist)/log(10))));
    dist = nb*puissance;
    return dist;
}

int Overlay::scaleLength()
{
    return int(scaleDist()/scaleRatio);
}

double Overlay::dist()
{
    double pixels =0;
    QPoint precedent;
    if(selection.size()>1)
    {
        precedent = selection[0];
        for(int i=1;i<selection.size();i++)
        {
            pixels+=QLineF(precedent,selection[i]).length();
            precedent = selection[i];
        }
    }
    return pixels*scaleRatio;
}

void GpxOverlay::setListWidget(QListWidget *list)
{
     gpxList = list;
     connect(gpxList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(showTrace(QListWidgetItem*)));
}

void Overlay::addPoint(QPoint point)
{
    switch(selType)
    {
    case SEL_TELECHARGEMENT:
    case SEL_LIGNE:
        if(selection.size()==2)
            selection.replace(1,point);
        else
            selection<<point;
        break;
    case SEL_CHEMIN:
            selection<<point;
        break;
    }
}


void Overlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(selectionOn)
    {
        //paint selection
        painter.setPen(Qt::red);
        if(selection.size()>1)
        switch(selType)
        {
        case SEL_TELECHARGEMENT:
            painter.drawRect(QRect(selection[0],selection[1]));
            break;
        case SEL_LIGNE:
            painter.drawLine(selection[0],selection[1]);
            break;
        case SEL_CHEMIN:
            painter.drawPolyline(selection);
            break;
        }
    }
    if(scaleOn)
    {
        //paint scale
        int length = scaleLength();
        int dist = scaleDist();
        painter.setBrush(QBrush(QColor(255,255,255,150)));
        painter.setPen(Qt::NoPen);
        painter.drawRect(QRect(this->width()-length-20,this->height()-30,
                               length+20,23));

        painter.setPen(QPen(QBrush(Qt::black),3));
        painter.drawLine(QPoint(this->width()-length-10,this->height()-10),
                         QPoint(this->width()-10,this->height()-10));
        painter.drawLine(QPoint(this->width()-length-10,this->height()-7),
                         QPoint(this->width()-length-10,this->height()-13));
        painter.drawLine(QPoint(this->width()-10,this->height()-7),
                         QPoint(this->width()-10,this->height()-13));
        painter.drawText(QRect(this->width()-length-10,this->height()-25,
                               length,15), QString("%1 m").arg(dist),
                         QTextOption(Qt::AlignHCenter));
    }

    painter.end();
}






GpxOverlay::GpxOverlay(MapWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    map=parent;
}


void GpxOverlay::loadTraceFromGPX(QString filename)
{
    QDomDocument gpx;
    QFile fichier(filename);
    if(!fichier.open(QIODevice::ReadOnly)) {
        qDebug() << tr("Failed to open: ") + filename;
        return;}
    if(!gpx.setContent(&fichier)) {
        fichier.close();
        qDebug() << tr("Failed to read: ") + filename;
        return;}
    fichier.close();

    const  QDomElement& docElem = gpx.documentElement();
    if(docElem.tagName() != "gpx") {
        qDebug() << tr("Not a GPX file: ") + filename;
        return;
    }

    const QDomNodeList& trks = gpx.elementsByTagName("trk");
        uint N = trks.count();
        for(uint n = 0; n < N; ++n) {
            const QDomNode& trk = trks.item(n);
            const QDomNode& trkseg = trk.namedItem("trkseg");
            QDomElement trkpt = trkseg.firstChildElement("trkpt");
            Trace trace;
            while (!trkpt.isNull()) {
                QDomNamedNodeMap attr = trkpt.attributes();
                gpxPoint pt;
                pt.coords = QPointF(attr.namedItem("lon").nodeValue().toDouble(),
                                             attr.namedItem("lat").nodeValue().toDouble());
                QDomElement ele= trkpt.firstChildElement("ele");
                pt.elevation = ele.firstChild().nodeValue().toDouble();
                QDomElement time= trkpt.firstChildElement("time");
                pt.time = QDateTime::fromString(time.firstChild().nodeValue(),"yyyy-MM-ddTHH:mm:ssZ");
                trace.append(pt);
                trkpt = trkpt.nextSiblingElement("trkpt");
            }
            traces.append(trace);
            if(gpxList)
            {
                QListWidgetItem* item = new QListWidgetItem(QFileInfo(filename).fileName(),gpxList);
                item->setData(32,traces.size()-1);
            }
            update();
        }
}

void GpxOverlay::showTrace(QListWidgetItem *trace)
{
    emit(gotoLongLat(traces[trace->data(32).toInt()][0].coords));
}


void GpxOverlay::removeTraces()
{
    traces.clear();
    if(gpxList)
        gpxList->clear();
}

void GpxOverlay::drawDepart(QPolygon trace)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint end = trace.back();
    QRadialGradient gradient;
    gradient.setFocalPoint(end);
    gradient.setCenter(end);
    gradient.setRadius(10);
    gradient.setColorAt(0,Qt::white);
    gradient.setColorAt(0.5,Qt::white);
    gradient.setColorAt(0.6,Qt::red);
    gradient.setColorAt(1,Qt::transparent);

    painter.setBrush(QBrush(gradient));
    painter.fillRect(rect(),gradient);

    painter.setPen(QPen(QColor(Qt::red),2));
    painter.setBrush(QBrush(Qt::red));
    QPolygon croix;
    croix.append(end);
    croix.append(end+QPoint(3,3));
    croix.append(end);
    croix.append(end+QPoint(-3,3));
    croix.append(end);
    croix.append(end+QPoint(-3,-3));
    croix.append(end);
    croix.append(end+QPoint(3,-3));
    painter.drawPolygon(croix);


    QPoint start = trace[0];
    gradient.stops().clear();
    gradient.setFocalPoint(start);
    gradient.setCenter(start);
    gradient.setRadius(10);
    gradient.setColorAt(0,Qt::white);
    gradient.setColorAt(0.5,Qt::white);
    gradient.setColorAt(0.6,Qt::darkGreen);
    gradient.setColorAt(1,Qt::transparent);

    painter.setBrush(QBrush(gradient));
    painter.fillRect(rect(),gradient);

    painter.setPen(QPen(QColor(Qt::darkGreen),1));
    painter.setBrush(QBrush(Qt::darkGreen));
    QPolygon fleche;
    fleche.append(start+QPoint(3,0));
    fleche.append(start+QPoint(-3,2));
    fleche.append(start+QPoint(-3,-2));
    painter.drawPolygon(fleche);

}

void GpxOverlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    // draw traces
    painter.setRenderHint(QPainter::Antialiasing);
    for(int i=0;i<traces.size();i++)
    {
        QPolygon trace;
        for(int j=0;j<traces[i].size();j++)
        {
            trace.append(map->convertLongLatToScreenXY(traces[i][j].coords));
        }
        painter.setPen(QPen(QColor(255,0,0,100),8));
        painter.drawPolyline(trace);
        painter.setPen(QPen(QColor(0,0,255,200),3));
        painter.drawPolyline(trace);

        if(trace.size()>1)
            drawDepart(trace);

    }
    painter.end();
}
