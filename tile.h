#ifndef TILE_H
#define TILE_H

#include <QWidget>
#include <QLabel>
#include <QPoint>

class Tile : public QLabel
{
    Q_OBJECT
public:
    explicit Tile(QWidget *parent = 0);
    void loadPixmapFromData(QByteArray data, const char* format=0);
    void setX(int new_x){number.setX(new_x);}
    void setY(int new_y){number.setY(new_y);}
    void setNum(QPoint n) {number = n;}
    QPoint getNum() {return number;}

signals:

public slots:

protected:
    //void paintEvent (QPaintEvent * event);

private :
    QPoint number;
    int zoomLevel;

};

#endif // TILE_H
