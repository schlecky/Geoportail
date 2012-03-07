#include "overlay.h"
#include <math.h>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDomDocument>

Overlay::Overlay(MapWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    scaleOn = true;
    scaleRatio = 0.5;
    map=parent;
    loadTraceFromGPX(QString("test.gpx"));
}

int Overlay::scaleDist()
{
    int dist = int(110*scaleRatio);
    dist = int(dist/pow(10,int(log(dist)/log(10))))*pow(10,int(log(dist)/log(10)));
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
      /*  if(selection.size()>0)
        {
            if(QLineF(QPointF(point),QPointF(selection.back())).length()>2)
                selection<<point;
        }
        else*/
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

    // draw traces
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(QBrush(Qt::blue),3));
    for(int i=0;i<traces.size();i++)
    {
        QPolygon trace;
        for(int j=0;j<traces[i].size();j++)
        {
            //qDebug()<<traces[i][j].x()<<" "<<traces[i][j].y();
            trace.append(map->convertLongLatToScreenXY(traces[i][j]));
        }
        painter.drawPolyline(trace);
    }
    painter.end();
}

void Overlay::loadTraceFromGPX(QString filename)
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
               // qDebug()<<attr.namedItem("lon").nodeValue().toDouble()<<" "<<
               //           attr.namedItem("lat").nodeValue().toDouble();
                trace.append(QPointF(attr.namedItem("lon").nodeValue().toDouble(),
                                     attr.namedItem("lat").nodeValue().toDouble()));
                trkpt = trkpt.nextSiblingElement("trkpt");
            }
            traces.append(trace);
        }
}

void Overlay::removeTraces()
{
    traces.clear();
}
