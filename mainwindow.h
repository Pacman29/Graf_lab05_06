#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QPainter>

#include "qcanvas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void Mouse_current_pos();
    void Mouse_click_pos();
    void Mouse_left();
    void Mouse_unpress();

    void on_Clear_btn_clicked();

private:
    Ui::MainWindow *ui;
    QPixmap *pix;
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
