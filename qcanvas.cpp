#include "qcanvas.h"
#include <windows.h>
#include "cmath"
#include <QDebug>

#include <QTime>
#include <QCoreApplication>

QCanvas::QCanvas(QWidget *parent) : QLabel(parent)
{
    coord_move.setX(0);
    coord_move.setY(0);
    coord_click.setX(0);
    coord_click.setY(0);
    coord_click_old.setX(-1);
    coord_click_old.setY(-1);
    pix = new QPixmap(this->size());
    Clear_canvas();
    pressed = false;

}

void QCanvas::mouseMoveEvent(QMouseEvent *event)
{
    coord_move.setX(event->x());
    coord_move.setY(event->y());
    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    emit mouse_moveevent();
}

void QCanvas::mousePressEvent(QMouseEvent *event)
{
    pressed = true;
    coord_click_old = coord_click;
    coord_click.setX(event->x());
    coord_click.setY(event->y());
    this->setToolTip(QString("%1 : %2").arg( event->x()).arg( event->y()));
    this->setToolTipDuration(0);
    emit mouse_pressevent();
}

void QCanvas::mouseReleaseEvent(QMouseEvent *event)
{
    pressed = false;
    emit mouse_releaseevent();
}

void QCanvas::leaveEvent(QEvent *event)
{
    emit mouse_leaveevent();
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
    paint.setPen(QPen(QBrush(Qt::black),2));
    paint.drawLine(S,F);
    this->setPixmap(*pix);
}

void QCanvas::save_obj_line(QPoint S, QPoint F)
{
    line_t line;
    line.S = S;
    line.F = F;
    obj_lines.push_back(line);
}

void QCanvas::draw_all_save_obj()
{
    for(size_t i = 0; i<obj_lines.size();++i)
        Add_lines(obj_lines.value(i).S,obj_lines.value(i).F);
}

void QCanvas::delete_all_save_obj()
{
    obj_lines.clear();
}

void QCanvas::xor_with_line(QColor color,QColor background)
{
    if (obj_lines.isEmpty())
        return;
    size_t min_x =  get_min_x().x();
    size_t max_x =  get_max_x().x();

    size_t line_x = (min_x+max_x)/2;

    if( min_x == SIZE_MAX || max_x == SIZE_MAX)
        return;

    QImage im = pix->toImage();
    Clear_canvas();
    for(size_t i = 0; i<obj_lines.size(); ++i)
    {
        QPoint S = obj_lines.value(i).S;
        QPoint F = obj_lines.value(i).F;

        if(S.y() > F.y())
            std::swap(S,F);

        for(size_t j = S.y(); j<F.y(); ++j)
        {
            size_t x = func(S,F,j);
            size_t y = j;
            while(x < line_x)
            {
                if(enabled_pix(color,QPoint(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
                x++;
            }
            if(x == line_x)
            {
                if(enabled_pix(color,QPoint(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
            }
            while (x > line_x)
            {
                if(enabled_pix(color,QPoint(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
                x--;
            }

            pix->convertFromImage(im);
            this->setPixmap(*pix);
        }

        QTime dieTime= QTime::currentTime().addSecs(1);
        while (QTime::currentTime() < dieTime)
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
    }
    draw_all_save_obj();
}

bool QCanvas::enabled_pix(QColor color, QPoint p)
{
    QColor tmp(pix->toImage().pixel(p.x(),p.y()));
    if (tmp == color)
        return true;
    else
        return false;
}

QPoint QCanvas::get_min_x()
{
    if(obj_lines.isEmpty())
        return QPoint(SIZE_MAX,SIZE_MAX);
    line_t tmp;
    QPoint min = (obj_lines.value(0).S.x() < obj_lines.value(0).F.x()) ? (obj_lines.value(0).S):(obj_lines.value(0).F);
    for(size_t i = 0; i<obj_lines.size(); ++i)
    {
        tmp = obj_lines.value(i);
        if (tmp.S.x() < min.x())
            min = tmp.S;
        if (tmp.F.x() < min.x())
            min = tmp.F;
    }
    return min;
}

QPoint QCanvas::get_max_x()
{
    if(obj_lines.isEmpty())
        return QPoint(SIZE_MAX,SIZE_MAX);
    line_t tmp;
    QPoint max = (obj_lines.value(0).S.x() < obj_lines.value(0).F.x()) ? (obj_lines.value(0).S):(obj_lines.value(0).F);
    for(size_t i = 0; i<obj_lines.size(); ++i)
    {
        tmp = obj_lines.value(i);
        if (tmp.S.x() > max.x())
            max = tmp.S;
        if (tmp.F.x() > max.x())
            max = tmp.F;
    }
    return max;
}

double QCanvas::func(QPoint p1, QPoint p2, size_t y)
{
    if (p1.y() == p2.y())
        return SIZE_MAX;
    return  (y*(p2.x() - p1.x())+ p1.x()*p2.y()-p1.y()*p2.x())/(double)(p2.y()-p1.y());
//    return (p2.y() - p1.y())*(x-p1.x())/(p2.x()-p1.x())+p1.y();
}

bool QCanvas::mouse_button_press()
{
    return pressed;
}



