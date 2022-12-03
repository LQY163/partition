#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    resize(1200,900);
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_DeleteOnClose);


}

void Form::init(QStringList& job_seq,std::vector<int>& job_data,std::vector<std::vector<int>>& table_data)
{
    tableInit_3(job_seq,job_data);
    tableInit_4(table_data);
    list_init(table_data);
}

void Form::tableInit_3(QStringList& job_seq,std::vector<int>& job_data)
{
    job_table = new QTableWidget(job_data.size(),2,this);
    job_table->resize(333,219);//333,219
    job_table->move(100,100);

    QLabel* label_job = new QLabel("作业分配表",this);
    QFont ft;
    ft.setPointSize(15);
    label_job->setFont(ft);
    label_job->move(185,50);

    job_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    job_table->setHorizontalHeaderLabels(QStringList()<<"大小(B)"<<"状态");
    job_table->setVerticalHeaderLabels(job_seq);

    btn_array.push_back(new QPushButton("全部分配",this));//btn_array[0]:全部分配
    //btn_flag[0]:全部分配
    //btn_flag[1-4]:J1-J4分配
    //true:未分配状态  btn text:分配
    //false:分配状态  btn text:回收
    btn_flag.push_back(true);

    for(int i=0;i<job_table->rowCount();i++){
        job_table->setItem(i,0,new QTableWidgetItem(QString::number(job_data[i])));
        job_table->item(i,0)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


        btn_array.push_back(new QPushButton("分配",this));    //btn_array[1]:J1分配
        btn_flag.push_back(true);                            //btn_flag[1]:

        job_table->setCellWidget(i,1,btn_array[i+1]);

        connect(btn_array[i+1],&QPushButton::clicked,this,[=](){
            btn_flag[i+1] = !btn_flag[i+1];
            if(!btn_flag[i+1]){
                //分配J1
                if(malloc(i+1,job_data[i])) btn_array[i+1]->setText("回收");
                else btn_flag[i+1] = !btn_flag[i+1];
                qDebug()<<QString("分配J%1").arg(i+1);
            }
            else{
                btn_array[i+1]->setText("分配");
                //回收J1
                release(i+1,job_data[i]);
                qDebug()<<QString("回收J%1").arg(i+1);
            }
        });
    }

//    btn_array.push_back(new QPushButton("分配",this));    //btn_array[2]:J2分配
//    btn_array.push_back(new QPushButton("分配",this));    //btn_array[3]:J3分配
//    btn_array.push_back(new QPushButton("分配",this));    //btn_array[4]:J4分配

    btn_array[0]->move(210,330);//147,330


    connect(btn_array[0],&QPushButton::clicked,this,[=]{
        btn_flag[0] = !btn_flag[0];
        if(!btn_flag[0]){
            btn_array[0]->setText("全部回收");
            //全部分配
            for(int i=1;i<=job_table->rowCount();i++){
                if(btn_flag[i]) btn_array[i]->click();
            }
        }
        else{
            btn_array[0]->setText("全部分配");
            //全部回收
            for(int i=1;i<=job_table->rowCount();i++){
                if(!btn_flag[i]) btn_array[i]->click();
            }
        }
    });

}

void Form::tableInit_4(std::vector<std::vector<int>>& table_data)
{

    Partition_table = new QTableWidget(table_data.size(),3,this);

    Partition_table->resize(501,600);//475,300
    Partition_table->move(570,75);


    Partition_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    QPushButton* close_btn = new QPushButton("关闭",this);
    close_btn->move(500,750);
    connect(close_btn,&QPushButton::clicked,this,[=]{
        this->close();
    });

    QLabel* label_initial = new QLabel("分区表",this);
    QFont ft;
    ft.setPointSize(15);
    label_initial->setFont(ft);
    label_initial->adjustSize();
    label_initial->move(770,25);

    Partition_table->setHorizontalHeaderLabels(QStringList()<<"大小(B)"<<"起始地址"<<"状态");
    for(int i=0;i<Partition_table->rowCount();i++){
        Partition_table->setItem(i,0,new QTableWidgetItem(QString::number(table_data[i][0])));
        Partition_table->setItem(i,1,new QTableWidgetItem(QString::number(table_data[i][1])));
        Partition_table->setItem(i,2,new QTableWidgetItem("free"));
        for(int j=0;j<Partition_table->columnCount();j++){
            Partition_table->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
    }
}


void Form::list_init(std::vector<std::vector<int>> &table_data)
{
    list = new Linklist();
    list->init(table_data);
}


bool Form::malloc(int job,int job_size)
{
    bool is_ok = false;
    switch(choose){
    case 1://First_Fit
        is_ok = list->First_Fit(job,job_size);
        break;
    case 2://Best_Fit
        is_ok = list->Best_Fit(job,job_size);
        break;
    case 3://Worst_Fit
        is_ok = list->Worst_Fit(job,job_size);
        break;
    case 4://Nest_Fit
        is_ok = list->Nest_Fit(job,job_size);
        break;
    case 5://Quick_Fit

        break;
    default:
        break;
    }

    if(is_ok){
        update();
        return true;
    }
    else{
        messageBox(job);
        return false;
    }
}

void Form::release(int job,int job_size)
{
    if(choose!=5){
        if(list->free_one(job)) update();
//        else ;
    }
    else{

    }
}

void Form::update()
{
    QStringList job_seq;
    int i = 0;
    Node* p = list->head;
    while(p){
        if(i >= Partition_table->rowCount()) Partition_table->insertRow(Partition_table->rowCount());
        QTableWidgetItem* item0 = new QTableWidgetItem(QString::number(p->data.size));
        QTableWidgetItem* item1 = new QTableWidgetItem(QString::number(p->data.address));
        QTableWidgetItem* item2 = new QTableWidgetItem(p->data.state ? "busy" : "free");
        Partition_table->setItem(i,0,item0);
        Partition_table->setItem(i,1,item1);
        Partition_table->setItem(i,2,item2);
        for(int j=0;j<Partition_table->columnCount();j++){
            Partition_table->item(i,j)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }
        if(p->data.state){
            item0->setTextColor(QColor(Qt::red));
            item1->setTextColor(QColor(Qt::red));
            item2->setTextColor(QColor(Qt::red));
            job_seq<<QString::fromStdString(list->map[p]);
        }
        else job_seq<<QString("%1").arg(i+1);
        i++;
        p = p->next;
    }
    if(i < Partition_table->rowCount()) Partition_table->setRowCount(i);
    Partition_table->setVerticalHeaderLabels(job_seq);
}


void Form::messageBox(int job)
{
    QMessageBox::critical(this,tr("错误！"),tr("内存不足！\nJ%1分配失败！").arg(job),
            QMessageBox::Ok | QMessageBox::Cancel , QMessageBox::Ok);
}


Form::~Form()
{
//    qDebug()<<"delete ui"<<endl;
    delete list;
    delete ui;
}
