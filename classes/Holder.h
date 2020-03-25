#ifndef HOLDER_H
#define HOLDER_H
#include <QMap>
#include <QSet>
namespace Holder {
    extern QMap<QString,int> *bib2snr;
    extern QMap<int,QString> *snr2bib;
    extern QMap<QString,int> *bib2snrUsed;
    extern QMap<int,QString> *snr2bibUsed;
    extern QList<int> *snrSortedUsed;
    extern QSet<int> *snrUnused;
    extern QMap<int,QString> *snr2name;
    extern QSet<QString> *seenBibs;
    extern QSet<QString> *seenBibsUsed;
    extern QSet<QString> *seenBibsNotSaved;
    extern QString unusedSnrString;
    extern QString curentName;
    void sortSnr();
    bool sortSnrComposer(int a,int b);
    void addBibSnr(QString bib,int snr);
    void addSnrName(int snr,QString name);
    void clearSnrName();
    void clearBibSnr();
    void updateUsedSnr();
    void addSeenBibs(QString bib,bool isNew=false);
    void clearSeenBibs();
    void clearKnownBibs();
    void setUnusedSnrString(QString value);
    void setCurentName(QString value);
}
#endif // HOLDER_H
