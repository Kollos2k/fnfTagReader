#include "TableModelKnownChips.h"
#include <QAbstractTableModel>
#include <QBrush>
#include <QPainter>
#include <QStyle>
#include "classes/Holder.h"

#include "TableModelKnownChipsRow.h"

TableModelKnownChips::TableModelKnownChips(QObject *parent): QAbstractTableModel(parent){
    parentObj=parent;
}
int TableModelKnownChips::rowCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return rowData.size();
}

bool TableModelKnownChips::insertRow(TableModelKnownChipsRow value){
    QModelIndex rowLeft = createIndex(rowData.size(),0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    beginInsertRows(QModelIndex(), rowData.size(), rowData.size());
    rowData.append(value);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
    endInsertRows();
    return true;
}
void TableModelKnownChips::fireChange(){
    QModelIndex rowLeft = createIndex(0,0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
}
void TableModelKnownChips::clear(){
    beginResetModel();
    rowData.clear();
    endResetModel();
}

int TableModelKnownChips::columnCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return 4;
}
QVariant TableModelKnownChips::data(const QModelIndex &index, int role) const{
    if(index.row()>=rowCount()||index.column()>=columnCount())return QVariant();
    if (role == Qt::DisplayRole){
        switch(index.column()){
        case 0: return rowData.at(index.row()).getBib();
        case 1: return rowData.at(index.row()).getSnr();
        case 2: return Holder::seenBibsUsed->contains(rowData.at(index.row()).getBib());
        case 3: return rowData.at(index.row()).getName();
        }
    } else if(role==Qt::BackgroundRole){
        switch(index.column()){
        case 2: return (!Holder::seenBibsUsed->contains(rowData.at(index.row()).getBib()))?QBrush(Qt::red):QBrush(Qt::darkGreen);
        }
    } else if(role==Qt::TextColorRole){
        switch(index.column()){
        case 2: return (!Holder::seenBibsUsed->contains(rowData.at(index.row()).getBib()))?QColor(Qt::red):QColor(Qt::darkGreen);
        }
    }
//    Qt::role
    return QVariant();
}
QVariant TableModelKnownChips::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Bib");
        case 1:
            return QString("Snr");
        case 2:
            return QString("seen");
        case 3:
            return QString("Name");
        }
    }
    return QVariant();
}
bool TableModelKnownChips::setData(const QModelIndex &index, const QVariant &value, int role){
    Q_UNUSED(role);
    Q_UNUSED(value);
    Q_UNUSED(index);
    return true;
}
Qt::ItemFlags TableModelKnownChips::flags(const QModelIndex &index) const{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
