//closer.out.over  closer.out.ro  numbersys.out.over  numbersys.out.ro

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent):
  QMainWindow(parent),
  ui(new Ui::MainWindow)
  {
  ui->setupUi(this);

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
    qDebug() <<  "grp1" ;
    setupModel("./Closer/out.over","./Closer/out.ro");

    setupChart();

    setWindowTitle("BD num1");
}

void MainWindow::grp2()
{
    qDebug() <<  "grp2" ;
}

void MainWindow::grp3()
{
qDebug() <<  "grp3" ;
}

void MainWindow::setupModel(QString filepatch, QString filepatch2)
{
    int i;

    QFile file(filepatch);
    bool qwe  = file.open(QFile::ReadOnly);
    if (qwe)
        qDebug() <<  "true"    ;
    else
        qDebug() << "false;"                     ;
    QDataStream stream(&file);
    stream >> size;
    qDebug() << size;
    Point points[size];

    for (i=0; i <size;i++)
    {
        QDataStream& operator >> (QDataStream& stream, Point point1);
        {
                stream >> points[i].x >> points[i].y;
                //return stream;
        }
        qDebug() << points[i].x;
        qDebug() << points[i].y;
    }

    file.close();

    qDebug() << "===============================";

    QFile file2(filepatch2);
    bool qwe2  = file2.open(QFile::ReadOnly);
    if (qwe2)
        qDebug() <<  "true"    ;
    else
        qDebug() << "false;"                     ;
    QDataStream stream2(&file2);
    stream2 >> size;
    qDebug() << size;
    Point points2[size];

    for (i=0; i <size;i++)
    {
        QDataStream& operator >> (QDataStream& stream2, Point point1);
        {
                stream2 >> points2[i].x >> points2[i].y;
                //return stream;
        }
        qDebug() << points2[i].x;
        qDebug() << points2[i].y;
    }

    file2.close();

  model=new QStandardItemModel(size,3,this);
  model->setHeaderData(0,Qt::Horizontal,tr("X"));
  model->setHeaderData(1,Qt::Horizontal,tr("Y"));
  model->setHeaderData(2,Qt::Horizontal,tr("K"));

  for(i=0;i<model->rowCount();i++)
    {
        //по іксу
    model->setData(model->index(i,0,QModelIndex()),points[i].x);

    model->setData(model->index(i,1,QModelIndex()),points[i].y);
    model->setData(model->index(i,1,QModelIndex()),QColor(255,0,0),Qt::DecorationRole);

    model->setData(model->index(i,2,QModelIndex()),points2[i].y);
    model->setData(model->index(i,2,QModelIndex()),QColor(0,255,0),Qt::DecorationRole);
    }

  //ui->table->setModel(model);
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
