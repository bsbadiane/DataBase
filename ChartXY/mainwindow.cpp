//closer.out.over  closer.out.ro  numbersys.out.over  numbersys.out.ro

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
  {
  ui->setupUi(this);
  ui->spinBox->setMaximum(180001);
  ui->spinBox_first->setMaximum(120000);
  ui->spinBox->setValue(180000);

  grp1();


  connect(ui->actionGrp1,SIGNAL(triggered()),this,SLOT(grp1()));
  connect(ui->actionGrp2,SIGNAL(triggered()),this,SLOT(grp2()));
  connect(ui->actionGrp3,SIGNAL(triggered()),this,SLOT(grp3()));
  }

MainWindow::~MainWindow()
  {
  delete ui;
  }



void MainWindow::grp1()
{
    num1 = 1;
    qDebug() <<  "grp1" ;
    setupModel("./Closer/out.over","./Closer/out.ro");

    setupChart();

    setWindowTitle("BD процентное отношение записей, попавших в область переполнения к общему числу записей");
    ui->textBrowser->setText("Метод преобразования системы счисления : BD процентное отношение записей, попавших в область переполнения к общему числу записей");
    ui->textBrowser_2->setText("Метод складывания : BD процентное отношение записей, попавших в область переполнения к общему числу записей");

}

void MainWindow::grp2()
{
    num1 = 2;
    qDebug() <<  "grp2" ;
    setupModel("./Closer/out.ro","./NumberSystem/out.ro");
    setupChart();
    setWindowTitle("Плотность заполнения основной области");
    ui->textBrowser->setText("Метод преобразования системы счисления : Плотность заполнения основной области");
    ui->textBrowser_2->setText("Метод складывания : Плотность заполнения основной области");
}

void MainWindow::grp3()
{
    num1 = 3;
    qDebug() <<  "grp3" ;
    setupModel("./Closer/out.time","./NumberSystem/out.time");
    setupChart();
    setWindowTitle("Время поиска");
    ui->textBrowser->setText("Метод преобразования системы счисления : Время поиска");
    ui->textBrowser_2->setText("Метод складывания : Время поиска");
}


void MainWindow::setupModel(QString filepatch, QString filepatch2)
{
    //отображение
    int maxGraphXView,i,minGraphXView;
    maxGraphXView = ui->spinBox->value();
    minGraphXView = ui->spinBox_first->value();
    maxGraphXView++;
    qDebug() << maxGraphXView;
    QFile file(filepatch);
    bool qwe  = file.open(QFile::ReadOnly);
    if (qwe)
        qDebug() <<  "true file"    ;
    else
        qDebug() << "false file"                     ;
    QDataStream stream(&file);
    stream >> size;
    Point points[size];
    sizeNew = 0;
    int sizeMin;
    sizeMin = 0;
    for (i=0; i <size;i++)
    {
        QDataStream& operator >> (QDataStream& stream, Point point1);
        stream >> points[i].x >> points[i].y;
        qDebug() << points[i].x << "   " << points[i].y;
        if (points[i].x <= minGraphXView)
            sizeMin++;
        if (points[i].x > maxGraphXView)
            break;
        sizeNew++;

    }
    file.close();

    QFile file2(filepatch2);
    bool qwe2  = file2.open(QFile::ReadOnly);
    if (qwe2)
        qDebug() <<  "true file2"    ;
    else
        qDebug() << "false file2"                     ;
    QDataStream stream2(&file2);
    stream2 >> size;
    Point points2[size];

    for (i=0; i <sizeNew;i++)
    {
        QDataStream& operator >> (QDataStream& stream2, Point point1);
        stream2 >> points2[i].x >> points2[i].y;
        qDebug() << points2[i].x << "   " << points2[i].y;
    }

    for (i=0; i <sizeNew;i++)
    {
        points2[i].x = points2[i].x + 10;
    }
    file2.close();

    int temp;
    temp = sizeNew-sizeMin;
    qDebug() << temp << "=" << sizeNew << "-" << sizeMin;
    model=new QStandardItemModel(temp,3,this);
    model->setHeaderData(0,Qt::Horizontal,tr("X"));
    model->setHeaderData(1,Qt::Horizontal,tr("Y"));
    model->setHeaderData(2,Qt::Horizontal,tr("K"));

    qDebug() << "======";
    qDebug() << sizeMin ;
    i=sizeMin;
    for(int j=0;j<model->rowCount();j++)
    {
        qDebug() << "-=- " << i << " " << j;
        //по іксу
        model->setData(model->index(j,0,QModelIndex()),points[i].x);
        model->setData(model->index(j,1,QModelIndex()),points[i].y);
        model->setData(model->index(j,1,QModelIndex()),QColor(255,0,0),Qt::DecorationRole);
        model->setData(model->index(j,2,QModelIndex()),points2[i].y);
        model->setData(model->index(j,2,QModelIndex()),QColor(0,255,0),Qt::DecorationRole);
        i++;
    }
  }

void MainWindow::setupChart(void)
  {
  ui->chart->setModel(model);
  ui->chart->setting().border().setRight(64);
  ui->chart->setting().border().setBackgroundColor(QColor(255,255,255));
  ui->chart->setting().grid().horizzontalTick().setTickMajor(4);
  ui->chart->setting().grid().horizzontalTick().setTickMinor(5);
  ui->chart->setting().grid().verticalTick().setTickMajor(8);
  ui->chart->setting().grid().verticalTick().setTickMinor(5);
  ui->chart->setting().grid().setBackgroundColor(QColor(192,192,192));
  ui->chart->setting().grid().setForegroundColor(QColor(0,0,0));
  ui->chart->setting().scale().setAutoCurrentLimit(true);
  ui->chart->setting().scale().setNominalAutoLimit(true);
  ui->chart->updateChart();
  }

void MainWindow::on_pushButton_clicked()
{
    if (num1 == 1)
        grp1();
    if (num1 == 2)
        grp2();
    if (num1 == 3)
        grp3();
}
