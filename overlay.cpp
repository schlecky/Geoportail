#include "overlay.h"
#include <QPainter>

Overlay::Overlay(QWidget *parent) :
    QWidget(parent)
{
    setPalette(Qt::transparent);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void Overlay::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setPen(Qt::red);
    painter.drawRect(selection);
    painter.end();
}
