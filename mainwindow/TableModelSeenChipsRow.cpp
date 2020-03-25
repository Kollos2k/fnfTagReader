#include "TableModelSeenChipsRow.h"
#include "classes/Holder.h"

TableModelSeenChipsRow::TableModelSeenChipsRow(QString value){bib=value;}
QString TableModelSeenChipsRow::getBib() const{return bib;}
int TableModelSeenChipsRow::getSnr() const{
    auto i=Holder::bib2snr->find(bib);
    if(i!=Holder::bib2snr->end())return i.value();
    return 0;
}
QString TableModelSeenChipsRow::getName() const{
    int snr=getSnr();
    if(snr>0){
        auto i=Holder::snr2name->find(snr);
        if(i!=Holder::snr2name->end())return i.value();
    }
    return QString();
}
