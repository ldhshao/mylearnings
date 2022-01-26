#ifndef FRUITMODEL_H
#define FRUITMODEL_H

#include <QAbstractListModel>
#include <QStringList>


class Fruit
{
public:
    Fruit(const QString &name, const float &price);

    QString name() const;
    float price() const;
    void setPrice(const float &price);

private:
    QString m_name;
    float m_price;
};

class FruitModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FruitRoles {
        NameRole = Qt::UserRole + 1,
        PriceRole
    };

    FruitModel(QObject *parent = 0);

    void addFruit(const Fruit &animal);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
public slots:
    void update();
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Fruit> m_fruits;
};
#endif // FRUITMODEL_H
