#ifndef CSVIO_H
#define CSVIO_H
#include <QDebug>
#include "account.h"
#include <QRandomGenerator>

class csvIO
{
    QString filename01;
    QString filename02;
    QString getFileName(QString path);
public:
    static void loadFiasCodes(QHash<QString, QString> &list, QHash<QString, QString> &numbers, QString &fileName01, QString &fileName02);
    csvIO(QString filename01);
    QString getFilename() const;
    void setOutFileName(QString filename);
    QString saveModifedFile(QMap<QString, Account> *map, QString period, QString outPath);
    QString getDataFromFiles(QMap<QString, Account> *map, QList<QString> *head,
                             QString defFias, QHash<QString, QString> *fiasCodes);
    static bool getBankAccountsFromFile(QHash<QString, QString> *bankAccounts);
private:
//    QString findFias(QString adress, QString defFias);
    int getFlat(QString fullAdress);
    QHash<QString, QString> houseFias;
    QString getFias(QString account, QHash<QString, QString> *list, QString defFias, QString adress);
    static QString getHouseAdress(QString fulladress);
};

#endif // CSVIO_H
