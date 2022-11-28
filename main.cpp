#include "mainwindow.h"
#include "Partition.h"
#include <vector>
#include <QDebug>
#include <QApplication>
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    std::vector<int> v(10,6);
//    for(auto& i:v){
//        qDebug()<<i<<endl;
//    }
//    qDebug()<<666<<endl;

    return a.exec();
}
