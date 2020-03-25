#include "TableModelGroupChips.h"
#include <QAbstractTableModel>
#include <QBrush>
#include <QPainter>
#include <QStyle>
#include "classes/Holder.h"

#include "TableModelGroupChipsRow.h"

TableModelGroupChips::TableModelGroupChips(QObject *parent): QAbstractTableModel(parent){
    parentObj=parent;
}
int TableModelGroupChips::rowCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return rowData.size();
}

bool TableModelGroupChips::insertRow(TableModelGroupChipsRow value){
    QModelIndex rowLeft = createIndex(rowData.size(),0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    beginInsertRows(QModelIndex(), rowData.size(), rowData.size());
    rowData.append(value);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
    endInsertRows();
    return true;
}
void TableModelGroupChips::fireChange(){
    QModelIndex rowLeft = createIndex(0,0);
    QModelIndex rowRight = createIndex(rowData.size(),columnCount()-1);
    emit dataChanged(rowLeft, rowRight, {Qt::DisplayRole});
}
void TableModelGroupChips::clear(){
    beginResetModel();
    rowData.clear();
    endResetModel();
}

int TableModelGroupChips::columnCount(const QModelIndex & parent) const{
    Q_UNUSED(parent);
    return 5;
}
QVariant TableModelGroupChips::data(const QModelIndex &index, int role) const{
    if(index.row()>=rowCount()||index.column()>=columnCount())return QVariant();
    if (role == Qt::DisplayRole){
        switch(index.column()){
        case 0: return rowData.at(index.row()).getColumn1();
        case 1: return rowData.at(index.row()).getColumn2();
        case 2: return rowData.at(index.row()).getColumn3();
        case 3: return rowData.at(index.row()).getColumn4();
        case 4: return rowData.at(index.row()).getColumn5();
        }
    } else if(role==Qt::BackgroundRole){
        switch(index.column()){
        case 2: return (!rowData.at(index.row()).getColumn3())?QBrush(Qt::red):QBrush(Qt::darkGreen);
        }
    } else if(role==Qt::TextColorRole){
        switch(index.column()){
        case 2: return (!rowData.at(index.row()).getColumn3())?QColor(Qt::red):QColor(Qt::darkGreen);
        }
    }
    return QVariant();
}
QVariant TableModelGroupChips::headerData(int section, Qt::Orientation orientation, int role) const{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return QString("Von");
        case 1:
            return QString("Bis");
        case 2:
            return QString("Seen");
        case 3:
            return QString("n/sum");
        case 4:
            return QString("Offene Nummern");
        }
    }
    return QVariant();
}
bool TableModelGroupChips::setData(const QModelIndex &index, const QVariant &value, int role){
    Q_UNUSED(role);
    Q_UNUSED(value);
    Q_UNUSED(index);
    return true;
}
Qt::ItemFlags TableModelGroupChips::flags(const QModelIndex &index) const{
    return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
}
TableModelGroupChipsRow *TableModelGroupChips::getRowAt(int row){
    if(rowCount()>row){
        TableModelGroupChipsRow ret=rowData.at(row);
        return &ret;
    }
    return nullptr;
}

bool TableModelGroupChips::isSeen(int row){
    if(rowCount()>row){
        return rowData.at(row).getColumn3();
    }
    return false;
}
