#ifndef TABLEMODELALLCHIPSPROXY_H
#define TABLEMODELALLCHIPSPROXY_H
#include <QSortFilterProxyModel>

#include <QObject>

class TableModelKnownChipsProxy : public QSortFilterProxyModel{
    Q_OBJECT
public:
    TableModelKnownChipsProxy(QObject *parent = 0);
//    bool filterAcceptsRow(int source_row,const QModelIndex &source_parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation,int role) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool lessThan(const QModelIndex &left,const QModelIndex &right) const override;
};

#endif // TABLEMODELALLCHIPSPROXY_H
