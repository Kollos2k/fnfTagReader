#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include "classes/scanner.h"
#include <QString>
#include <QThread>
#include <QTextDocument>
#include <QPrinter>
#include <QDateTime>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QSet>
#include <QByteArray>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QRegularExpressionMatchIterator>
#include <QDesktopServices>
#include "TableModelKnownChips.h"
#include "TableModelKnownChipsProxy.h"
#include "TableModelKnownChipsRow.h"
#include "TableModelSeenChips.h"
#include "TableModelSeenChipsProxy.h"
#include "TableModelSeenChipsRow.h"
#include "TableModelGroupChips.h"
#include "TableModelGroupChipsProxy.h"
#include "TableModelGroupChipsRow.h"
#include "importwindow/ImportWindow.h"
#include "infowindow/InfoWindow.h"
#include "classes/Holder.h"
#include "classes/configdb.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    ui->readerSN->setText("");
    ui->lineEditGroupSize->setProperty("oldValue",10);
    Holder::updateUsedSnr();
    scanner=new Scanner();
    connect(scanner,SIGNAL(newChipIDSignal(QString)),this,SLOT(on_newChipIDReaded(QString)));
    connect(ui->actionImport,&QAction::triggered,this,&MainWindow::on_actionImport);
    connect(ui->actionClearSeenBibs,&QAction::triggered,this,&MainWindow::on_actionClearSeenBibs);
    connect(ui->actionInfoChipCounter,&QAction::triggered,this,&MainWindow::on_actionInfoChipCounter);
    connect(ui->actionClearKnownBibs,&QAction::triggered,this,&MainWindow::on_actionClearKnownBibs);
    connect(ui->actionClearKnownNames,&QAction::triggered,this,&MainWindow::on_actionClearKnownNames);
    connect(ui->actionPrintReportToPDF,&QAction::triggered,this,&MainWindow::on_actionPrintReportToPDF);

    tableModelKnownChips=new TableModelKnownChips(this);
    TableModelKnownChipsProxy *tableAllChipsModelProxy=new TableModelKnownChipsProxy(this);
    tableAllChipsModelProxy->setSourceModel(tableModelKnownChips);
    tableAllChipsModelProxy->setDynamicSortFilter(true);
    ui->tableKnownChips->setModel(tableAllChipsModelProxy);
    ui->tableKnownChips->sortByColumn(1,Qt::SortOrder::AscendingOrder);

    tableModelSeenChips=new TableModelSeenChips(this);
    QSet<QString>::iterator si;
    for(si=Holder::seenBibs->begin();si!=Holder::seenBibs->end();++si){
        tableModelSeenChips->insertRow(TableModelSeenChipsRow(*si));
    }
    TableModelSeenChipsProxy *tableModelSeenChipsProxy=new TableModelSeenChipsProxy(this);
    tableModelSeenChipsProxy->setSourceModel(tableModelSeenChips);
    tableModelSeenChipsProxy->setDynamicSortFilter(true);
    ui->tableSeenChips->setModel(tableModelSeenChipsProxy);

    tableModelGroupChips=new TableModelGroupChips(this);
    TableModelGroupChipsProxy *tableModelGroupChipsProxy=new TableModelGroupChipsProxy(this);
    tableModelGroupChipsProxy->setSourceModel(tableModelGroupChips);
    tableModelGroupChipsProxy->setDynamicSortFilter(true);
    ui->tableGroupChips->setModel(tableModelGroupChipsProxy);

//    tableModelGroupChips->data((tableModelGroupChips->rowCount()-1))
//    ui->tableGroupChips->resizeColumnToContents(1);
//    ui->tableGroupChips->resizeColumnToContents(2);
//    ui->tableKnownChips->setColumnWidth(3,300);
    ui->tableKnownChips->resizeColumnsToContents();
    ui->tableKnownChips->setColumnWidth(0,100);
    ui->tableGroupChips->resizeColumnsToContents();
//    ui->tableGroupChips->setColumnWidth(3,300);

    updateTableModelKnown();
    updateTableModelGroup();

    updateHeaderSeenCount();
    updateHeaderSnrUsed();
    updateHeaderSnrKnown();
    updateTableKnownStatus();
    updateTableSeenStatus();
    updateTableGroupStatus();
    ui->unusedSnrLineEdit->setProperty("userInput",false);
    ui->unusedSnrLineEdit->setText(Holder::unusedSnrString);
    ui->unusedSnrLineEdit->setProperty("userInput",true);
    ui->curentEventNameLE->setText(Holder::curentName);
}
MainWindow::~MainWindow(){
    scanner->disconnect();
    delete ui;
}


