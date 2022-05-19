#ifndef MYLISTMODEL_H
#define MYLISTMODEL_H
#include <QAbstractListModel>
#include <QObject>
#include <QList>

class MyListModel: public QAbstractListModel {
    Q_OBJECT
public:
    MyListModel(const QList<QString>& strings, QObject* parent = 0)
        : QAbstractListModel(parent), mStrings(strings) {}

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation ori,
                        int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex& index) const;

    bool setData(const QModelIndex& index, const QVariant& value,
                 int role = Qt::EditRole);
    bool insertRows(int pos, int rows, const QModelIndex& index = QModelIndex());
    bool removeRows(int pos, int rows, const QModelIndex& index = QModelIndex());
    bool insertRow( QString val, const QModelIndex& index = QModelIndex());
private:
    QList<QString> mStrings;
};


#endif // MYLISTMODEL_H
