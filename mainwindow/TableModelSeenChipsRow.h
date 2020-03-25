#ifndef TABLEMODELSEENCHIPSROW_H
#define TABLEMODELSEENCHIPSROW_H
#include <QString>

class TableModelSeenChipsRow {
private:
    QString bib;
public:
    TableModelSeenChipsRow(QString value);
    QString getBib() const;
    int getSnr() const;
    QString getName() const;
};

#endif // TABLEMODELSEENCHIPSROW_H
