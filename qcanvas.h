#ifndef QCANVAS_H
#define QCANVAS_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QString>

class QCanvas : public QLabel
{
    Q_OBJECT
public:
    explicit QCanvas(QWidget *parent = 0);

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void leaveEvent(QEvent *event);
    QPoint coord_move;
    QPoint coord_click;

signals:
    void mouse_Press();
    void mouse_Left();
    void mouse_Pos();
public slots:
};

#endif // QCANVAS_H
