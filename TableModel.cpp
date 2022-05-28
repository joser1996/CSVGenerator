#include "TableModel.h"
#include <QTime>

int TableModel::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return mTrackTitles.count();
    }
    return 0;
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    if (!parent.isValid()) {
        return 3;
    }
    return 0;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    if (role == Qt::DisplayRole) {
        if (index.column() == TRACK_COL) {
            return mTrackTitles[index.row()];
        } else if (index.column() == START_COL) {
            return mStartTimes[index.row()];
        } else {
            return mStopTimes[index.row()];
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation ori, int role) const
{
    if (role != Qt::DisplayRole) return QVariant();
    if (ori == Qt::Horizontal) {
        if (section == TRACK_COL) {
            return "Track Title";
        } else if (section == START_COL) {
            return "Start Time";
        } else {
            return "Stop Time";
        }
    } else if (ori == Qt::Vertical) {
        return tr("%1").arg(section);
    }
    return QVariant();
}


void TableModel::insertSegment(QString title, QTime startTime, QTime stopTime)
{

    qDebug() << tr("[%1, %2]").arg(startTime.toString(), stopTime.toString());
    for (auto stopStr: mStopTimes) {
        QTime currentTime = QTime::fromString(stopStr, "HH:mm:ss");
        if (startTime < currentTime) {
            qDebug() << "Overlap!";
            return;
        }
    }

    const int lastRow = mTrackTitles.count();
    beginInsertRows(QModelIndex(), lastRow, lastRow);
    mTrackTitles.append(title);
    mStartTimes.append(startTime.toString());
    mStopTimes.append(stopTime.toString());
    endInsertRows();
}

bool TableModel::removeRow(int row, const QModelIndex& parent) {
    if (!parent.isValid()) {
        beginRemoveRows(parent, row, row);
        mTrackTitles.removeAt(row);
        mStartTimes.removeAt(row);
        mStopTimes.removeAt(row);
        endRemoveRows();
        return true;
    }
    return false;
}

QString TableModel::getData(int row, int col) const {
    switch (col) {
    case TRACK_COL:
        return mTrackTitles[row];
    case START_COL:
        return mStartTimes[row];
    case STOP_COL:
        return mStopTimes[row];
    }
    return "";
}







