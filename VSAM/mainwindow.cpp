#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../DataBase/Hashers/CloserHasher.h"


/*
Надо проинициализировать два VSAMa. Одни для чисел, второй для строк. Потом занести туда отсортированные по ключу записи из файлов прошлой лабы. после этого по одному элементу заносить оставшиеся 200000.
Потом уже надо делать форму. В ней два поля ввода. Одно для числа, второе для строки, и кнопка "НАйти". Потом в одном VSAM искать по строке, во втором по числу. Получится два множетсва найденных записей. Нужно найти их пересечение, а потом найти их по ID в базе из первой лабы и вывести все это н аэкран ввиде таблички.*/



using namespace std;

namespace db {
    std::istream &operator>>(std::istream &is, db::Record &stan) {
        is.read((char*)&stan, sizeof(db::Record));
        return is;
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    base = NULL;
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->label->setText("HI. LAB BD");
    ui->lineEdit_int->setText("123456");
    ui->lineEdit_string->setText("Abramtsevo");
}

MainWindow::~MainWindow()
{
    delete base;
    delete ui;
}


////Потом в одном VSAM искать по строке, во втором по числу.
//Получится два множетсва найденных записей.
//Нужно найти их пересечение, а потом найти их по ID в базе из первой лабы и вывести все это н аэкран ввиде таблички.
void MainWindow::on_Button_go_clicked()
{
//    //поля
//    StrVSAM::ResultType r2 = vsamString.findByKeyField("town");
//    NumVSAM::ResultType r1 = vsamInt.findByKeyField(num);

//    //получіть перересеченіе ІД
//    //circle
//    db::Record* r = base->searchByID("ID");
//    r->

//            //предусмотреть вывод номера управляюўего регіона і інтервала.

}

void MainWindow::on_pushButton_clicked()
{
    ui->label->setText("WAIT.......");



    try {
        base = new db::DataBase("../db_result/base.db", 1200000, 10000, db::CloserHasher::build);
    } catch (std::exception* e) {
        qDebug() << e->what();
        throw;
    }


    //Надо проинициализировать два VSAMa. Одни для чисел, второй для строк.
    std::ifstream inputInt;
    inputInt.open("../db_result/radix.out.int", ios::in | ios::binary);

    istream_iterator<db::Record> firstInt(inputInt), lastInt;
    vsamInt.clearAndInsertRecords(firstInt, lastInt);

    std::ifstream inputString;
    inputString.open("../db_result/radix.out.string", ios::in | ios::binary);

    istream_iterator<db::Record> firstString(inputString), lastString;
    vsamString.clearAndInsertRecords(firstString, lastString);


    twoHundredThousand();
//    //Потом занести туда отсортированные по ключу записи из файлов прошлой лабы.

//    //после этого по одному элементу заносить оставшиеся 200000.
//    db::Record r = {"123456", "Abramtsevo", 1};
    for (int i = 0; i < 200000; ++i)
    {
        vsamInt.insertRecord(records[i]);
    }

    for (int i = 0; i < 200000; ++i)
    {
        vsamString.insertRecord(records[i]);
    }

    ui->stackedWidget->setCurrentIndex(1);
    //Потом уже надо делать форму. В ней два поля ввода. Одно для числа, второе для строки, и кнопка "НАйти".

}

//после этого по одному элементу заносить оставшиеся 200000.
void MainWindow::twoHundredThousand()
{
    FILE *f=NULL;
    //Record element;
    f = fopen("../db_result/city.base","rb");
    for (int i=0; i <800000;i++)
    {
        db::Record tempRecord;
        fread(&tempRecord,sizeof(db::Record),1,f);
    }
    for (int i=0; i <200000;i++)
    {
        db::Record tempRecord;
        fread(&tempRecord,sizeof(db::Record),1,f);
        //qDebug() << tempRecord.number;
        //stringtofloat(tempRecord.string);
        records.push_back(tempRecord);
    }

    fclose(f);
}
