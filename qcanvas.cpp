#include "qcanvas.h"

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

bool QCanvas::mouse_button_press()
{
    return pressed;
}



