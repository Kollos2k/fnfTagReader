#ifndef IMPORTWINDOW_H
#define IMPORTWINDOW_H

#include <QDialog>

namespace Ui {
class ImportWindow;
}

class ImportWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ImportWindow(QWidget *parent = nullptr);
    ~ImportWindow();

private slots:
    void on_buttonCancel_clicked();
    void on_buttonImport_clicked();
    void on_buttonSearchFile_clicked();
    void on_lineEditSource_textChanged(const QString);

    void on_checkBox_ignoreFirstLine_stateChanged(int arg1);

    void on_radioButton_starter_clicked();

    void on_radioButton_bib_clicked();

private:
    Ui::ImportWindow *ui;
    void openFile(int action);
};

#endif // IMPORTWINDOW_H
