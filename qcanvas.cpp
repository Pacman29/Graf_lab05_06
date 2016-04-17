#include "qcanvas.h"

QCanvas::QCanvas(QWidget *parent) : QLabel(parent)
{
    coord_move.setX(0);
    coord_move.setY(0);
    coord_click.setX(0);
    coord_click.setY(0);
    pix = new QPixmap(this->size());
    old = new QPixmap(this->size());
    Clear_canvas();
    pressed = false;

}

void QCanvas::mouseMoveEvent(QMouseEvent *event)
{
    coord_move.setX(event->x());
    coord_move.setY(event->y());
    if(add_line)
        if(pressed)
        {
            delete pix;
            pix = new QPixmap(*old);
            Add_lines(coord_click,coord_move);
        }

    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    emit mouse_Pos();
}

void QCanvas::mousePressEvent(QMouseEvent *event)
{

    if(add_line)
        if (!pressed)
        {
            delete old;
            old = new QPixmap(*pix);
            pressed = true;
            coord_click.setX(event->x());
            coord_click.setY(event->y());
        }
        else
        {
            coord_move.setX(event->x());
            coord_move.setY(event->y());
            delete pix;
            pix = new QPixmap(*old);
            Add_lines(coord_click,coord_move);
        }
    else
    {
        coord_click.setX(event->x());
        coord_click.setY(event->y());
    }
    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    this->setToolTipDuration(0);
    emit mouse_Press();
}

void QCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    if(add_line)
    {
        pressed = false;
        delete old;
        old = new QPixmap(*pix);
    }
}

void QCanvas::leaveEvent(QEvent *event)
{
    emit mouse_Left();
}

void QCanvas::Clear_canvas()
{
    delete pix;
    pix = new QPixmap(this->size());
    pix->fill(Qt::black);
    QPainter paint(pix);
    paint.setBrush(QBrush(Qt::white));
    paint.setPen(QPen(Qt::black));
    paint.drawRect(0,0+1,this->width()-3,this->height()-3);
    this->setPixmap(*pix);
}

void QCanvas::Add_lines(QPoint S, QPoint F)
{
    QPainter paint(pix);
    paint.drawLine(S,F);
    this->setPixmap(*pix);
}