void MainWindow::on_readerConnectBtn_clicked(){
    if(scanner->isConnected()){
        scanner->disconnect();
        ui->readerSN->setText("");
        ui->readerConnectBtn->setText("connect");
    } else {
        if(scanner->connect()){
            ui->readerSN->setText(scanner->getSerialNumber());
            ui->readerConnectBtn->setText("disconnect");
        }
    }
    scanner->beebShort();
}

void MainWindow::on_readerReadBtn_clicked(){

//    sThread->start();
//    scanner->startScan();

    if(scanner->isRunning()){
        scanner->requestInterruption();
        ui->readerReadBtn->setText("start read");
    }else if(scanner->isConnected()){
        scanner->start(QThread::Priority::LowPriority);
        ui->readerReadBtn->setText("stop read");
    }
}
void MainWindow::on_newChipIDReaded(QString newChipID){
    if(!Holder::seenBibs->contains(newChipID)){
        Holder::addSeenBibs(newChipID,true);
        ConfigDB::getInstance()->newBibsFired();
        if(ui->readerBeeb->isChecked()){scanner->beebGreenLight();}
        else {scanner->greenLight();}
        updateHeaderSeenCount();
        tableModelKnownChips->fireChange();
        tableModelGroupChips->fireChange();
        tableModelSeenChips->insertRow(*new TableModelSeenChipsRow(newChipID));
        updateTableSeenStatus();
        updateTableKnownStatus();
//        std::cout<<newChipID.toStdString()<<std::endl;
    }
}
void MainWindow::updateHeaderSeenCount(){
    ui->headerBibSeenLbl->setText(QString::number(Holder::seenBibs->size()));
}

