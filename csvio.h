#ifndef CSVIO_H
#define CSVIO_H
#include <QDebug>
#include <iostream>
#include <fstream>
#include "account.h"
using namespace std;

class csvIO
{
    QString filename01;
    QString filename02;
    QString getFileName(QString path);
public:
    csvIO(QString filename01, QString filename02);
    QString getFilename() const;
    //ofstream& openFile(QString fileName, ofstream &ofstr);
    QString saveModifedFile(QMap<QString, Account> *map, QList<QString> *head, QString outPath);
    QString getDataFromCsv(QMap<QString, Account> *map, QMap<QString, QString> *accNumbers, QList<QString> *head);
};

#endif // CSVIO_H
