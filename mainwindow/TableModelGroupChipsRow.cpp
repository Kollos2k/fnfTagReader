#include "TableModelGroupChipsRow.h"
#include "classes/Holder.h"
#include <QSet>
#include <QList>

TableModelGroupChipsRow::TableModelGroupChipsRow(int valueRow, int groupSize){
    row=valueRow;
    snrSet=new QList<int>();
    int start=valueRow*groupSize;
    int end=start+groupSize-1;
    int count=0;
    QList<int>::iterator i;
    for(i=Holder::snrSortedUsed->begin();i!=Holder::snrSortedUsed->end();++i){
        if(count>=start){
            snrSet->append(*i);
            if(count==end)break;
        }
        count++;
    }
}
int TableModelGroupChipsRow::getColumn1() const{
    return snrSet->first();
}
int TableModelGroupChipsRow::getColumn2() const{
    return snrSet->last();
}
bool TableModelGroupChipsRow::getColumn3() const{
    QList<int>::iterator i;
    for(i=snrSet->begin();i!=snrSet->end();++i){
        if(Holder::snr2bibUsed->contains(*i))
        if(!Holder::seenBibs->contains(*Holder::snr2bib->find(*i))){
            return false;
        }
    }
    return true;
}
QString TableModelGroupChipsRow::getColumn4() const{
    int seenCount=0;
    QList<int>::iterator i;
    for(i=snrSet->begin();i!=snrSet->end();++i){
        if(Holder::snr2bibUsed->contains(*i))
        if(Holder::seenBibs->contains(*Holder::snr2bib->find(*i))){
            seenCount++;
        }
    }
    return QString::number(seenCount)+" / "+QString::number(snrSet->size());
}
QString TableModelGroupChipsRow::getColumn5() const{
    QString ret="";
    QList<int>::iterator i;
    for(i=snrSet->begin();i!=snrSet->end();++i){
        if(Holder::snr2bibUsed->contains(*i))
        if(!Holder::seenBibs->contains(*Holder::snr2bib->find(*i))){
            ret+=(ret.size()>0?",":"")+QString::number(*i);
        }
    }
    return ret;
}
bool TableModelGroupChipsRow::isSeen(){
    return getColumn3();
}
