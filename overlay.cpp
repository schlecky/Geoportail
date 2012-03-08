#include "overlay.h"
#include <math.h>
#include <QPainter>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDomDocument>
#include <QFileInfo>

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

void Overlay::setListWidget(QListWidget *list)
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


void Overlay::drawDepart(QPoint start1, QPoint start2)
{
    QPainter painter(this);
    painter.setPen(QPen(QBrush(Qt::blue),3));
    painter.setBrush(QBrush(Qt::blue));
    QPolygon fleche;
    QPoint dir = start2-start1;
    double norm = sqrt(dir.x()*dir.x() + dir.y()*dir.y());
    dir = QPoint(floor(20*double(dir.x())/norm),floor(20*dir.y()/norm));
    QPoint perp;
    double alpha= double(dir.y())/double(dir.x());
    perp = QPoint(floor(5*(-alpha+sqrt(alpha*alpha+4*alpha))),
                  floor(5*(1-1/alpha*sqrt(alpha*alpha+4*alpha))));
    fleche.append(start1+perp);
    fleche.append(start1+dir);
    fleche.append(start1-perp);
    painter.drawConvexPolygon(fleche);
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
    for(int i=0;i<traces.size();i++)
    {
        QPolygon trace;
        for(int j=0;j<traces[i].size();j++)
        {
            trace.append(map->convertLongLatToScreenXY(traces[i][j]));
        }
        painter.setPen(QPen(QColor(255,0,0,100),8));
        painter.drawPolyline(trace);
        painter.setPen(QPen(QColor(0,0,255,200),3));
        painter.drawPolyline(trace);
       /*
        if(trace.size()>1)
            drawDepart(trace[0],trace[1]);
        */
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
                trace.append(QPointF(attr.namedItem("lon").nodeValue().toDouble(),
                                     attr.namedItem("lat").nodeValue().toDouble()));
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

void Overlay::showTrace(QListWidgetItem *trace)
{
    emit(gotoLongLat(traces[trace->data(32).toInt()][0]));
}


void Overlay::removeTraces()
{
    traces.clear();
    if(gpxList)
        gpxList->clear();
}
