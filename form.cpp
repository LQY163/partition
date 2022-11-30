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
//    job_table->setItem(1,1,new QTableWidgetItem (std::to_string(link.free_head->data.size).data()));
//    job_table->setItem(0,0,new QTableWidgetItem("50"));
//    job_table->setItem(1,0,new QTableWidgetItem("80"));
//    job_table->setItem(2,0,new QTableWidgetItem("240"));
//    job_table->setItem(3,0,new QTableWidgetItem("300"));

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
                btn_array[i+1]->setText("回收");
                //分配J1
                malloc(i+1);
            }
            else{
                btn_array[i+1]->setText("分配");
                //回收J1
                release(i+1);
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

//    tableInit_3();

//    table_initial->repaint();
//    getTableData(table_initial);

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


void Form::malloc(int job)
{
    switch(choose){
    case 1://First_Fit

        break;
    case 2://Best_Fit

        break;
    case 3://Worst_Fit

        break;
    case 4://Nest_Fit

        break;
    case 5://Quick_Fit

        break;
    default:
        break;
    }

}

void Form::release(int job)
{
    switch(choose){
    case 1://First_Fit

        break;
    case 2://Best_Fit

        break;
    case 3://Worst_Fit

        break;
    case 4://Nest_Fit

        break;
    case 5://Quick_Fit

        break;
    default:
        break;
    }
}

void Form::update()
{

}

Form::~Form()
{
//    qDebug()<<"delete ui"<<endl;
    delete ui;
}
