#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#define TRACK_COL   0
#define START_COL   1
#define STOP_COL    2

class TableModel: public QAbstractTableModel {
public:
    TableModel(QObject* parent = nullptr): QAbstractTableModel(parent) {}
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation ori,
                        int role = Qt::DisplayRole) const;
    void insertSegment(QString title, QTime startTime, QTime stopTime);
    QString getData(int row, int col) const;
    bool removeRow(int row, const QModelIndex& parent = QModelIndex());
private:
    QStringList mTrackTitles;
    QStringList mStartTimes;
    QStringList mStopTimes;

};

#endif // TABLEMODEL_H
