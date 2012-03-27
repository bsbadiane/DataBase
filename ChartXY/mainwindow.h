#ifndef MAINWINDOW_H
  #define MAINWINDOW_H

  #include <QMainWindow>
  #include <QStandardItemModel>
  #include <QDateTime>

namespace Ui
{
    class MainWindow;
}

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent=0);
    ~MainWindow();
    void setupModel(QString filepatch, QString filepatch2);
    void setupChart(void);
public slots:
    void grp1();
    void grp2();
    void grp3();
private:
    Ui::MainWindow *ui;
    QStandardItemModel *model;
    struct Point
    {
        int x;
        float y;
    };
    int size,sizeNew,num1;

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
