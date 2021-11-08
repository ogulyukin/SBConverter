#ifndef CSVIO_H
#define CSVIO_H
#include <QDebug>
#include "account.h"
#include <QRandomGenerator>

class csvIO
{
    QString filename01;
    QString getFileName(QString path);
public:
    static void loadFiasCodes(QHash<QString, QString> &list, QHash<QString, QString> &numbers, QString &fileName01, QString &fileName02);
    csvIO(QString filename01);
    QString getFilename() const;
    QString saveModifedFile(QMap<QString, Account> *map, QList<QString> *head, QString outPath);
    QString getDataFromFiles(QMap<QString, Account> *map, QList<QString> *head,
                             QString defFias, QHash<QString, QString> *fiasCodes);
private:
//    QString findFias(QString adress, QString defFias);
    QString getFlat(QString fullAdress);
    QHash<QString, QString> houseFias;
    QString getFias(QString account, QHash<QString, QString> *list,/* QString defFias, */QString adress);
    static QString getHouseAdress(QString fulladress);
};

#endif // CSVIO_H
