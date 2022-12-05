#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <vector>
#include "form.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void init();
    void tableInit_1();
    void tableInit_2();
    void formInit(int choose);

    void getTableData();
    std::vector<std::vector<int>> table_data;
    std::vector<int> job_data;
    QStringList job_seq;

    QTableWidget* table_job;
    QTableWidget* table_initial;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
