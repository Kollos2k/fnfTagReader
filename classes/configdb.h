#ifndef CONFIGDB_H
#define CONFIGDB_H
#include <QSqlDatabase>
#include <QString>
#include <QObject>
#include <QTimer>

class ConfigDB : public QObject {
    Q_OBJECT
protected:
public:
    ConfigDB();
    ~ConfigDB();
    static ConfigDB *getInstance();
    void setBibs(QMap<QString,int> value);
    void loadData();
    void clearSeenBibs();
    void clearKnownBibs();
    static void clearSnr2Name();
    void newBibsFired();
    void saveUnusedSnrString();
    static void setSnr2Name(QMap<int,QString> value);
    static void saveCurentName();
private:
    static ConfigDB* instance;
    QSqlDatabase db;
    QTimer *seenTimer=nullptr;
public slots:
    void saveUnsavedSeen();
};

#endif // CONFIGDB_H
