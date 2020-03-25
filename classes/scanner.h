#ifndef SCANNER_H
#define SCANNER_H
#include "windows.h"
#include <QThread>
#include <QString>
#include <QObject>

class Scanner: public QThread{
    Q_OBJECT
private:
    HMODULE handle = LoadLibrary(TEXT("tagReader.dll"));
//    HMODULE handle = LoadLibrary(TEXT("C:\\Qt\\Projekte\\RRChipScanner\\src\\libs\\link\\rr\\tagReader.dll"));
    static Scanner* instance;
    int tagReaderID=0;
    bool connected=false;
    char* readerSerialNumber={};
    int curPower=2;
public:
    Scanner();
    static Scanner *getInstance(){return instance;}
    char* sendCommand(int cmd, char* data);
    bool connect();
    bool disconnect();
    bool isConnected();
    bool setReaderPower(int value);
    QString readChipID();
    void greenLight();
    void beebGreenLight();
    void beebShort();

    QString getSerialNumber();
//    static void scannerThread();
    void run();
public slots:
//    void process();
signals:
    void newChipIDSignal(QString);
//    void finished();
//    void error(QString err);
};

#endif // SCANNER_H
