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
        QPoint S;
        QPoint F;
    }line_t;

public:
    explicit QCanvas(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    QPoint coord_move;
    QPoint coord_move_old;
    QPoint coord_click;
    QPoint coord_click_old;

    void Clear_canvas();
    void Add_lines(QPoint S,QPoint F);
    void save_obj_line(QPoint S,QPoint F);
    void draw_all_save_obj();
    void delete_all_save_obj();

    void xor_with_line(QColor color, QColor background, bool time_sleep);
    void fill_algorithm(QPoint start, QColor color, QColor border, bool time_sleep);
    bool enabled_pix(QColor color, QPoint p);

    bool mouse_button_press();

private:
    QPixmap *pix;
    bool pressed;
    QVector<line_t> obj_lines;
    QPoint get_min_x();
    QPoint get_max_x();

    double func(QPoint p1, QPoint p2, size_t y);

signals:
    void mouse_pressevent();
    void mouse_leaveevent();
    void mouse_moveevent();
    void mouse_releaseevent();
public slots:
};

#endif // QCANVAS_H
