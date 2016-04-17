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

    connect(ui->Canvas,SIGNAL(mouse_Left()),this,SLOT(Mouse_left()));
    connect(ui->Canvas,SIGNAL(mouse_Pos()),this,SLOT(Mouse_current_pos()));
    connect(ui->Canvas,SIGNAL(mouse_Press()),this,SLOT(Mouse_click_pos()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mouse_current_pos()
{
    ui->statusBar->showMessage(QString("Координаты курсора %1 : %2 ").arg(ui->Canvas->coord_move.x()).arg(ui->Canvas->coord_move.y()));

}

void MainWindow::Mouse_click_pos()
{
    if(ui->Press_start_point->isChecked())
    {
        ui->Canvas->add_line = false;
        ui->X_box->setValue(ui->Canvas->coord_click.x());
        ui->Y_box->setValue(ui->Canvas->coord_click.y());
    }
    else
    {
        ui->Canvas->add_line = true;
    }
}

void MainWindow::Mouse_left()
{
}


void MainWindow::on_Clear_btn_clicked()
{
    ui->Canvas->Clear_canvas();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    on_Clear_btn_clicked();
    ui->X_box->setMaximum(pix->width());
    ui->Y_box->setMaximum(pix->height());
}
