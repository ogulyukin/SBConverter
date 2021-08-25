#include "csvio.h"
#include <QTextCodec>
#include <QFile>
#include <QTextStream>

QString csvIO::getFilename() const
{
    return filename02;
}

QString csvIO::saveModifedFile(QMap<QString, Account> *map, QList<QString> *head, QString outPath)
{

    QTextCodec *codec; //set codec for WIN coding
    codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForLocale(codec);
    QString temp = filename02;
    temp = getFileName(temp);
    temp.remove(temp.length() - 4, 4);
    temp += "_ready.txt";
    temp = outPath + "/" + temp;
    QFile target(temp);
    if (!target.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        return "Невозможно открыть файл " + temp + " для записи";
    }
    QTextStream ofstr(&target);
    ofstr.setCodec(codec); //Установка кодека для записи
    foreach(QString i, *head)
    {
        ofstr << i << "/n";
    }
    foreach(Account i, *map)
    {
        ofstr << i.getString() << "/n";
    }

     target.close();
     return "Результат записан в " + temp + " успешно.";
}

QString csvIO::getDataFromCsv(QMap<QString, Account> *map, QMap<QString, QString> *accNumbers, QList<QString> *head)
{
    QTextCodec *codec = QTextCodec::codecForName("cp1251");; //set codec for WIN coding
    QTextCodec::setCodecForLocale(codec);

    //Loading first file with Accounts
    QFile file(filename01);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Невозможно открыть файл с ЛС!";
        return "Невозможно открыть файл с ЛС!";
    }
    QTextStream in(&file);
    in.setCodec(codec);
    QString line = in.readLine();
    while(!line.isNull())
    {
        qDebug() << line;
        if (line != "")
        {
            QList<QString> list;
            list = (line.split(";"));
            if (list.length() < 2)
                return "Неверный формат файла с Лицевыми счетами!";
            accNumbers->insert(list.at(0),list.at(1));
        }
        line = in.readLine();
    }
    file.close();
    //Loading second file with Data
    QFile file2(filename02);
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Невозможно открыть файл с начислениями!";
        return "Невозможно открыть файл с начислениями!";
    }
    QTextStream in2(&file2);
    line = in2.readLine();
    while(!line.isNull())
    {
        qDebug() << line;
        if(line[0] == "#")
        {
            head->append(line);
        }else if (line != ""){
            QList<QString> list;
            list = (line.split(";"));
            if(list.length() < 4)
                return "Неверный формат файла с Начислениями!";
            Account *newAccount = new Account(list.at(0), list.at(1), list.at(2), list.at(3));
            map->insert(list.at(2), *newAccount);
        }
        line = in2.readLine();
    }
    file2.close();

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

csvIO::csvIO(QString filename01, QString filename02) : filename01(filename01), filename02(filename02)
{

}
