#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
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


    void on_Regiont_btn_clicked();

    void on_tabWidget_tabBarClicked(int index);

    void on_Razor_btn_clicked();

    void on_Rectangle_razor_rbt_clicked();

    void on_Polyhedron_razor_rbt_2_clicked(bool checked);

    void on_Regiont_btn_2_clicked();

    void on_Polyhedron_razor_rbt_clicked(bool checked);

    void on_Clear_region_btn_clicked();

    void on_Clear_region_btn_2_clicked();

private:
    Ui::MainWindow *ui;
    bool print_btn_pressed;
    bool first_click;
    bool region_draw;
    bool draw_line;
    QPointF pt1;
    QPointF pt2;

    QPointF start_poli;
    void resizeEvent(QResizeEvent *event);
    bool near(QPointF old, QPointF now);
};

#endif // MAINWINDOW_H
