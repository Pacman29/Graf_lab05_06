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

    ui->Polyhedron_razor_rbt->setChecked(true);

    print_btn_pressed = false;
    first_click = false;
    region_draw = false;
    draw_line = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Mouse_current_pos()
{
    ui->statusBar->showMessage(QString("Координаты курсора %1 : %2 ").arg(ui->Canvas->coord_move.x()).arg(ui->Canvas->coord_move.y()));
    if((ui->tab_2->isVisible() || ui->tab_3->isVisible() ) && region_draw)
    {
        if(ui->Rectangle_razor_rbt->isChecked() && ui->Canvas->mouse_button_press())
        {
            ui->Canvas->Clear_canvas();
            QPointF pt1 (ui->Canvas->coord_move.x(),ui->Canvas->coord_click.y());
            QPointF pt2 (ui->Canvas->coord_click.x(),ui->Canvas->coord_move.y());
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt1,Qt::green);
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt2,Qt::green);
            ui->Canvas->Add_lines(pt1,ui->Canvas->coord_move,Qt::green);
            ui->Canvas->Add_lines(pt2,ui->Canvas->coord_move,Qt::green);

            return;
        }
        if(ui->Polyhedron_razor_rbt->isChecked() && first_click && !ui->Regiont_btn->isEnabled())
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::green);
            return;
        }
    }
    if(!print_btn_pressed)
    {
        if(ui->Draw_Lines->isChecked() && ui->Canvas->mouse_button_press())
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::black);
            if(ui->tab_2->isVisible())
            {
                pt1.setX(0);
                pt1.setY(0);
                pt2.setX(ui->Canvas->width());
                pt2.setY(ui->Canvas->height());
            }
        }
    }

    if(ui->Draw_Polygon->isChecked() && first_click)
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->draw_all_save_polihendron();
        ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::black);
    }

    if(ui->Draw_Pixel->isChecked() && ui->Canvas->mouse_button_press())
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->draw_all_save_polihendron();
        ui->Canvas->Add_lines(ui->Canvas->coord_move_old,ui->Canvas->coord_move,Qt::black);
        ui->Canvas->save_obj_line(ui->Canvas->coord_move_old,ui->Canvas->coord_move);
        ui->Canvas->coord_move_old = ui->Canvas->coord_move;
    }

}

