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
    resize(1100,700);
    setWindowTitle("可变分区");


//    Linklist link =  Linklist ();


    table_job = new QTableWidget(4,1,this);
    table_initial = new QTableWidget(5,3,this);

    tableInit_1();//table_job
    tableInit_2();//table_initial
//    tableInit_3();

//    int First_Fit();
//    int Best_Fit();
//    int Worst_Fit();
//    int Nest_Fit();
//    int Quick_Fit();
    QPushButton* add_job_btn = new QPushButton("添加",this);
    add_job_btn->move(103,375);
    connect(add_job_btn,&QPushButton::clicked,this,[=]{
        int row = table_job->rowCount();
        table_job->insertRow(row);

        table_job->setItem(row,0,new QTableWidgetItem("0"));
        table_job->item(row,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        QStringList list;
        for(int i=0;i<row+1;i++){
            list<<QString("J%1").arg(i+1);
        }
        job_seq = list;
        table_job->setVerticalHeaderLabels(job_seq);
    });

    QPushButton* del_job_btn = new QPushButton("删除",this);
    del_job_btn->move(203,375);
    connect(del_job_btn,&QPushButton::clicked,this,[=]{
        table_job->setRowCount(table_job->rowCount()-1);
    });


    QPushButton* add_btn = new QPushButton("添加",this);
    add_btn->move(625,400);
    connect(add_btn,&QPushButton::clicked,this,[=]{
        int old_row = table_initial->rowCount();
        table_initial->insertRow(old_row);
        for(int i=0;i<table_initial->columnCount();i++){
            if(i<2) table_initial->setItem(old_row,i,new QTableWidgetItem("0"));
            else table_initial->setItem(old_row,i,new QTableWidgetItem("free"));
            table_initial->item(old_row,i)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    });

    QPushButton* del_btn = new QPushButton("删除",this);
    del_btn->move(775,400);
    connect(del_btn,&QPushButton::clicked,this,[=]{
        table_initial->setRowCount(table_initial->rowCount()-1);
    });

    QPushButton* first_fit_btn = new QPushButton("First_Fit",this);
    first_fit_btn->move(300,500);
    connect(first_fit_btn,&QPushButton::clicked,this,[=]{
        formInit(1);

    });

    QPushButton* best_fit_btn = new QPushButton("Best_Fit",this);
    best_fit_btn->move(400,500);
    connect(best_fit_btn,&QPushButton::clicked,this,[=]{
        formInit(2);

    });

    QPushButton* worst_fit_btn = new QPushButton("Worst_Fit",this);
    worst_fit_btn->move(500,500);
    connect(worst_fit_btn,&QPushButton::clicked,this,[=]{
        formInit(3);

    });

    QPushButton* nest_fit_btn = new QPushButton("Nest_Fit",this);
    nest_fit_btn->move(600,500);
    connect(nest_fit_btn,&QPushButton::clicked,this,[=]{
        formInit(4);
        //
    });

    QPushButton* quick_fit_btn = new QPushButton("Quick_Fit",this);
    quick_fit_btn->move(700,500);
    connect(quick_fit_btn,&QPushButton::clicked,this,[=]{
        formInit(5);
        //
    });

    QPushButton* btn_close = new QPushButton("关闭",this);
    btn_close->move(500,600);
    connect(btn_close,&QPushButton::clicked,this,[=]{
        MainWindow::close();
        //
    });


//    ui->pushButton_2->setText(std::to_string(link.free_head->data.id).data());



}

//void MainWindow::init()
//{
//    QWidget* widget = new QWidget();
//    widget->setWindowTitle("QTableWidget");//
//    widget->resize(1600,900);
//    QTableWidget* table = new QTableWidget(10,5,widget);
//    table->setWindowTitle("QtableWidget");
//    table->resize(300,350);
//    table->move(200,100);
//    widget->show();

//}

void MainWindow::tableInit_1()
{
    table_job->resize(205,259);//333,219
    table_job->move(100,100);

    QLabel* label_job = new QLabel("作业表",this);
    QFont ft;
    ft.setPointSize(15);
    label_job->setFont(ft);
    label_job->move(145,50);

    job_seq<<"J1"<<"J2"<<"J3"<<"J4";
    table_job->setHorizontalHeaderLabels(QStringList()<<"大小(B)"<<"状态");
    table_job->setVerticalHeaderLabels(job_seq);
//    table_job->setItem(1,1,new QTableWidgetItem (std::to_string(link.free_head->data.size).data()));
    table_job->setItem(0,0,new QTableWidgetItem("50"));
    table_job->setItem(1,0,new QTableWidgetItem("80"));
    table_job->setItem(2,0,new QTableWidgetItem("240"));
    table_job->setItem(3,0,new QTableWidgetItem("300"));
    for(int i=0;i<4;i++){
        table_job->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    }
}

void MainWindow::tableInit_2()
{   
    table_initial->resize(501,300);//475,300
    table_initial->move(500,75);

    QLabel* label_initial = new QLabel("起始分区表",this);
    QFont ft;
    ft.setPointSize(15);
    label_initial->setFont(ft);
    label_initial->adjustSize();
    label_initial->move(675,25);

    table_initial->setHorizontalHeaderLabels(QStringList()<<"大小(B)"<<"起始地址"<<"状态");
    QString str_size[5] = {"25","75","150","300","450"};
    QString str_adders[5] = {"0","25","100","250","550"};
    QString str_state[5] = {"free","free","free","free","free"};
    for(int i=0;i<5;i++){
        table_initial->setItem(i,0,new QTableWidgetItem(str_size[i]));
        table_initial->setItem(i,1,new QTableWidgetItem(str_adders[i]));
        table_initial->setItem(i,2,new QTableWidgetItem(str_state[i]));
        for(int j=0;j<table_initial->columnCount();j++){
            table_initial->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}


void MainWindow::formInit(int choose)
{
    Form* form = new Form();
    getTableData();
    form->choose = choose;
    form->init(job_seq,job_data,table_data);
    form->show();
}



void MainWindow::getTableData()
{
    int row = table_job->rowCount();
    job_data.clear();
    for(int i=0;i<row;i++){
        if(table_job->item(i,0)) job_data.push_back(table_job->item(i,0)->text().toInt());
    }

    row = table_initial->rowCount();
    table_data.clear();
    for(int i=0;i<row;i++){
        std::vector<int> r;
        if(table_initial->item(i,0)) r.push_back(table_initial->item(i,0)->text().toInt());
        if(table_initial->item(i,1)) r.push_back(table_initial->item(i,1)->text().toInt());
        if(r.size()) table_data.push_back(r);
    }
}



MainWindow::~MainWindow()
{
//    delete widget;
//    delete form;
    delete ui;
}

