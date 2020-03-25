#include "configdb.h"
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QObject>
#include <QString>
#include <iostream>
#include <QDir>
#include <QSet>
#include <QTimer>
#include "classes/Holder.h"
ConfigDB* ConfigDB::instance=nullptr;

ConfigDB::ConfigDB(){
    instance=this;
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER)){
       db = QSqlDatabase::addDatabase(DRIVER);
       QDir dir;

        db.setDatabaseName(dir.absolutePath()+"/config.db");

        if(!db.open()) qWarning() << "ERROR: " << db.lastError();
        else{
            db.exec("CREATE TABLE bibs ([bib] TEXT, [snr] INTEGER)");
            db.exec("CREATE TABLE seenBibs ([bib] TEXT)");
            db.exec("CREATE TABLE snr2name ([snr] INTEGER,[name] TEXT)");
            db.exec("CREATE TABLE [config] ([key] TEXT,[value] TEXT)");
            {
                QSqlQuery query(db);
                query.exec("SELECT [value] FROM [config] WHERE [key]='UNUSEDSNR'");
                if(query.next())Holder::unusedSnrString=query.value(0).toString();
                else query.exec("INSERT INTO [config] ([value],[key]) VALUES('','UNUSEDSNR')");
                query.exec("SELECT [value] FROM [config] WHERE [key]='CURNAME'");
                if(query.next())Holder::curentName=query.value(0).toString();
                else query.exec("INSERT INTO [config] ([value],[key]) VALUES('','CURNAME')");
            }
            {
                QSqlQuery query(db);
                query.prepare("SELECT [bib],[snr] FROM [bibs]");
                query.exec();
                while(query.next()){
                    Holder::addBibSnr(query.value(0).toString(),query.value(1).toInt());
                }
            }
            {
                QSqlQuery query(db);
                query.prepare("SELECT [bib] FROM [seenBibs]");
                query.exec();
                while(query.next()){
                    Holder::addSeenBibs(query.value(0).toString());
                }
            }
            {
                QSqlQuery query(db);
                query.prepare("SELECT [snr],[name] FROM [snr2name]");
                query.exec();
                while(query.next()){
                    Holder::addSnrName(query.value(0).toInt(),query.value(1).toString());
                }
            }
        }
    }
}
ConfigDB::~ConfigDB(){}
ConfigDB *ConfigDB::getInstance(){
    return instance;
}
void ConfigDB::saveUnusedSnrString(){
    QSqlQuery query(db);
    query.prepare("UPDATE [config] SET [value]=:value WHERE [key]='UNUSEDSNR'");
    query.bindValue(":value",Holder::unusedSnrString);
    query.exec();
//    db.commit();
}
void ConfigDB::newBibsFired(){
    if(seenTimer==nullptr&&Holder::seenBibsNotSaved->size()>0){
        seenTimer=new QTimer();
        seenTimer->setSingleShot(true);
        seenTimer->singleShot(1000,this,SLOT(saveUnsavedSeen()));
//        QTimer::singleShot(2000,this,"saveUnsavedSeen");
    }
}
void ConfigDB::setBibs(QMap<QString,int> value){
    QMap<QString, int>::iterator i;
    db.transaction();
    QSqlQuery query(db);
    query.exec("DELETE FROM [bibs]");
    query.prepare("INSERT INTO [bibs] ([bib],[snr]) VALUES(:bib,:snr)");
    for(i=value.begin();i!=value.end();++i){
        query.bindValue(":bib",i.key());
        query.bindValue(":snr",i.value());
        query.exec();
//        std::cout<<i.key().toStdString()<<"    "<<i.value()<<std::endl;
    }
    db.commit();
}
void ConfigDB::saveUnsavedSeen(){
    seenTimer=nullptr;
    QSet<QString> bibs=*Holder::seenBibsNotSaved;
    Holder::seenBibsNotSaved=new QSet<QString>();
    QSet<QString>::iterator i;
    db.transaction();
    QSqlQuery query(db);
    query.prepare("INSERT INTO [seenBibs] (bib) VALUES(:bib)");
    for(i=bibs.begin();i!=bibs.end();++i){
        query.bindValue(":bib",*i);
        query.exec();
    }
    db.commit();
//    std::cout<<bibs.size()<<"   "<<Holder::seenChipNotSaved->size()<<std::endl;
}
void ConfigDB::clearSeenBibs(){
    db.exec("DELETE FROM [seenBibs]");
}
void ConfigDB::clearKnownBibs(){
    db.exec("DELETE FROM [bibs]");
}
void ConfigDB::clearSnr2Name(){
    getInstance()->db.exec("DELETE FROM [snr2name]");
}
void ConfigDB::setSnr2Name(QMap<int,QString> value){
    QMap<int,QString>::iterator i;
    getInstance()->db.transaction();
    QSqlQuery query(getInstance()->db);
    query.exec("DELETE FROM [snr2name]");
    query.prepare("INSERT INTO [snr2name] ([snr],[name]) VALUES(:snr,:name)");
    for(i=value.begin();i!=value.end();++i){
        query.bindValue(":snr",i.key());
        query.bindValue(":name",i.value());
        query.exec();
    }
    getInstance()->db.commit();
}
void ConfigDB::saveCurentName(){
    QSqlQuery query(getInstance()->db);
    query.prepare("UPDATE [config] SET [value]=:value WHERE [key]='CURNAME'");
    query.bindValue(":value",Holder::curentName);
    query.exec();
}
