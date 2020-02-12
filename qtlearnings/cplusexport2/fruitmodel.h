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
    void setPrice(float);

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
    ~FruitModel();

    void addFruit(Fruit *animal);
    void rowReset(int index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

protected:
    QHash<int, QByteArray> roleNames() const;
private:
    QList<Fruit*> m_fruits;
};
#endif // FRUITMODEL_H
