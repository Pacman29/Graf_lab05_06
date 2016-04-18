#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Canvas->setMouseTracking(true);
    pix = new QPixmap(ui->Canvas->size());

    on_Clear_btn_clicked();

    connect(ui->Canvas,SIGNAL(mouse_leaveevent()),this,SLOT(Mouse_left()));
    connect(ui->Canvas,SIGNAL(mouse_moveevent()),this,SLOT(Mouse_current_pos()));
    connect(ui->Canvas,SIGNAL(mouse_pressevent()),this,SLOT(Mouse_click_pos()));
    connect(ui->Canvas,SIGNAL(mouse_releaseevent()),this,SLOT(Mouse_unpress()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mouse_current_pos()
{
    ui->statusBar->showMessage(QString("Координаты курсора %1 : %2 ").arg(ui->Canvas->coord_move.x()).arg(ui->Canvas->coord_move.y()));
    if(ui->Draw_Lines->isChecked() && ui->Canvas->mouse_button_press())
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
    }
}

void MainWindow::Mouse_click_pos()
{
    if(ui->Draw_Lines->isChecked())
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
    }
}

void MainWindow::Mouse_left()
{
}

void MainWindow::Mouse_unpress()
{
    if(ui->Draw_Lines->isChecked())
    {
        ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
        ui->Canvas->save_obj_line(ui->Canvas->coord_click,ui->Canvas->coord_move);
    }
}


void MainWindow::on_Clear_btn_clicked()
{
    ui->Canvas->delete_all_save_obj();
    ui->Canvas->Clear_canvas();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    on_Clear_btn_clicked();
    ui->X_box->setMaximum(pix->width());
    ui->Y_box->setMaximum(pix->height());
}
