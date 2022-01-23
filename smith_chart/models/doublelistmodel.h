#ifndef DOUBLELISTMODEL_H
#define DOUBLELISTMODEL_H

#include <QAbstractListModel>

class DoubleListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit DoubleListModel(QObject *parent = nullptr);
    void fillModel(const QVector<double> &vect);
    QVector<double> getAllData();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    QModelIndex addItem();
    void removeItem(const QModelIndex &index);
private:

    QVector<double> m_numbers;
};

#endif // DOUBLELISTMODEL_H
