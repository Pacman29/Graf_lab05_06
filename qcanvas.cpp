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

void QCanvas::Add_lines(QPointF S, QPointF F, QColor color)
{
    QPainter paint(pix);
    paint.setPen(QPen(QBrush(color),2));
    paint.drawLine(S,F);
    this->setPixmap(*pix);
}

void QCanvas::save_obj_polihendron(QPointF S, QPointF F)
{
    line_t line;
    line.S = S;
    line.F = F;
    obj_Polyhedrons.push_back(line);
}

void QCanvas::save_obj_line(QPointF S, QPointF F)
{
    line_t line;
    line.S = S;
    line.F = F;
    obj_lines.push_back(line);
}

void QCanvas::draw_all_save_obj()
{
    for(size_t i = 0; i<obj_lines.size();++i)
        Add_lines(obj_lines.value(i).S,obj_lines.value(i).F,Qt::black);
}

void QCanvas::draw_all_save_polihendron()
{
    for(size_t i = 0; i<obj_Polyhedrons.size();++i)
        Add_lines(obj_Polyhedrons.value(i).S,obj_Polyhedrons.value(i).F,Qt::green);
}

void QCanvas::delete_all_save_obj()
{
    obj_lines.clear();
}

void QCanvas::delete_Polyhedrons()
{
    obj_Polyhedrons.clear();
}

bool QCanvas::isClosed()
{
    return  (obj_Polyhedrons.size() == 0) ?
                (false) :  (obj_Polyhedrons.value(0).S == obj_Polyhedrons.value(obj_Polyhedrons.size()-1).F);
}

ssize_t QCanvas::isConvex()
{
    ssize_t z = 0;
    for(size_t i = 0; i < obj_Polyhedrons.size() ; ++i)
    {
        line_t A = obj_Polyhedrons.value(i);
        line_t B = obj_Polyhedrons.value((i+1) % obj_Polyhedrons.size());
        z+= (((A.F.x() - A.S.x())*(B.F.y() - B.S.y()) - (A.F.y() - A.S.y())*(B.F.x() - B.S.x())) < 0) ? (-1):(1);
    }

    return (fabs(z) == obj_Polyhedrons.size()) ? (z):(0);
}

bool QCanvas::Obj_is_empty()
{
    return obj_lines.isEmpty();
}

