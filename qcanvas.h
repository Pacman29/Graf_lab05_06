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
public:
    explicit QCanvas(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    QPoint coord_move;
    QPoint coord_click;

    void Clear_canvas();
    void Add_lines(QPoint S,QPoint F);

    bool add_line;
private:
    QPixmap *old;
    QPixmap *pix;
    bool pressed;

signals:
    void mouse_Press();
    void mouse_Left();
    void mouse_Pos();

public slots:
};

#endif // QCANVAS_H
