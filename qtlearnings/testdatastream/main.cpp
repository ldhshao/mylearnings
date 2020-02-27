#include <QFile>
#include <QDataStream>
#include <QCoreApplication>
#include <QList>
#include <QDebug>

void write_file(){
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_0);

    out<<(quint32)0xA0B0C0D0;
    out<<(quint32)100;
    out<<QString("This is a test file");
}
void read_file(){
    QFile file("file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_0);

    quint32 magic, ver;
    QString data;
    in>>magic;
    in>>ver;
    in>>data;

    qDebug()<<magic<<" "<<ver;
    qDebug()<<data;
}

void test_file(){
    write_file();
    read_file();
}

void write_list2file(){
    QFile file("file.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_0);

    QList<int> list;
    for(int i = 0; i < 100; i++){
        list.push_back(i);
    }
    out<<list;
}
void read_listfromfile(){
    QFile file("file.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_0);

    QList<int> list;
    in>>list;

    for(int i = 0; i < list.size(); i++)
        qDebug()<<list[i]<<" ";
}

void test_seriallist(){
    write_list2file();
    read_listfromfile();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    //test_file();
    test_seriallist();
    return a.exec();
}

