#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <algorithm>
#include <string>
#include <limits>
#include <fstream>
#include <iterator>
#include <stdexcept>
#include "../DataBase/Record.h"
#include "VSAM.h"
#include <QDebug>

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
    void on_Button_go_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    void twoHundredThousand();
    QVector <db::Record> records;

};

#endif // MAINWINDOW_H
