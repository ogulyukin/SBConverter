#include "csvio.h"
#include <QTextCodec>
#include <QFile>
#include <QtCore>

std::string csvIO::getFilename() const
{
    return filename02;
}

ofstream& csvIO::openFile(QString fileName, ofstream &ofstr)
{
    ofstr.open(fileName.toStdString());
    if(!ofstr.is_open())
    {
        qFatal("Can not open file for saving information!!!");
    }
    return ofstr;
}

QString csvIO::saveModifedFile(QMap<QString, Account> *map, QList<QString> *head, QString outPath)
{

    QTextCodec *codec; //set codec for WIN coding
    codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForLocale(codec);
    QString temp(filename02.c_str());
    temp = getFileName(temp);
    temp.remove(temp.length() - 4, 4);
    temp += "_ready.txt";
    temp = outPath + "/" + temp;
    QFile target(codec->fromUnicode(temp));
    if (!target.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return "Невозможно открыть файл " + temp + " для записи";
    }
    QTextStream ofstr(&target);
    ofstr.setCodec(codec); //Установка кодека для записи
    foreach(QString i, *head)
    {
        ofstr << i << endl;
    }
    foreach(Account i, *map)
    {
        ofstr << i.getString() << endl;
    }

     target.close();
     return "Результат записан в " + temp + " успешно.";
}

QString csvIO::getDataFromCsv(QMap<QString, Account> *map, QMap<QString, QString> *accNumbers, QList<QString> *head)
{
    QTextCodec *codec = QTextCodec::codecForName("cp1251");; //set codec for WIN coding
    QTextCodec::setCodecForLocale(codec);
    std::ifstream ifstr;
    std::ifstream ifstr2;
    ifstr.open(filename01);
    if(!ifstr.is_open())
    {
        return "Невозможно открыть файл с ЛС!";
    }
    QString reading;
    char temp[1000];
    while(!ifstr.eof())
    {
        ifstr.getline(temp,1000);
        reading = temp;
        if (reading != ""){
            QList<QString> list;
            list = (reading.split(";"));
            accNumbers->insert(list.at(0),list.at(1));
        }
    }
    ifstr.close();
    ifstr.open(filename02);
    if(!ifstr.is_open())
    {
        return "Невозможно открыть файл с начислениями!";
    }
//убрал отсюда
    while(!ifstr.eof())
    {
        ifstr.getline(temp,200);
        reading = codec->toUnicode(temp); //converting to UTF-8
        //qInfo() << reading;
        if(reading[0] == "#"){
            head->append(reading);
        }else if (reading != ""){
            QList<QString> list;
            list = (reading.split(";"));
            Account *newAccount = new Account(list.at(0), list.at(1), list.at(2), list.at(3));
            map->insert(list.at(2), *newAccount);
        }
    }
    ifstr.close();
//    qInfo() << "Now Testing...";
//    qInfo() << "************************************************************";
//    foreach(auto i , map->values()){
//        qInfo() << "Строка " << i.getString() << "\t Key: " << map->key(i);
//    }
//    qInfo() << "************************************************************";
//    foreach(auto i , accNumbers->values()){
//        qInfo() << "ELS: " << i << "\t LS: " << accNumbers->key(i);
//    }
//    qInfo() << "************************************************************";
//    foreach(auto i, *head)
//    {
//        qInfo() << i;
//    }
    return "OK";
}

QString csvIO::getFileName(QString path)
{
    QStringList list = path.split('/');
    if (list.length() < 2)
        return path;
    qDebug() << list.length();
    return list[list.length() - 1];
}

csvIO::csvIO(string filename01, string filename02) : filename01(filename01), filename02(filename02)
{

}
