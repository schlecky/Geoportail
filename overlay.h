#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWidget>
#include <QRect>

class Overlay : public QWidget
{
    Q_OBJECT
public:
    explicit Overlay(QWidget *parent = 0);

signals:

public slots:
    void setSelection(QRect rect) {selection = rect;}
protected :
    virtual void paintEvent(QPaintEvent *);

private :
    QRect selection;
};

#endif // OVERLAY_H
