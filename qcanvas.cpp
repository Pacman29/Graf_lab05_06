#include "qcanvas.h"

QCanvas::QCanvas(QWidget *parent) : QLabel(parent)
{
    coord_move.setX(0);
    coord_move.setY(0);
    coord_click.setX(0);
    coord_click.setY(0);
}

void QCanvas::mouseMoveEvent(QMouseEvent *event)
{
    coord_move.setX(event->x());
    coord_move.setY(event->y());
    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    emit mouse_Pos();
}

void QCanvas::mousePressEvent(QMouseEvent *event)
{
    coord_click.setX(event->x());
    coord_click.setY(event->y());
    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    this->setToolTipDuration(0);
    emit mouse_Press();
}

void QCanvas::leaveEvent(QEvent *event)
{
    emit mouse_Left();
}

