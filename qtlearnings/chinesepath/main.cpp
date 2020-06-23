#include "mainwindow.h"
#include <QApplication>
#include <fstream>
#include <iostream>

//思路：测试三种场景
//1. 中文路径由代码以参数形式给出， 打开某一文件
//2. 中文路径由用户在界面中输入，
//3. 中文路径从文件（该文件路径不包含中文）中读取

void readFixedPath()
{
    //std::ifstream ifs("D:\\books\\测试\\packet.txt", std::ios::in);
    std::ifstream ifs(QString("D:\\books\\测试\\packet.txt").toStdString().c_str(), std::ios::in);
    if (ifs.is_open())
    {
        std::string strText;
        while(std::getline(ifs, strText))
        {
            std::cout<<strText<<"\n";
        }
        ifs.close();
    }
}
void readFromConfig()
{

}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    readFixedPath();

    MainWindow w;
    w.show();

    return a.exec();
}
