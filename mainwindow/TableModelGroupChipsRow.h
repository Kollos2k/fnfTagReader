#ifndef TABLEMODELGROUPCHIPSROW_H
#define TABLEMODELGROUPCHIPSROW_H
#include <QString>
#include <QMap>
#include <QSet>
#include <QList>

class TableModelGroupChipsRow {
private:
    int row;
    QList<int> *snrSet;
public:
    TableModelGroupChipsRow(int valueRow,int groupSize);
    int getColumn1() const;
    int getColumn2() const;
    bool getColumn3() const;
    QString getColumn4() const;
    QString getColumn5() const;
    bool isSeen();
};

#endif // TABLEMODELGROUPCHIPSROW_H
