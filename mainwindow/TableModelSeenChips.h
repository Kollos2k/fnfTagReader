#ifndef TABLEMODELSEENCHIPS_H
#define TABLEMODELSEENCHIPS_H
#include <QAbstractTableModel>
#include <QObject>
#include "TableModelSeenChipsRow.h"

class TableModelSeenChips : public QAbstractTableModel{
    Q_OBJECT
private:
    QVector<TableModelSeenChipsRow> rowData;
    QObject *parentObj=nullptr;
public:
    TableModelSeenChips(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool insertRow(TableModelSeenChipsRow value);
    void fireChange();
    bool removeRow(QModelIndex);
    void clear();
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
};

#endif // TABLEMODELSEENCHIPS_H
