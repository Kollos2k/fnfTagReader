#include "TableModelSeenChipsProxy.h"
#include <QSortFilterProxyModel>
#include <iostream>

TableModelSeenChipsProxy::TableModelSeenChipsProxy(QObject *parent):QSortFilterProxyModel(parent){

}
QVariant TableModelSeenChipsProxy::headerData(int section, Qt::Orientation orientation,
                                int role) const {
    return sourceModel()->headerData(section, orientation,
                                     role);
}
int TableModelSeenChipsProxy::columnCount(const QModelIndex &parent) const{
    return sourceModel()->columnCount(parent);
}
bool TableModelSeenChipsProxy::lessThan(const QModelIndex &left,const QModelIndex &right) const{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);
    if(leftData.type() == QVariant::Int){
        return (leftData.toInt() < rightData.toInt());
    }else if(leftData.type() == QVariant::String){
        return (leftData.toString() < rightData.toString());
    }else if(leftData.type() == QVariant::Bool){
        return (leftData.toInt() < rightData.toInt());
    }
    return true;
}
