#include "csvio.h"
#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include "xlsxdocument.h"

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
        ofstr << i << Qt::endl;
    }
    foreach(Account i, *map)
    {
        ofstr << i.getString() << Qt::endl;
    }

     target.close();
     return "Результат записан в " + temp + " успешно.";
}

QString csvIO::getDataFromCsv(QMap<QString, Account> *map, QMap<QString, QString> *accNumbers, QList<QString> *head)
{
    QTextCodec *codec = QTextCodec::codecForName("cp1251");; //set codec for WIN coding
    QTextCodec::setCodecForLocale(codec);

    //Loading first file with Accounts from xlsx файла
    QXlsx::Document doc(filename01);
    if (!doc.load())
    {
        qDebug() << "Невозможно открыть файл с ЛС!";
        return "Невозможно открыть файл с ЛС!";
    }
    int xCount = 2;
    QString acc01 = doc.read(xCount,1).toString();
    QString acc02 = doc.read(xCount,2).toString();
    while(acc01 != "")
    {
        //qDebug() << acc01 << "\t" << acc02;
        if (acc01 != "")
        {
            accNumbers->insert(acc01, acc02);
        }
        xCount++;
        acc01 = doc.read(xCount,1).toString();
        acc02 = doc.read(xCount,2).toString();
    }


    //Loading second file with Data
    QFile file2(filename02);
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Невозможно открыть файл с начислениями!";
        return "Невозможно открыть файл с начислениями!";
    }
    QTextStream in2(&file2);
    QString line = in2.readLine();
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