void MainWindow::updateHeaderSnrUsed(){
    ui->headerSnrUsedLbl->setText(QString::number(Holder::snr2bibUsed->size()));
}
void MainWindow::updateHeaderSnrKnown(){
    ui->headerSnrKnownLbl->setText(QString::number(Holder::snr2bib->size()));
}
void MainWindow::on_actionImport(){
    ImportWindow iw;
    if(iw.exec()==0){
        updateTableModelKnown();
        updateTableModelGroup();
        tableModelSeenChips->fireChange();
        updateHeaderSnrUsed();
        updateHeaderSnrKnown();
    }
}
void MainWindow::on_actionInfoChipCounter(){
    InfoWindow iw;
    iw.exec();
}
void MainWindow::on_actionClearSeenBibs(){
    Holder::clearSeenBibs();
    ConfigDB::getInstance()->clearSeenBibs();
    tableModelKnownChips->fireChange();
    tableModelSeenChips->clear();
    tableModelGroupChips->fireChange();
    updateHeaderSeenCount();
    updateTableKnownStatus();
    updateTableSeenStatus();
    updateTableGroupStatus();
}
void MainWindow::on_actionClearKnownBibs(){
    Holder::clearKnownBibs();
    ConfigDB::getInstance()->clearKnownBibs();
    tableModelKnownChips->clear();
    tableModelSeenChips->fireChange();
    tableModelGroupChips->clear();
    updateHeaderSeenCount();
    updateHeaderSnrUsed();
    updateHeaderSnrKnown();
    updateTableKnownStatus();
    updateTableSeenStatus();
    updateTableGroupStatus();
}
void MainWindow::on_actionClearKnownNames(){
    Holder::clearSnrName();
    ConfigDB::clearSnr2Name();
    tableModelKnownChips->fireChange();
    tableModelSeenChips->fireChange();
    updateHeaderSeenCount();
    updateTableKnownStatus();
    updateTableSeenStatus();
}
bool MainWindow::on_actionPrintReportToPDF(){
    QFile tempFile("templates/Report_Template.html");
    if(!tempFile.exists())return false;
    QFileDialog dialog;
//    dialog.setNameFilters(QDir::setFilter(tr("JPEG (*.jpg *.jpeg)")));
    QString exName="Report-"+Holder::curentName+".pdf";
    QString f=dialog.getSaveFileName(this,"Save Report",exName,"PDF (*.pdf)");
    if(f.size()>0){
        tempFile.open(QFile::ReadOnly);
        QString body;
        QTextDocument doc;
        QByteArray line;
        QString tmpBody;
        QDateTime curDate=QDateTime::currentDateTime();
        while((line=tempFile.readLine()).size()>0){
            tmpBody+=line;
            line.clear();
        }
        tempFile.close();
//        std::cout<<tmpBody.toStdString()<<std::endl;
//        tmpBody=tmpBody.simplified();
//        std::cout<<tmpBody.toStdString()<<std::endl;
        tmpBody.replace("%EVENTTITLE%",Holder::curentName);
        tmpBody.replace("%NOW%",curDate.toString("dd.MM.yyyy hh:mm:ss"));
        tmpBody.replace("%SEEN_TAGS_ALL_SIZE%",QString::number(Holder::seenBibs->size()));
        tmpBody.replace("%KNOWN_TAGS_SIZE%",QString::number(Holder::snr2bib->size()));
        tmpBody.replace("%UNUSED_SNR%",Holder::unusedSnrString);
        tmpBody.replace("%USED_TAGS_SIZE%",QString::number(Holder::snr2bibUsed->size()));
        tmpBody.replace("%USED_SEEN_TAGS_SIZE%",QString::number(Holder::seenBibsUsed->size()));
        tmpBody.replace("%UNUSED_TAGS_SIZE%",QString::number(Holder::snrUnused->size()));
        tmpBody.replace("%USED_NOTSEEN_TAGS_SIZE%",QString::number(Holder::snr2bibUsed->size()-Holder::seenBibsUsed->size()));


        if(Holder::snr2bib->size()==0){
            QRegularExpression regexBody("%BLOCK_WITHOUTKNOWN_BEGIN%(.*)%BLOCK_WITHOUTKNOWN_END%",QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatch match = regexBody.match(tmpBody);
            if(match.hasMatch()){
                QString tmpBlock=match.captured(1);
                QRegularExpression regexLoop("%LOOP_READED_TAGS_BEGIN%(.*)%LOOP_READED_TAGS_END%",QRegularExpression::DotMatchesEverythingOption);
                match=regexLoop.match(tmpBlock);
                if(match.hasMatch()){
                    QString tmpLoop;
                    QList<QString> sorted;
                    sorted=Holder::seenBibs->values();
                    std::sort(sorted.begin(),sorted.end());
                    QList<QString>::iterator i;
                    i=sorted.begin();
                    while(i!=sorted.end()){
                        QString curBibLine=match.captured(1).simplified();
                        curBibLine.replace("%CURTAG%",*i);
                        tmpLoop+=curBibLine;
                        ++i;
                    }
                    tmpBlock.replace(regexLoop,tmpLoop);
                }
                tmpBody.replace(regexBody,tmpBlock);
                regexBody.setPattern("%BLOCK_WITHKNOWN_BEGIN%(.*)%BLOCK_WITHKNOWN_END%");
                tmpBody.replace(regexBody,"");
            }
        } else{
            QRegularExpression regexBody("%BLOCK_WITHKNOWN_BEGIN%(.*)%BLOCK_WITHKNOWN_END%",QRegularExpression::DotMatchesEverythingOption);
            QRegularExpressionMatch match = regexBody.match(tmpBody);
            if(match.hasMatch()){
                QString tmpBlock=match.captured(1);
                QRegularExpression regexLoop("%LOOP_NOTSEEN_TAGS_BEGIN%(.*)%LOOP_NOTSEEN_TAGS_END%",QRegularExpression::DotMatchesEverythingOption);
                match=regexLoop.match(tmpBlock);
                if(match.hasMatch()){
                    QString tmpLoop;
                    QList<int>::iterator i;
                    i=Holder::snrSortedUsed->begin();
                    int notSeenCount=0;
                    while(i!=Holder::snrSortedUsed->end()){
                        QString curBibLine=match.captured(1).simplified();
                        auto find=Holder::snr2bib->find(*i);
                        auto nameI=Holder::snr2name->find(*i);
                        QString name=(nameI!=Holder::snr2name->end())?nameI.value():"";
                        if(find!=Holder::snr2bib->end()){
                        QString bib=find.value();
                            if(!Holder::seenBibs->contains(bib)){
                                notSeenCount++;
                                curBibLine.replace("%CURSNR%",QString::number(*i));
                                curBibLine.replace("%CURNAME%",name);
                                curBibLine.replace("%CURTAG%",bib);
                                tmpLoop+=curBibLine;
                            }
                        }
                        ++i;
                    }
                    tmpBlock.replace(regexLoop,tmpLoop);
                    regexLoop.setPattern("%NO_NOTSEEN_TAGS_BEGIN%(.*)%NO_NOTSEEN_TAGS_END%");
                    QString notSeenReplaceString="";
                    if(notSeenCount==0){
                        match=regexLoop.match(tmpBlock);
                        if(match.hasMatch()){
                            notSeenReplaceString=match.captured(1);
                        }
                    }
                    tmpBlock.replace(regexLoop,notSeenReplaceString);
                }
                tmpBody.replace(regexBody,tmpBlock);
                regexBody.setPattern("%BLOCK_WITHOUTKNOWN_BEGIN%(.*)%BLOCK_WITHOUTKNOWN_END%");
                tmpBody.replace(regexBody,"");
            }
        }
        doc.setHtml(tmpBody);
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOutputFormat(QPrinter::PdfFormat);

        printer.setOutputFileName(f);
        QFileInfo fileInfo;
        QFile qf(f);
        QDateTime lMod;
        if(qf.exists()){
            fileInfo.setFile(qf);
            lMod=fileInfo.lastModified();
        }
        doc.print(&printer);
        qf.setFileName(f);
        fileInfo.setFile(qf);
        if(!qf.exists()||fileInfo.lastModified().toMSecsSinceEpoch()==lMod.toMSecsSinceEpoch()){
            QMessageBox msg;
            msg.setText("Beim Speichern ist eine Fehler aufgetreten.");
            msg.exec();
        } else {
            QDesktopServices::openUrl(QUrl::fromLocalFile(f));
        }
    }
}
void MainWindow::on_buttonAppendGroupSize_clicked(){
    updateTableModelGroup();
    updateTableGroupStatus();
}
void MainWindow::updateTableModelKnown(){
    tableModelKnownChips->clear();
    QMap<QString,int>::iterator i;
    for(i=Holder::bib2snrUsed->begin();i!=Holder::bib2snrUsed->end();++i){
        tableModelKnownChips->insertRow(TableModelKnownChipsRow(i.key()));
    }
    updateTableKnownStatus();
}
void MainWindow::updateTableModelGroup(){
    tableModelGroupChips->clear();
    int grpSize=ui->lineEditGroupSize->text().toInt();
    int rows=Holder::snrSortedUsed->size()/grpSize;
    if(Holder::snrSortedUsed->size()%grpSize>0)rows++;
    for(int x=0;x<rows;x++)tableModelGroupChips->insertRow(TableModelGroupChipsRow(x,grpSize));
    updateTableGroupStatus();
}
void MainWindow::updateTableKnownStatus(){
    ui->tableKnownChipsStatus->setText("Gesehene: "+QString::number(Holder::seenBibsUsed->size())+" / "+QString::number(Holder::bib2snrUsed->size()));
}
void MainWindow::updateTableSeenStatus(){
    ui->tableSeenChipsStatus->setText("Gesehene Transponder: "+QString::number(Holder::seenBibs->size()));
}
void MainWindow::updateTableGroupStatus(){
    int seen=0;
    for(int x=0;x<tableModelGroupChips->rowCount();x++){
        if(tableModelGroupChips->isSeen(x))seen++;
    }
    ui->tableGroupChipsStatus->setText("Gruppen: "+QString::number(tableModelGroupChips->rowCount())+"  Gesehen: "+QString::number(seen));
}

void MainWindow::on_lineEditGroupSize_textChanged(const QString &arg1){
    QVariant oldValue=ui->lineEditGroupSize->property("oldValue");
    if(arg1.toInt()>0){
        ui->lineEditGroupSize->setProperty("oldValue",arg1.toInt());
    } else {
        ui->lineEditGroupSize->setText(oldValue.toString());
    }
}

void MainWindow::on_unusedSnrLineEdit_textChanged(const QString &arg1){
    Holder::snrUnused->clear();
    QStringList list=arg1.split(';');
    QStringList subList;
    QStringList::iterator i;
    i=list.begin();
    while(i!=list.end()){
        if(i->toInt()>0){
            Holder::snrUnused->insert(i->toInt());
        } else if(i->indexOf('-')>0) {
            subList=i->split('-');
            if(subList.size()==2&&subList.at(0).toInt()>0&&subList.at(1).toInt()>0&&subList.at(0).toInt()<=subList.at(1).toInt()){
                for(int x=subList.at(0).toInt();x<=subList.at(1).toInt();x++){
                    Holder::snrUnused->insert(x);
                }
            }
        }
        i++;
    }
    Holder::updateUsedSnr();
    updateTableModelKnown();
    updateTableModelGroup();
    updateTableKnownStatus();
    updateTableGroupStatus();
    updateHeaderSeenCount();
    updateHeaderSnrUsed();
    updateHeaderSnrKnown();

    if(ui->unusedSnrLineEdit->property("userInput").toBool())Holder::setUnusedSnrString(arg1);
}

void MainWindow::on_curentEventNameLE_textChanged(const QString &arg1){
    Holder::setCurentName(arg1);
}
