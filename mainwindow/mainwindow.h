#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLibrary>
#include <QSet>
#include "classes/scanner.h"
#include "TableModelKnownChips.h"
#include "TableModelSeenChips.h"
#include "TableModelGroupChips.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_readerConnectBtn_clicked();
    void on_readerReadBtn_clicked();
    void on_buttonAppendGroupSize_clicked();

    void on_lineEditGroupSize_textChanged(const QString &arg1);

    void on_unusedSnrLineEdit_textChanged(const QString &arg1);

    void on_curentEventNameLE_textChanged(const QString &arg1);

public slots:
    void on_newChipIDReaded(QString newChipID);
    void on_actionImport();
    void on_actionClearSeenBibs();
    void on_actionClearKnownBibs();
    void on_actionClearKnownNames();
    void on_actionInfoChipCounter();
    bool on_actionPrintReportToPDF();

private:
    Ui::MainWindow *ui;
    QLibrary* handle;
    Scanner* scanner;
    void updateHeaderSeenCount();
    void updateHeaderSnrUsed();
    void updateHeaderSnrKnown();
    void updateTableModelKnown();
    void updateTableModelGroup();
    void updateTableKnownStatus();
    void updateTableSeenStatus();
    void updateTableGroupStatus();
    TableModelKnownChips *tableModelKnownChips;
    TableModelSeenChips *tableModelSeenChips;
    TableModelGroupChips *tableModelGroupChips;

//    QThread* sThread;
};
#endif // MAINWINDOW_H
