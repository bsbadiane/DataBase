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
#include "../DataBase/DataBase.h"

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
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

private:
    typedef db::VSAM<db::Record, db::RecordString, decltype(db::RecordString::string),
            &db::RecordString::string> StrVSAM;

    typedef db::VSAM<db::Record, db::RecordNumber, int,
            &db::RecordNumber::number> NumVSAM;

    Ui::MainWindow *ui;
    void twoHundredThousand();
    QVector <db::Record> records;

    NumVSAM vsamInt;
    StrVSAM vsamString;
    db::DataBase* base;

    QVector<QString> getIDs(const StrVSAM::ResultType&, const NumVSAM::ResultType&);
};

#endif // MAINWINDOW_H
