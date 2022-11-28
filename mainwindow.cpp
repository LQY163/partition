#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Partition.h"
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1600,900);

    Linklist link =  Linklist ();

    QTableWidget* table = new QTableWidget(5,3,this);

    tableInit_1();
    tableInit_2(table);

    ui->pushButton_2->move(1200,330);//303,330
    connect(ui->pushButton_2,&QPushButton::clicked,this,[=]{
        MainWindow::close();
        //
    });


//    ui->pushButton_2->setText(std::to_string(link.free_head->data.id).data());

//    connect(ui->tableWidget,&QTableWidget::cellClicked,ui->tableWidget,[=](int row,int column){
//        ui->tableWidget->setItem(row,column,new QTableWidgetItem("table"));
//    });

//    ui->pushButton_2->setText("aaa");


}

void MainWindow::init()
{
    QWidget* widget = new QWidget();
    widget->setWindowTitle("QTableWidget");
    widget->resize(900,900);
    QTableWidget* table = new QTableWidget(10,5,widget);
    table->setWindowTitle("QtableWidget");
    table->resize(300,350);
    table->move(200,100);
    widget->show();
}

void MainWindow::tableInit_1()
{

    ui->tableWidget->resize(333,219);//333,219
    ui->tableWidget->move(100,100);

    ui->pushButton->move(227,330);//147,330
//    ui->pushButton_2->move(500,330);//303,330

    ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"大小"<<"状态");
    ui->tableWidget->setVerticalHeaderLabels(QStringList()<<"J1"<<"J2"<<"J3"<<"J4");
//    ui->tableWidget->setItem(1,1,new QTableWidgetItem (std::to_string(link.free_head->data.size).data()));
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("50B"));
    ui->tableWidget->setItem(1,0,new QTableWidgetItem("80B"));
    ui->tableWidget->setItem(2,0,new QTableWidgetItem("240B"));
    ui->tableWidget->setItem(3,0,new QTableWidgetItem("300B"));
    for(int i=0;i<4;i++){
        ui->tableWidget->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }

    ui->tableWidget->setCellWidget(0,1,ui->pushButton_3);
    ui->tableWidget->setCellWidget(1,1,ui->pushButton_4);
    ui->tableWidget->setCellWidget(2,1,ui->pushButton_5);
    ui->tableWidget->setCellWidget(3,1,ui->pushButton_6);

    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
        if(ui->pushButton_3->text()=="分配"){
            ui->pushButton_3->setText("回收");
            //
        }
        else{
            ui->pushButton_3->setText("分配");
            //
        }

    });
    connect(ui->pushButton_4,&QPushButton::clicked,this,[=](){
        if(ui->pushButton_4->text()=="分配"){
            ui->pushButton_4->setText("回收");
            //
        }
        else{
            ui->pushButton_4->setText("分配");
            //
        }
    });
    connect(ui->pushButton_5,&QPushButton::clicked,this,[=](){
        if(ui->pushButton_5->text()=="分配"){
            ui->pushButton_5->setText("回收");
            //
        }
        else{
            ui->pushButton_5->setText("分配");
            //
        }
    });
    connect(ui->pushButton_6,&QPushButton::clicked,this,[=](){
        if(ui->pushButton_6->text()=="分配"){
            ui->pushButton_6->setText("回收");
            //
        }
        else{
            ui->pushButton_6->setText("分配");
            //
        }
    });


    connect(ui->pushButton,&QPushButton::clicked,this,[=]{
        if(ui->pushButton->text()=="全部分配"){
            ui->pushButton->setText("全部回收");
            //
        }
        else{
            ui->pushButton->setText("全部分配");
            //
        }
//        init();
    });

//    connect(ui->pushButton_2,&QPushButton::clicked,this,[=]{
//        MainWindow::close();
//        //
//    });

}


void MainWindow::tableInit_2(QTableWidget* table)
{
    table->resize(475,300);
    table->move(500,100);
    table->setHorizontalHeaderLabels(QStringList()<<"大小"<<"起始地址"<<"状态");
//    table->show();
}


MainWindow::~MainWindow()
{
    delete ui;
}