void QCanvas::xor_with_line(QColor color,QColor background,bool time_sleep)
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
        QPointF S = obj_lines.value(i).S;
        QPointF F = obj_lines.value(i).F;

        if(S.y() > F.y())
            std::swap(S,F);

        for(size_t j = S.y(); j<F.y(); ++j)
        {
            size_t x = func(S,F,j);
            size_t y = j;
            while(x < line_x)
            {
                if(enabled_pix(color,QPointF(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
                x++;
            }
            if(x == line_x)
            {
                if(enabled_pix(color,QPointF(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
            }
            while (x > line_x)
            {
                if(enabled_pix(color,QPointF(x,y)))
                    im.setPixel(x,y,background.rgb());
                else
                    im.setPixel(x,y,color.rgb());
                x--;
            }

            pix->convertFromImage(im);
            for(size_t j = 0; j<i+1; ++j)
                Add_lines(obj_lines.value(j).S,obj_lines.value(j).F,Qt::black);

            this->setPixmap(*pix);
        }
        if(time_sleep)
        {
            QTime dieTime= QTime::currentTime().addSecs(1);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
        }

    }
    draw_all_save_obj();
}

void QCanvas::fill_algorithm(QPointF start, QColor color,QColor border, bool time_sleep)
{
    QVector<QPointF> stack;
    QImage im(pix->toImage());

    im.setPixel(start.toPoint(),color.rgb());
    stack.push_back(start);


    while(!stack.isEmpty())
    {
        QPoint tmp_pix = stack.takeLast().toPoint();
        im.setPixel(tmp_pix,color.rgb());

        size_t tmp_x = tmp_pix.x();
        tmp_pix.setX(tmp_x+1);

        while(im.pixel(tmp_pix) != border.rgb())
        {
            im.setPixel(tmp_pix,color.rgb());
            tmp_pix.setX(tmp_pix.x()+1);
        }

        size_t x_r = tmp_pix.x() - 1;
        tmp_pix.setX(tmp_x-1);

        while(im.pixel(tmp_pix) != border.rgb())
        {
            im.setPixel(tmp_pix,color.rgb());
            tmp_pix.setX(tmp_pix.x()-1);
        }

        size_t x_l = tmp_pix.x() + 1;
        tmp_pix.setX(x_l);
        tmp_pix.setY(tmp_pix.y()+1);
        while(tmp_pix.x() <= x_r)
        {
            bool flag = false;
            while(im.pixel(tmp_pix) != border.rgb() &&
                  im.pixel(tmp_pix) != color.rgb() &&
                  tmp_pix.x() <= x_r)
            {
                if(!flag)
                    flag = true;
                tmp_pix.setX(tmp_pix.x()+1);
            }
            if(flag)
            {
                if (tmp_pix.x() == x_r &&
                    im.pixel(tmp_pix) != border.rgb() &&
                    im.pixel(tmp_pix) != color.rgb())
                    stack.push_back(tmp_pix);
                else
                    stack.push_back(QPointF(tmp_pix.x()-1,tmp_pix.y()));
                flag = false;
            }
            size_t x_st = tmp_pix.x();
            while(im.pixel(tmp_pix) == border.rgb() ||
                  im.pixel(tmp_pix) == color.rgb() &&
                  tmp_pix.x() <= x_r)
                tmp_pix.setX(tmp_pix.x()+1);
            if(tmp_pix.x() == x_st)
                tmp_pix.setX(tmp_pix.x()+1);
        }

        if(time_sleep)
        {

            pix->convertFromImage(im);
            this->setPixmap(*pix);
            QTime dieTime= QTime::currentTime().addMSecs(10);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        tmp_pix.setX(x_l);
        tmp_pix.setY(tmp_pix.y()-2);
       while(tmp_pix.x() <= x_r)
        {
            bool flag = false;
            while(im.pixel(tmp_pix) != border.rgb() &&
                  im.pixel(tmp_pix) != color.rgb() &&
                  tmp_pix.x() <= x_r)
            {
                if(!flag)
                    flag = true;
                tmp_pix.setX(tmp_pix.x()+1);
            }
            if(flag)
            {
                if (tmp_pix.x() == x_r &&
                    im.pixel(tmp_pix) != border.rgb() &&
                    im.pixel(tmp_pix) != color.rgb())
                    stack.push_back(tmp_pix);
                else
                    stack.push_back(QPointF(tmp_pix.x()-1,tmp_pix.y()));
                flag = false;
            }
            size_t x_st = tmp_pix.x();
            while(im.pixel(tmp_pix) == border.rgb() ||
                  im.pixel(tmp_pix) == color.rgb() &&
                  tmp_pix.x() <= x_r)
                tmp_pix.setX(tmp_pix.x()+1);
            if(tmp_pix.x() == x_st)
                tmp_pix.setX(tmp_pix.x()+1);
        }
        if(time_sleep)
        {

            pix->convertFromImage(im);
            this->setPixmap(*pix);
            QTime dieTime= QTime::currentTime().addMSecs(10);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
    }

    pix->convertFromImage(im);
    this->setPixmap(*pix);
}

void QCanvas::regular_razor(QPointF pt1, QPointF pt2,bool time_sleep)
{
    if(obj_lines.isEmpty())
        return;
    for(size_t i = 0; i<obj_lines.size(); ++i)
    {
        line_t line = obj_lines.value(i);
        razor(line.S,line.F,pt1,pt2,time_sleep);
    }
    this->setPixmap(*pix);
}

bool QCanvas::razor_Cyrus_Beck(bool time_sleep)
{
    if(!isConvex())
        return false;

    for(size_t i = 0; i< obj_lines.size(); ++i)
        razor_CB(obj_lines.value(i).S,obj_lines.value(i).F,time_sleep);

    return true;
}

bool QCanvas::razor_Cazerlend_Hodzhmen(bool time_sleep)
{

    if(obj_lines.isEmpty())
        return false;
    ssize_t direction_value;
    if(!(direction_value = isConvex()))
        return false;
    else
        if(direction_value < 0)
        {
            size_t size = obj_Polyhedrons.size();
            for(size_t i = 0; i< size/2. ; ++i)
            {
                line_t line_tmp_1 = obj_Polyhedrons.value(i);
                line_t line_tmp_2 = obj_Polyhedrons.value(size-1-i);
                std::swap(line_tmp_1.S,line_tmp_1.F);
                std::swap(line_tmp_2.S,line_tmp_2.F);
                obj_Polyhedrons.replace(i,line_tmp_2);
                obj_Polyhedrons.replace(size-1-i,line_tmp_1);
            }
        }


    QVector< QVector<QPointF> > arr_poli;

    QPointF start = obj_lines.first().S;
    QVector<QPointF> tmp;
    tmp.push_back(start);
    for(size_t i = 0; i < obj_lines.size(); ++i)
        if(obj_lines.value(i).F != tmp.first())
            tmp.push_back(obj_lines.value(i).F);
        else
        {
            arr_poli.push_back(tmp);
            tmp.clear();
            if(i+1 != obj_lines.size())
                tmp.push_back(obj_lines.value(i+1).S);
            else
                break;
        }

    for(size_t i = 0; i< arr_poli.size(); ++i)
    {
        QVector<QPointF> tmp = arr_poli.value(i);
        razor_CH(obj_Polyhedrons,tmp,false);
        print_poly(tmp);
    }
}

bool QCanvas::enabled_pix(QColor color, QPointF p)
{
    QColor tmp(pix->toImage().pixel(p.x(),p.y()));
    if (tmp == color)
        return true;
    else
        return false;
}

QPointF QCanvas::get_min_x()
{
    if(obj_lines.isEmpty())
        return QPointF(SIZE_MAX,SIZE_MAX);
    line_t tmp;
    QPointF min = (obj_lines.value(0).S.x() < obj_lines.value(0).F.x()) ? (obj_lines.value(0).S):(obj_lines.value(0).F);
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

QPointF QCanvas::get_max_x()
{
    if(obj_lines.isEmpty())
        return QPointF(SIZE_MAX,SIZE_MAX);
    line_t tmp;
    QPointF max = (obj_lines.value(0).S.x() < obj_lines.value(0).F.x()) ? (obj_lines.value(0).S):(obj_lines.value(0).F);
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

int QCanvas::bit_code(QPointF pt1, QPointF pt2, QPointF search)
{
    int res = 0;
    if(search.x() < pt1.x())
        res +=1;
    if(search.y() > pt2.y())
        res +=2;
    if(search.x() > pt2.x())
        res +=4;
    if(search.y() < pt1.y())
        res +=8;
    return res;
}

void QCanvas::razor(QPointF A, QPointF B, QPointF pt1, QPointF pt2, bool time_sleep)
{

    if(length(A,B) < sqrt(2))
        return;
    int bit_A = bit_code(pt1,pt2,A);
    int bit_B = bit_code(pt1,pt2,B);

    if(bit_A == bit_B && bit_A != 0 && bit_B != 0 )
        return;

    if( (bit_A==0 && bit_B==0) )
    {
        Add_lines(A,B,Qt::blue);
        if(time_sleep)
        {
            this->setPixmap(*pix);
            QTime dieTime= QTime::currentTime().addMSecs(100);
            while (QTime::currentTime() < dieTime)
                QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        }
        return;
    }

    QPointF half((A.x()+B.x())/2,(A.y()+B.y())/2);

    razor(A,half,pt1,pt2,time_sleep);
    razor(half,B,pt1,pt2,time_sleep);
}

void QCanvas::razor_CB(QPointF A, QPointF B, bool time_sleep)
{
    double t0 = 0;
    double t1 = 1;

    size_t size = obj_Polyhedrons.size();

    QPoint vec1(obj_Polyhedrons.value(0).F.x() - obj_Polyhedrons.value(0).S.x(),obj_Polyhedrons.value(0).F.y() - obj_Polyhedrons.value(0).S.y());
    QPoint vec2(obj_Polyhedrons.value(1).F.x() - obj_Polyhedrons.value(0).S.x(),obj_Polyhedrons.value(1).F.y() - obj_Polyhedrons.value(0).S.y());

    ssize_t koef = ((vec1.x()*vec2.y() - vec1.y()*vec2.x()) > 0) ? (1):(-1);
    QPoint D (B.x() - A.x(),B.y() - A.y());

    bool flag = 1;

    for(size_t i = 0; i<size; ++i)
    {
        double t = 0;

        QPoint w (A.x() - obj_Polyhedrons.value(i).F.x(), A.y() - obj_Polyhedrons.value(i).F.y());
        QPoint N (koef*(obj_Polyhedrons.value(i).S.y() - obj_Polyhedrons.value(i).F.y()),koef*(obj_Polyhedrons.value(i).F.x() - obj_Polyhedrons.value(i).S.x()));


        double Q = scalar(w,N);
        double P = scalar(D,N);

        if(P == 0)
        {
            if(Q<0)
            {
                flag = 0;
                break;
            }
        }
        else
        {
            t = - Q/P;

                if(P>0)
                {
                    if(t>1)
                    {
                        flag = 0;
                        break;
                    }
                    else
                        if(t>t0)
                            t0 = t;
                }
                else
                {
                    if(t<0)
                    {
                        flag = 0;
                        break;
                    }
                    else
                        if(t<t1)
                            t1 = t;
                }

        }
    }

    if (t0<=t1 && flag)
        Add_lines(QPointF( A.x()+(B.x()-A.x())*t0 , A.y()+(B.y()-A.y())*t0),
              QPointF( A.x()+(B.x()-A.x())*t1 , A.y()+(B.y()-A.y())*t1),
                  Qt::blue);
}

void QCanvas::razor_CH(QVector<QCanvas::line_t> razor, QVector<QPointF> &poly, bool time_sleep)
{
    for(size_t i = 0; i<razor.size(); ++i)
    {
        QVector<QPointF> tmp;
        for(size_t j = 0; j< poly.size(); ++j)
        {
            QPointF S = poly.value(j);
            QPointF F = poly.value((j+1) % poly.size());
            size_t flag = Intersection(S,F,razor.value(i).S,razor.value(i).F);
            while(flag)
            switch (flag) {
            case 1:
            {
                flag = 0;
                continue;
            }
            case 2:
            {
                QPointF add = LinesCross(S,F,razor.value(i).S,razor.value(i).F);
                if (add == F)
                {
                    flag = 3;
                    continue;
                }
                tmp.push_back(add);
                tmp.push_back(F);
                flag = 0;
                continue;
            }
            case 3:
            {
                tmp.push_back(F);
                flag = 0;
                continue;
            }
            case 4:
            {
                QPointF add = LinesCross(S,F,razor.value(i).S,razor.value(i).F);
                if (add == F)
                {
                    flag = 3;
                    continue;
                }
                tmp.push_back(add);
                flag = 0;
                continue;
            }
            }

        }
        poly.swap(tmp);
    }
}

double QCanvas::length(QPointF A, QPointF B)
{
    if (!(B.x()-A.x())  &&  !(B.y()-A.y()))
        return 0;
    return sqrt((B.x()-A.x())*(B.x()-A.x()) + (B.y()-A.y())*(B.y()-A.y()));
}

double QCanvas::func(QPointF p1, QPointF p2, size_t y)
{
    if (p1.y() == p2.y())
        return SIZE_MAX;
    return  (y*(p2.x() - p1.x())+ p1.x()*p2.y()-p1.y()*p2.x())/(double)(p2.y()-p1.y());
}

double QCanvas::scalar(QPoint A, QPoint B)
{
    return A.x()*B.x() + A.y()*B.y();
}

double QCanvas::vector(QPointF st1, QPointF end1, QPointF st2, QPointF end2)
{
    return (end1.x() - st1.x()) * (end2.y() - st2.y()) - (end2.x() - st2.x()) * (end1.y() - st1.y());
}

size_t QCanvas::Intersection(QPointF poly_0, QPointF poly_1, QPointF A_razor, QPointF B_razor)
{
     qreal rez1 = vector(A_razor,poly_0,A_razor,B_razor);
     qreal rez2 = vector(A_razor,poly_1,A_razor,B_razor);

     if(rez1 > 0)
     {
         if(rez2 > 0)
             return 1; // обе вершины вне
         else
             return 2; // пересечение из вне -> внутрь
     }
     else
     {
         if(rez2 > 0)
             return 4; // пересечение из внутри -> наружу
         else
             return 3; //обе вершины внутри
     }

}

QPointF QCanvas::LinesCross(QPointF poly_0, QPointF poly_1, QPointF A_razor, QPointF B_razor)
{
   qreal zn = (B_razor.y() - A_razor.y()) * (poly_1.x() - poly_0.x()) - (B_razor.x() - A_razor.x()) * (poly_1.y() - poly_0.y());
   qreal ch1 = (B_razor.x() - A_razor.x()) * (poly_0.y() - A_razor.y()) - (B_razor.y() - A_razor.y()) * (poly_0.x() - A_razor.x());
   qreal ch2 = (poly_1.x() - poly_0.x()) * (poly_0.y() - A_razor.y()) - (poly_1.y() - poly_0.y()) * (poly_0.x() - A_razor.x());

   if(!zn && (!ch1 || !ch2))
       return poly_1;

   ch1/=zn;

   return QPointF(poly_0.x() + ch1 * (poly_1.x() - poly_0.x()) , poly_0.y() + ch1 * (poly_1.y() - poly_0.y()));

}

void QCanvas::print_poly(QVector<QPointF>& poly)
{
    for(size_t i = 0; i<poly.size(); ++i)
    {
        Add_lines(poly.value(i),poly.value((i+1)%poly.size()),Qt::blue);
    }
}

bool QCanvas::mouse_button_press()
{
    return pressed;
}



