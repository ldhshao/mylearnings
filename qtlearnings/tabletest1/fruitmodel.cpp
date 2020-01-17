#include "fruitmodel.h"
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
        return fruit.price();
    return QVariant();
}

//![0]
QHash<int, QByteArray> FruitModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PriceRole] = "cost";
    return roles;
}
