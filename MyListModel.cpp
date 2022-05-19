#include "MyListModel.h"

int MyListModel::rowCount(const QModelIndex &parent) const {
    if (!parent.isValid())
        return mStrings.count();
    return 0;
}

QVariant MyListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= mStrings.size() || role != Qt::DisplayRole)
        return QVariant();

    return mStrings.at(index.row());
}


QVariant MyListModel::headerData(int section, Qt::Orientation ori, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (ori == Qt::Horizontal) {
        return QString("Column %1").arg(section);
    }

    return QString("Row %1").arg(section);
}

Qt::ItemFlags MyListModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool MyListModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        mStrings.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

//need to connect this to save
bool MyListModel::insertRows(int pos, int rows, const QModelIndex &index) {
    qDebug() << "Inserting Rows";
    beginInsertRows(QModelIndex(), pos, pos + rows - 1);

    for (int row = 0; row < rows; row++) {
        mStrings.insert(pos, "");
    }
    endInsertRows();
    return true;
}

bool MyListModel::insertRow(QString val, const QModelIndex& index) {
    qDebug() << "Attempting to insert: " << val;
    int lastIndex = this->mStrings.count();
    beginInsertRows(index, lastIndex, lastIndex);
        mStrings.append(val);
    endInsertRows();
    return true;
}

bool MyListModel::removeRows(int pos, int rows, const QModelIndex &index) {
    beginRemoveRows(QModelIndex(), pos, pos + rows - 1);

    for (int row = 0; row < rows; row++) {
        mStrings.removeAt(pos);
    }

    endRemoveRows();
    return true;
}




