#include "tile.h"
#include <QPainter>

Tile::Tile(QWidget *parent) :
    QLabel(parent)
{
    QPixmap pix;
    pix.load("noImg.jpg");
    resize(pix.size());
    setPixmap(pix);
    zoomLevel=0;
}

void Tile::loadPixmapFromData(QByteArray data, const char* format)
{
    QPixmap pix;
    pix.loadFromData(data, format);
    setPixmap(pix);
    resize(pix.size());
    repaint();
}
