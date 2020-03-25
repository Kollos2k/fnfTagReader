#ifndef TABLEMODELGROUPCHIPS_H
#define TABLEMODELGROUPCHIPS_H
#include <QAbstractTableModel>
#include <QObject>
#include "TableModelGroupChipsRow.h"

class TableModelGroupChips : public QAbstractTableModel{
    Q_OBJECT
private:
    QVector<TableModelGroupChipsRow> rowData;
    QObject *parentObj=nullptr;
public:
    TableModelGroupChips(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRow(TableModelGroupChipsRow value);
    void fireChange();
    bool removeRow(QModelIndex);
    void clear();
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    TableModelGroupChipsRow *getRowAt(int row);
    bool isSeen(int row);
};

#endif // TABLEMODELGROUPCHIPS_H
