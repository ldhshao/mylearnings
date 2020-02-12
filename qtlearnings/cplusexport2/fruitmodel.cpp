#include "fruitmodel.h"

#include "myapp.h"
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
void Fruit::setPrice(float newPrice)
{
    m_price = newPrice;
}

///FruitModel
FruitModel::FruitModel(QObject *parent)
    : QAbstractListModel(parent)
{
}
FruitModel::~FruitModel()
{
    qDebug()<<"FruitModel disconstruction";
    for(QList<Fruit*>::iterator it = m_fruits.begin(); it != m_fruits.end(); it++){
        delete *it;
    }
    m_fruits.clear();
}

void FruitModel::addFruit(Fruit *animal)
{
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_fruits << animal;
    endInsertRows();
}
void FruitModel::rowReset(int index)
{
        beginResetModel();
        endResetModel();
}

int FruitModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_fruits.count();
}

QVariant FruitModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_fruits.count())
        return QVariant();

    Fruit* fruit = m_fruits[index.row()];
    if (role == NameRole)
        return fruit->name();
    else if (role == PriceRole)
        return fruit->price();
    return QVariant();
}

//![0]
QHash<int, QByteArray> FruitModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PriceRole] = "cost";
    return roles;
}
