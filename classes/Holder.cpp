#include "Holder.h"
#include <iostream>
#include <QMap>
#include <QSet>
#include <QList>
#include "classes/configdb.h"
namespace Holder {
    QMap<QString,int> *bib2snr=new QMap<QString,int>();
    QMap<int,QString> *snr2bib=new QMap<int,QString>();

    QMap<QString,int> *bib2snrUsed=new QMap<QString,int>();
    QMap<int,QString> *snr2bibUsed=new QMap<int,QString>();
    QList<int> *snrSortedUsed=new QList<int>();

    QMap<int,QString> *snr2name=new QMap<int,QString>();

    QSet<int> *snrUnused=new QSet<int>();
    QSet<QString> *seenBibs=new QSet<QString>();
    QSet<QString> *seenBibsUsed=new QSet<QString>();
    QSet<QString> *seenBibsNotSaved=new QSet<QString>();
    QString unusedSnrString="";
    QString curentName="";
}
void Holder::setUnusedSnrString(QString value){
    unusedSnrString=value;
    ConfigDB::getInstance()->saveUnusedSnrString();
}
void Holder::addSeenBibs(QString bib, bool isNew){
    seenBibs->insert(bib);
    if(isNew)seenBibsNotSaved->insert(bib);
    if(bib2snrUsed->contains(bib)){
        seenBibsUsed->insert(bib);
    }
}
void Holder::clearSeenBibs(){
    seenBibs->clear();
    seenBibsUsed->clear();
    seenBibsNotSaved->clear();
}
bool Holder::sortSnrComposer(int a,int b){
    return a<b;
}
void Holder::sortSnr(){
    *snrSortedUsed=snr2bibUsed->keys();
    std::sort(snrSortedUsed->begin(),snrSortedUsed->end(),sortSnrComposer);
}
void Holder::addBibSnr(QString bib,int snr){
    bib2snr->insert(bib,snr);
    snr2bib->insert(snr,bib);
    if(!snrUnused->contains(snr)){
        bib2snrUsed->insert(bib,snr);
        snr2bibUsed->insert(snr,bib);
    }
    sortSnr();
}
void Holder::clearBibSnr(){
    bib2snr->clear();
    snr2bib->clear();
    bib2snrUsed->clear();
    snr2bibUsed->clear();
    snrSortedUsed->clear();
}
void Holder::updateUsedSnr(){
    bib2snrUsed->clear();
    snr2bibUsed->clear();
    seenBibsUsed->clear();
    QMap<QString,int>::iterator i;
    for(i=bib2snr->begin();i!=bib2snr->end();++i){
        if(!snrUnused->contains(i.value())){
            bib2snrUsed->insert(i.key(),i.value());
            snr2bibUsed->insert(i.value(),i.key());
            if(seenBibs->contains(i.key()))seenBibsUsed->insert(i.key());
        }
    }
    sortSnr();
}
void Holder::addSnrName(int snr,QString name){
    snr2name->insert(snr,name);
}
void Holder::clearSnrName(){
    snr2name->clear();
}
void Holder::clearKnownBibs(){
    bib2snr->clear();
    snr2bib->clear();
    bib2snrUsed->clear();
    snr2bibUsed->clear();
    snrSortedUsed->clear();
}
void Holder::setCurentName(QString value){
    Holder::curentName=value;
    ConfigDB::saveCurentName();
}
