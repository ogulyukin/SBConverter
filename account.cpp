#include "account.h"

QString Account::getAccountNumber() const
{
    return accountNumber;
}

Account::Account(QString fio, QString adress, QString accountNumber, QString summa, QString fias) : fias(fias)
{
    this->fio = fio;
    //this->els = els;
    this->adress = adress;
    this->accountNumber = accountNumber;
    this->summa = summa;
}

QString Account::getString()
{
    return fio + ";" + els + ";" + fias + ";" + adress + ";" + accountNumber + ";" + summa;
}

void Account::setEls(QString els)
{
    this->els = els;
}

void Account::setFias(QString fias)
{
    this->fias = fias;
}
