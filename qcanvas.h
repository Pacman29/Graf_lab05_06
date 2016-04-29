#ifndef QCANVAS_H
#define QCANVAS_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QDragMoveEvent>
#include <QEvent>
#include <QString>

#include <QPainter>
#include <QPixmap>

#include <QVector>

class QCanvas : public QLabel
{
    Q_OBJECT
    typedef struct line_type{
        QPointF S;
        QPointF F;
    }line_t;

public:
    explicit QCanvas(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    QPointF coord_move;
    QPointF coord_move_old;
    QPointF coord_click;
    QPointF coord_click_old;

    void Clear_canvas();
    void Add_lines(QPointF S,QPointF F, QColor color);
    void save_obj_polihendron(QPointF S,QPointF F);
    void save_obj_line(QPointF S,QPointF F);
    void draw_all_save_obj();
    void draw_all_save_polihendron();
    void delete_all_save_obj();
    void delete_Polyhedrons();

    bool isClosed();
    bool isConvex();

    void xor_with_line(QColor color, QColor background, bool time_sleep = false);
    void fill_algorithm(QPointF start, QColor color, QColor border, bool time_sleep = false);
    void regular_razor(QPointF pt1, QPointF pt2,bool time_sleep = false );
    bool razor_Cyrus_Beck(bool time_sleep = false);
    bool enabled_pix(QColor color, QPointF p);

    bool mouse_button_press();

private:
    QPixmap *pix;
    bool pressed;
    QVector<line_t> obj_lines;
    QVector<line_t> obj_Polyhedrons;
    QPointF get_min_x();
    QPointF get_max_x();
    int bit_code(QPointF pt1, QPointF pt2, QPointF search);
    void razor(QPointF A, QPointF B, QPointF pt1, QPointF pt2,bool time_sleep);
    void razor_CB(QPointF A, QPointF B, bool time_sleep);
    double length(QPointF A, QPointF B);
    double func(QPointF p1, QPointF p2, size_t y);
    double scalar(QPoint A, QPoint B);

signals:
    void mouse_pressevent();
    void mouse_leaveevent();
    void mouse_moveevent();
    void mouse_releaseevent();
public slots:
};

#endif // QCANVAS_H
