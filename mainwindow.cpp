#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Canvas->setMouseTracking(true);

    on_Clear_btn_clicked();

    connect(ui->Canvas,SIGNAL(mouse_leaveevent()),this,SLOT(Mouse_left()));
    connect(ui->Canvas,SIGNAL(mouse_moveevent()),this,SLOT(Mouse_current_pos()));
    connect(ui->Canvas,SIGNAL(mouse_pressevent()),this,SLOT(Mouse_click_pos()));
    connect(ui->Canvas,SIGNAL(mouse_releaseevent()),this,SLOT(Mouse_unpress()));

    print_btn_pressed = false;
    first_click = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mouse_current_pos()
{
    ui->statusBar->showMessage(QString("Координаты курсора %1 : %2 ").arg(ui->Canvas->coord_move.x()).arg(ui->Canvas->coord_move.y()));
    if(!print_btn_pressed)
    {
        if(ui->Draw_Lines->isChecked() && ui->Canvas->mouse_button_press())
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
        }
    }

    if(ui->Draw_Polygon->isChecked() && first_click)
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
    }

    if(ui->Draw_Pixel->isChecked() && ui->Canvas->mouse_button_press())
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->Add_lines(ui->Canvas->coord_move_old,ui->Canvas->coord_move);
        ui->Canvas->save_obj_line(ui->Canvas->coord_move_old,ui->Canvas->coord_move);
        ui->Canvas->coord_move_old = ui->Canvas->coord_move;
    }

}

void MainWindow::Mouse_click_pos()
{
    if(!print_btn_pressed)
    {
        ui->Canvas->coord_move_old = ui->Canvas->coord_click;
        if(ui->Draw_Lines->isChecked())
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
        }
        else if(!ui->Draw_Pixel->isChecked())
            if(!first_click)
            {
                first_click = true;
                start_poli = ui->Canvas->coord_click;
                ui->Canvas->coord_click_old = ui->Canvas->coord_click;
            }
            else
            {
                if (near(start_poli,ui->Canvas->coord_move))
                {
                    first_click = false;
                    ui->Canvas->Clear_canvas();
                    ui->Canvas->draw_all_save_obj();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,start_poli);
                    ui->Canvas->save_obj_line(ui->Canvas->coord_click_old,start_poli);
                }
                else
                {
                    ui->Canvas->Clear_canvas();
                    ui->Canvas->draw_all_save_obj();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,ui->Canvas->coord_move);
                    ui->Canvas->save_obj_line(ui->Canvas->coord_click_old,ui->Canvas->coord_move);
                }
            }
    }
    else
    {
        ui->Canvas->fill_algorithm(ui->Canvas->coord_click,Qt::red,Qt::black,ui->Sleep_box->isChecked());
        on_Print_bth_clicked();
    }

}

void MainWindow::Mouse_left()
{
}

void MainWindow::Mouse_unpress()
{
    if(!print_btn_pressed)
    {
        if(ui->Draw_Lines->isChecked())
        {
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move);
            ui->Canvas->save_obj_line(ui->Canvas->coord_click,ui->Canvas->coord_move);
        }
        else
        {

        }
    }
}


void MainWindow::on_Clear_btn_clicked()
{
    first_click = false;
    ui->Canvas->delete_all_save_obj();
    ui->Canvas->Clear_canvas();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{   
    ui->Canvas->Clear_canvas();
    ui->Canvas->draw_all_save_obj();
}

bool MainWindow::near(QPoint old, QPoint now)
{

    if( old.x() - 3 <=  now.x() &&
        old.x() + 3 >=  now.x() &&
        old.y() - 3 <=  now.y() &&
        old.y() + 3 >=  now.y() )
        return true;
    else
        return false;
}

void MainWindow::on_Print_bth_clicked()
{

    if(ui->Fuse_rbt->isChecked())
    {
        ui->Fill_Gbox->setEnabled(print_btn_pressed);
        ui->Draw_Gbox->setEnabled(print_btn_pressed);
        ui->Clear_btn->setEnabled(print_btn_pressed);

        if(!print_btn_pressed)
        {
            print_btn_pressed = true;
            ui->Print_bth->setText("Отмена");
        }
        else
        {
            print_btn_pressed = false;
            ui->Print_bth->setText("Заполнить");
        }
    }
    else
    {
        ui->Canvas->xor_with_line(Qt::red,Qt::white,ui->Sleep_box->isChecked());
    }
}
