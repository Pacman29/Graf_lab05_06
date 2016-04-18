#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QString>
#include <QMouseEvent>
#include <QKeyEvent>
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

    void on_Print_bth_clicked();


private:
    Ui::MainWindow *ui;
    bool print_btn_pressed;
    bool first_click;

    QPoint start_poli;
    void resizeEvent(QResizeEvent *event);
    bool near(QPoint old, QPoint now);
};

#endif // MAINWINDOW_H
