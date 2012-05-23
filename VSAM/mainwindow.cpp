#include "mainwindow.h"
#include "ui_mainwindow.h"


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


typedef db::VSAM<db::Record, db::RecordString, decltype(db::RecordString::string),
        &db::RecordString::string> StrVSAM;

typedef db::VSAM<db::Record, db::RecordNumber, int,
        &db::RecordNumber::number> NumVSAM;

NumVSAM vsamInt;
StrVSAM vsamString;
db::DataBase* base;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->label->setText("HI. LAB BD");
    ui->spinBox->setValue(123456);
    ui->lineEdit_string->setText("Abramtsevo");
}

MainWindow::~MainWindow()
{
    delete ui;
}


////Потом в одном VSAM искать по строке, во втором по числу.
//Получится два множетсва найденных записей.
//Нужно найти их пересечение, а потом найти их по ID в базе из первой лабы и вывести все это н аэкран ввиде таблички.
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
void MainWindow::on_pushButton_2_clicked()
{
    QString town = ui->lineEdit_string->text();
    int number = ui->spinBox->value();
    char string[12];
    for (int i = 0; i < town.length(); i++)
    {
        string[i] = town[i];
    }

//    //поля
    StrVSAM::ResultType r1 = vsamString.findByKeyField(string);
    NumVSAM::ResultType r2 = vsamInt.findByKeyField(number);
    QVector <db::Record> intersection;
    for (int i = 0; i< r1.lenght;i++)
        for (int i1 = 0; i1< r2.lenght;i1++)
        {
            if (r1[i] == r2[i1]) {
                intersection.push_back(r1[i]);
                break;
            }
        }


    //    //получіть перересеченіе ІД
    //    //circle
    //    db::Record* r = base->searchByID("ID");
    //    r->
    QTableWidgetItem *newItem = new QTableWidgetItem;
    for (int i = 0;i < intersection.size(); i++)
    {
        newItem->setText(intersection[i].ID);
        ui->tableWidget->setItem(i, 0, newItem);
        newItem->setText(intersection[i].string);
        ui->tableWidget->setItem(i, 1, newItem);
        newItem->setText(intersection[i].number);
        ui->tableWidget->setItem(i, 2, newItem);
    }



//            //предусмотреть вывод номера управляюўего регіона і інтервала.

}