void MainWindow::Mouse_click_pos()
{
    if((ui->tab_2->isVisible() || ui->tab_3->isVisible() )&& region_draw)
    {
        if(ui->Rectangle_razor_rbt->isChecked())
        {
            QPointF pt1 (ui->Canvas->coord_move.x(),ui->Canvas->coord_click.y());
            QPointF pt2 (ui->Canvas->coord_click.x(),ui->Canvas->coord_move.y());
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt1,Qt::green);
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt2,Qt::green);
            ui->Canvas->Add_lines(pt1,ui->Canvas->coord_move,Qt::green);
            ui->Canvas->Add_lines(pt2,ui->Canvas->coord_move,Qt::green);
            this->pt1 = ui->Canvas->coord_click;
            return;
        }
        if(ui->Polyhedron_razor_rbt->isChecked() && !ui->Regiont_btn->isEnabled())
        {
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
                    ui->Canvas->draw_all_save_polihendron();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,start_poli,Qt::green);
                    ui->Canvas->save_obj_polihendron(ui->Canvas->coord_click_old,start_poli);
                    ui->Regiont_btn->setEnabled(region_draw);
                    ui->Razor_btn->setEnabled(region_draw);
                    ui->Razor_box->setEnabled(region_draw);
                    ui->Draw_Gbox->setEnabled(true);
                    ui->tab->setEnabled(true);
                    region_draw = false;
                    ui->tabWidget->setEnabled(!region_draw);


                }
                else
                {
                    ui->Canvas->Clear_canvas();
                    ui->Canvas->draw_all_save_obj();
                    ui->Canvas->draw_all_save_polihendron();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,ui->Canvas->coord_move,Qt::green);
                    ui->Canvas->save_obj_polihendron(ui->Canvas->coord_click_old,ui->Canvas->coord_move);
                }
            }
            return;
        }
    }
    if(!print_btn_pressed)
    {
        ui->Canvas->coord_move_old = ui->Canvas->coord_click;
        if(ui->Draw_Lines->isChecked())
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::black);
            draw_line = true;

        }
        else if(!ui->Draw_Pixel->isChecked())
            if(!first_click)
            {
                first_click = true;
                start_poli = ui->Canvas->coord_click;
                ui->Canvas->coord_click_old = ui->Canvas->coord_click;
                ui->tabWidget->setEnabled(!first_click);
            }
            else
            {
                if (near(start_poli,ui->Canvas->coord_move))
                {
                    first_click = false;
                    ui->Canvas->Clear_canvas();
                    ui->Canvas->draw_all_save_obj();
                    ui->Canvas->draw_all_save_polihendron();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,start_poli,Qt::black);
                    ui->Canvas->save_obj_line(ui->Canvas->coord_click_old,start_poli);
                    ui->tabWidget->setEnabled(!first_click);
                }
                else
                {
                    ui->Canvas->Clear_canvas();
                    ui->Canvas->draw_all_save_obj();
                    ui->Canvas->draw_all_save_polihendron();
                    ui->Canvas->Add_lines(ui->Canvas->coord_click_old,ui->Canvas->coord_move,Qt::black);
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
    if((ui->tab_2->isVisible() || ui->tab_3->isVisible() ) && region_draw)
    {
        if(ui->Rectangle_razor_rbt->isChecked())
        {
            ui->Canvas->Clear_canvas();
            QPointF pt1 (ui->Canvas->coord_move.x(),ui->Canvas->coord_click.y());
            QPointF pt2 (ui->Canvas->coord_click.x(),ui->Canvas->coord_move.y());
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt1,Qt::green);
            ui->Canvas->Add_lines(ui->Canvas->coord_click,pt2,Qt::green);
            ui->Canvas->Add_lines(pt1,ui->Canvas->coord_move,Qt::green);
            ui->Canvas->Add_lines(pt2,ui->Canvas->coord_move,Qt::green);
            ui->Regiont_btn->setEnabled(region_draw);
            ui->Razor_btn->setEnabled(region_draw);
            ui->Razor_box->setEnabled(region_draw);
            this->pt2 = ui->Canvas->coord_move;
            region_draw = false;
            return;
        }
        if(ui->Polyhedron_razor_rbt->isChecked() && !ui->Regiont_btn->isEnabled() && region_draw)
        {
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            ui->Canvas->draw_all_save_polihendron();
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::green);
            return;
        }
    }
    if(!print_btn_pressed)
    {
        if(ui->Draw_Lines->isChecked() && draw_line )
        {
            ui->Canvas->Add_lines(ui->Canvas->coord_click,ui->Canvas->coord_move,Qt::black);
            ui->Canvas->save_obj_line(ui->Canvas->coord_click,ui->Canvas->coord_move);
            draw_line = false;
        }
        else
        {

        }
        return;
    }
}


