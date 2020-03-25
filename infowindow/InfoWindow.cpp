#include "InfoWindow.h"
#include "ui_InfoWindow.h"

InfoWindow::InfoWindow(QWidget *parent) : QDialog(parent), ui(new Ui::InfoWindow){
    ui->setupUi(this);
}
InfoWindow::~InfoWindow(){
    delete ui;
}

void InfoWindow::on_buttonClose_clicked(){
    this->close();
}
