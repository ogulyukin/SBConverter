#include "csvio.h"
#include <QTextCodec>
#include <QFile>
#include <QTextStream>

#include "xlsxdocument.h"

csvIO::csvIO(QString filename01) : filename01(filename01)
{

}

QString csvIO::getFilename() const
{
    return filename01;
}

QString csvIO::saveModifedFile(QMap<QString, Account> *map, QList<QString> *head, QString outPath)
{

    QTextCodec *codec; //set codec for WIN coding
    codec = QTextCodec::codecForName("cp1251");
    QTextCodec::setCodecForLocale(codec);
    QString temp = filename01;
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

QString csvIO::getDataFromFiles(QMap<QString, Account> *map, QList<QString> *head, QString defFias,
                                QHash<QString, QString> *fiasCodes)
{
    QTextCodec *codec = QTextCodec::codecForName("cp1251");; //set codec for WIN coding
    QTextCodec::setCodecForLocale(codec);
    //Loading first file with Accounts from xlsx файла
    //moved to static function
    //Loading second file with Data
    QFile file2(filename01);
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
            // Account(QString fio, QString adress, QString accountNumber, QString summa, QString fias = "");
            Account *newAccount = new Account(list.at(0), list.at(1), list.at(2), list.at(3),
                                              getFias(list.at(2), fiasCodes, defFias, list.at(1)));
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

void csvIO::loadFiasCodes(QHash<QString, QString> &numberFias, QHash<QString, QString> &numberEls,
                          QString &accountsF, QString &codesF)
{
    QHash<QString, QString> accounts;
    QHash<QString, QString> codes;
    QXlsx::Document doc(codesF);
    if (!doc.load())
    {
        qDebug() << "Невозможно открыть файл с Объектами учета!";
        return;
    }
    int xCount = 3;
    QString fileAdress = doc.read(xCount,1).toString();
    QString fileFias = doc.read(xCount,3).toString();
    QString isPom = doc.read(xCount,20).toString();
    while(fileAdress != "")
    {
        if(isPom == "")
        {
            //qDebug() << "Записан ФИАС для Дома: " << fileFias << " " << fileAdress << " " << QString::number(xCount);
            codes.insert(fileAdress, fileFias);
        }
        //qDebug() << "Обработано: " << QString::number(xCount) << fileAdress;
        xCount++;
        fileAdress = doc.read(xCount,1).toString();
        fileFias = doc.read(xCount,3).toString();
        isPom = doc.read(xCount,20).toString();
    }
    QXlsx::Document doc2(accountsF);
    if (!doc2.load())
    {
        qDebug() << "Невозможно открыть файл с Объектами учета!";
        return;
    }
    xCount = 2;
    QString numberAcc = doc2.read(xCount,1).toString();
    QString numberAcc2 = doc2.read(xCount,2).toString();
    QString adressAcc = doc2.read(xCount,5).toString();
    while(numberAcc != "")
    {
        accounts.insert(numberAcc, csvIO::getHouseAdress(adressAcc));
        numberEls.insert(numberAcc, numberAcc2);
        xCount++;
        numberAcc = doc2.read(xCount,1).toString();
        numberAcc2 = doc2.read(xCount,2).toString();
        adressAcc = doc2.read(xCount,5).toString();
        //qDebug() << "Загрузка данных ЛС: " << numberAcc << numberAcc2;
    }

    for(auto it = accounts.begin(); it != accounts.end(); it++)
    {
        if(codes.contains(it.value()))
        {
            numberFias.insert(it.key(), codes.value(it.value()));
            //qDebug() << "Add to numerFias: " << it.value() << "\t" << codes.value(it.key());
        }
    }
    qDebug() << "Totall in numberFias: " << numberFias.count();
}

int csvIO::getFlat(QString fullAdress)
{
    QStringList list = fullAdress.split(',');
    return (list.last()).toInt();
}

QString csvIO::getFias(QString account, QHash<QString, QString> *list, QString defFias, QString adress)
{
    int pomNumber = getFlat(adress);
    auto it = list->find(account.toUpper());
    if(it != list->end())
    {
        QString fias = it.value() + "," + QString::number(pomNumber);
        //qDebug() << "Фиас идентифицирован для " << account;
        return fias;
    }else
    {
        it = list->find(account.toLower());
        if(it != list->end())
        {
            QString fias = it.value() + "," + QString::number(pomNumber);
            //qDebug() << "Фиас идентифицирован для " << account;
            return fias;
        }
    }
    return defFias + "," + QString::number(pomNumber);
}

QString csvIO::getHouseAdress(QString fulladress)
{
    int pos = fulladress.lastIndexOf(",");
    if (pos < 1)
    {
        //qDebug() << "Не нашли последнюю запиточку!";
        return "";
    }
    QString result = fulladress.left(pos);
    //qDebug() << result;
    return result;
}
