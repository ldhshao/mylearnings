#include "fruitmodel.h"
#include <QtDebug>
///fruit
Fruit::Fruit(const QString &name, const float &price)
    : m_name(name), m_price(price)
{
}

QString Fruit::name() const
{
    return m_name;
}
float Fruit::price() const
{
    return m_price;
}
void Fruit::setPrice(const float &price)
{
    m_price = price;
}

///FruitModel
FruitModel::FruitModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

void FruitModel::addFruit(const Fruit &animal)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_fruits << animal;
    endInsertRows();
}

int FruitModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_fruits.count();
}

QVariant FruitModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_fruits.count())
        return QVariant();

    const Fruit &fruit = m_fruits[index.row()];
    if (role == NameRole)
        return fruit.name();
    else if (role == PriceRole)
        return QString("%1").arg(fruit.price(), 0, 'f', 2);
    return QVariant();
}

void FruitModel::update(){
    //qDebug()<<"update";
    if (m_fruits.count() > 0)
    {
        Fruit &ft = m_fruits[0];
        float fPrice = ft.price();
        float fStep = 0.1;
        static float fSign = 1;
        if (fPrice >= 5.0)
            fSign = -1;
        else if (fPrice <= 2.5)
            fSign = 1;

        ft.setPrice(fPrice + fSign*fStep);
        qDebug()<<"update: "<<ft.name()<<" "<<ft.price();
    }
}


//![0]
QHash<int, QByteArray> FruitModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PriceRole] = "cost";
    return roles;
}