void MainWindow::on_Clear_btn_clicked()
{
    first_click = false;
    ui->Canvas->Clear_canvas();
    ui->Canvas->delete_all_save_obj();
    ui->Canvas->delete_Polyhedrons();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{   
    ui->Canvas->Clear_canvas();
    ui->Canvas->draw_all_save_obj();
    ui->Canvas->draw_all_save_polihendron();
}

bool MainWindow::near(QPointF old, QPointF now)
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



void MainWindow::on_Regiont_btn_clicked()
{
    if(ui->Rectangle_razor_rbt->isChecked())
    {
        ui->Razor_box->setEnabled(region_draw);
        ui->Regiont_btn->setEnabled(region_draw);
        ui->Razor_btn->setEnabled(region_draw);
        region_draw = true;
        return;
    }
    if(ui->Polyhedron_razor_rbt->isChecked())
    {
        ui->Canvas->delete_Polyhedrons();
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        region_draw = true;
        ui->Razor_box->setEnabled(false);
        ui->Razor_btn->setEnabled(false);
        ui->Regiont_btn->setEnabled(false);
        ui->Draw_Gbox->setEnabled(false);
        ui->tab->setEnabled(false);
        ui->tabWidget->setEnabled(!region_draw);
        return;
    }
}

void MainWindow::on_tabWidget_tabBarClicked(int index)
{
    ui->Draw_Lines->setEnabled(true);
    ui->Draw_Pixel->setEnabled(true);
    ui->Sleep_box->setEnabled(true);
    ui->Canvas->Clear_canvas();
    ui->Canvas->delete_all_save_obj();
    ui->Canvas->delete_Polyhedrons();
    switch (index) {
    case 0:
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        break;
    }
    case 1:
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->delete_Polyhedrons();
        ui->Canvas->draw_all_save_polihendron();
        pt1.setX(0);
        pt1.setY(0);
        pt2.setX(ui->Canvas->width());
        pt2.setY(ui->Canvas->height());
        break;
    }
    case  2:
    {
        ui->Canvas->Clear_canvas();
        ui->Canvas->delete_all_save_obj();
        ui->Canvas->draw_all_save_obj();
        ui->Canvas->delete_Polyhedrons();
        ui->Canvas->draw_all_save_polihendron();
        pt1.setX(0);
        pt1.setY(0);
        pt2.setX(ui->Canvas->width());
        pt2.setY(ui->Canvas->height());
        ui->Polyhedron_razor_rbt_2->setChecked(true);

        ui->Draw_Lines->setEnabled(false);
        ui->Draw_Pixel->setEnabled(false);
        ui->Draw_Polygon->setChecked(true);
        ui->Sleep_box->setEnabled(false);
    }
    default:
        break;
    }
}

void MainWindow::on_Razor_btn_clicked()
{
    if(ui->Rectangle_razor_rbt->isChecked())
    {
        ui->Canvas->regular_razor(pt1,pt2,ui->Sleep_box->isChecked());
        ui->Canvas->Add_lines(pt1,QPointF(pt2.x(),pt1.y()),Qt::green);
        ui->Canvas->Add_lines(pt1,QPointF(pt1.x(),pt2.y()),Qt::green);
        ui->Canvas->Add_lines(QPointF(pt1.x(),pt2.y()),pt2,Qt::green);
        ui->Canvas->Add_lines(QPointF(pt2.x(),pt1.y()),pt2,Qt::green);
        return;
    }
    if(ui->Polyhedron_razor_rbt->isChecked())
    {
        if(!ui->Canvas->razor_Cyrus_Beck())
        {
            QMessageBox::warning(this,"Ошибка","Область отсекателя не является выпуклым многогранником");
            ui->Canvas->delete_Polyhedrons();
            ui->Canvas->Clear_canvas();
            ui->Canvas->draw_all_save_obj();
            return;
        }
        return;
    }
}

void MainWindow::on_Rectangle_razor_rbt_clicked()
{
    ui->Canvas->delete_Polyhedrons();
}



void MainWindow::on_Polyhedron_razor_rbt_2_clicked(bool checked)
{
    ui->Polyhedron_razor_rbt->setChecked(checked);
}

void MainWindow::on_Regiont_btn_2_clicked()
{
    emit ui->Regiont_btn->click();
}

void MainWindow::on_Polyhedron_razor_rbt_clicked(bool checked)
{
    ui->Regiont_btn_2->setChecked(checked);
}

void MainWindow::on_Clear_region_btn_clicked()
{
    ui->Canvas->delete_Polyhedrons();
    ui->Canvas->Clear_canvas();
    ui->Canvas->draw_all_save_obj();
}

void MainWindow::on_Clear_region_btn_2_clicked()
{
    emit ui->Clear_region_btn->click();
}

void MainWindow::on_Razor_btn_2_clicked()
{
    if(!ui->Canvas->isConvex())
    {
        QMessageBox::warning(this,"Ошибка","Область отсекателя не является выпуклым многогранником");
        ui->Canvas->delete_Polyhedrons();
        ui->Canvas->Clear_canvas();
        ui->Canvas->draw_all_save_obj();
        return;
    }
    if(!ui->Canvas->razor_Cazerlend_Hodzhmen())
    {
        QMessageBox::warning(this,"Ошибка","Нарисуйте многогранник");
    }
}
