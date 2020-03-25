#ifndef TABLEMODELALLCHIPSROW_H
#define TABLEMODELALLCHIPSROW_H
#include <QString>

class TableModelKnownChipsRow {
private:
    QString bib;
public:
    TableModelKnownChipsRow(QString value);
    QString getBib() const;
    int getSnr() const;
    QString getName() const;
};

#endif // TABLEMODELALLCHIPSROW_H
