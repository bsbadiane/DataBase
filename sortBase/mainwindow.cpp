#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    bool a1,a2,a3,a4;
    a1 = ui->binarytree->isChecked();
    a2 = ui->radix->isChecked();
    a3 = ui->stringfield->isChecked();
    a4 = ui->intfield->isChecked();

    //Выбор методо сортировки и параметра
    if(a1==a2)
        ui->textEdit->setText("Не выбран тип сортировки.");
    else if(a3==a4)
        ui->textEdit->setText("Не выбрано информационное поле.");
    else {

        if (a1&&a3) {
            btree tree;
            ui->textEdit->setText(tree.start(TRUE));
        }
        else if (a1&&a4) {
            btree tree;
            ui->textEdit->setText(tree.start(FALSE));
        }
        else if (a2&&a3) {
            radix tree;
            ui->textEdit->setText(tree.start(TRUE));
        }
        else if (a2&&a4) {
            radix tree;
            ui->textEdit->setText(tree.start(FALSE));
        }
    }
}
