#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QString>
#include <QLabel>
#include <QFont>
#include <QDebug>
#include <QMessageBox>
#include <vector>
#include "Partition.h"

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);

    void init(QStringList& job_seq,std::vector<int>& job_data,std::vector<std::vector<int>>& table_data);

    ~Form();

public:
    int choose = 0;

private:
    void tableInit_3(QStringList& job_seq,std::vector<int>& job_data);
    void tableInit_4(std::vector<std::vector<int>>& table_data);
    void list_init(std::vector<std::vector<int>>& table_data);
    bool malloc(int job,int job_size);
    void release(int job,int job_size);
    void update();
    void messageBox(int job);

    QTableWidget* job_table;
    QTableWidget* Partition_table;
    QButtonGroup* btn_group;


    std::vector<bool> btn_flag;
    std::vector<QPushButton*> btn_array;
    Linklist* list;
    Ui::Form *ui;
};

#endif // FORM_H
