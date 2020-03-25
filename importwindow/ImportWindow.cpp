#include "ImportWindow.h"
#include "ui_ImportWindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMap>
#include <iostream>
#include "classes/Holder.h"
#include "classes/configdb.h"

ImportWindow::ImportWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ImportWindow){
    ui->setupUi(this);
    ui->buttonGroupFileType->setId(ui->radioButton_CSV,0);
    ui->buttonGroupFileType->setId(ui->radioButton_Excel,1);
    ui->buttonGroupFileFormat->setId(ui->radioButton_bib,0);
    ui->buttonGroupFileFormat->setId(ui->radioButton_starter,1);
}

ImportWindow::~ImportWindow(){
    delete ui;
}

void ImportWindow::on_buttonCancel_clicked(){
    this->setResult(1);
    this->close();
}

void ImportWindow::on_buttonImport_clicked(){
    openFile(1);
}

void ImportWindow::on_buttonSearchFile_clicked(){
    QFileDialog fileDialog(this);
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    if(ui->buttonGroupFileType->checkedId()==0)fileDialog.setNameFilter(tr("CSV (*.csv)"));
    if(fileDialog.exec()){
        ui->lineEditSource->setText(fileDialog.selectedFiles().at(0));
    }
}

void ImportWindow::on_lineEditSource_textChanged(const QString /*&arg1*/){
    openFile(0);
}
void ImportWindow::openFile(int action){
    QFile file(ui->lineEditSource->text());
    if(file.exists()){

        //// IF CSV
        if(ui->buttonGroupFileType->checkedId()==0){

            //// IF Bib2Snr
            if(ui->buttonGroupFileFormat->checkedId()==0){
                QMap<QString,int> bib2snr;
                file.open(QFile::OpenModeFlag::ReadOnly);
                QByteArray buff;
                if(ui->checkBox_ignoreFirstLine->isChecked())file.readLine();
                while((buff=file.readLine()).size()>0){

                    QList<QByteArray> line=buff.split(';');
                    if(line.size()==2&&line.at(1).toInt()>0){
                        bib2snr.insert(line.at(0),line.at(1).toInt());
                    }
                    buff.clear();
                }
                file.close();
                ui->labelFoundSnrSize->setText(QString::number(bib2snr.size()));
                if(action==1){
                    Holder::clearBibSnr();
                    QMap<QString,int>::iterator i;
                    for(i=bib2snr.begin();i!=bib2snr.end();++i){
                        Holder::addBibSnr(i.key(),i.value());
                    }
                    ConfigDB::getInstance()->setBibs(bib2snr);
                    Holder::updateUsedSnr();
                    this->setResult(0);
                    this->close();
                }

            //// IF Snr2Name
            } else if(ui->buttonGroupFileFormat->checkedId()==1){
                QMap<int,QString> snr2name;
                file.open(QFile::OpenModeFlag::ReadOnly);
                QByteArray buff;
                if(ui->checkBox_ignoreFirstLine->isChecked())file.readLine();
                while((buff=file.readLine()).size()>0){
                    buff.replace('"',"");
                    QList<QByteArray> line=buff.split(';');
                    if(line.size()==2&&line.at(0).toInt()>0){
                        snr2name.insert(line.at(0).toInt(),QLatin1String(line.at(1)));
                        QString tmp=line.at(1);
                        std::cout<<line.at(1).toStdString()<<"    "<<tmp.toStdString()<<std::endl;
                    }
                    buff.clear();
                }
                file.close();
                ui->labelFoundSnrSize->setText(QString::number(snr2name.size()));
                if(action==1){
                    Holder::clearSnrName();
                    QMap<int,QString>::iterator i;
                    for(i=snr2name.begin();i!=snr2name.end();++i){
                        Holder::addSnrName(i.key(),i.value());
                    }
                    ConfigDB::setSnr2Name(snr2name);
                    this->setResult(0);
                    this->close();
                }
            }
        }
    }
}

void ImportWindow::on_checkBox_ignoreFirstLine_stateChanged(int arg1){
    Q_UNUSED(arg1);
    openFile(0);
}

void ImportWindow::on_radioButton_starter_clicked(){
    openFile(0);
}

void ImportWindow::on_radioButton_bib_clicked(){
    openFile(0);
}
