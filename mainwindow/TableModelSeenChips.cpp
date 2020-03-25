#include "TableModelSeenChips.h"
#include <QAbstractTableModel>
#include <QBrush>
#include <QPainter>
#include <QStyle>
#include "classes/Holder.h"

#include "TableModelSeenChipsRow.h"

TableModelSeenChips::TableModelSeenChips(QObject *parent): QAbstractTableModel(parent){
    parentObj=parent;
}
int TableModelSeenChips::rowCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return rowData.size();
}

bool TableModelSeenChips::insertRow(TableModelSeenChipsRow value){
    QModelIndex rowLeft = createIndex(rowData.size(),0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    beginInsertRows(QModelIndex(), rowData.size(), rowData.size());
    rowData.append(value);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
    endInsertRows();
    return true;
}
void TableModelSeenChips::fireChange(){
    QModelIndex rowLeft = createIndex(0,0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
}
void TableModelSeenChips::clear(){
    beginResetModel();
    rowData.clear();
    endResetModel();
}

int TableModelSeenChips::columnCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return 3;
}
QVariant TableModelSeenChips::data(const QModelIndex &index, int role) const{
    if(index.row()>=rowCount()||index.column()>=columnCount())return QVariant();
    if (role == Qt::DisplayRole){
        switch(index.column()){
        case 0: return rowData.at(index.row()).getBib();
        case 1: return rowData.at(index.row()).getSnr();
        case 2: return rowData.at(index.row()).getName();
        }
    }
    return QVariant();
}
QVariant TableModelSeenChips::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Bib");
        case 1:
            return QString("Snr");
        case 2:
            return QString("Name");
        }
    }
    return QVariant();
}
bool TableModelSeenChips::setData(const QModelIndex &index, const QVariant &value, int role){
    Q_UNUSED(role);
    Q_UNUSED(value);
    Q_UNUSED(index);
    return true;
}
Qt::ItemFlags TableModelSeenChips::flags(const QModelIndex &index) const{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
