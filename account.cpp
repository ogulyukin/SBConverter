#include "account.h"

QString Account::getAccountNumber() const
{
    return accountNumber;
}

Account::Account(QString fio, QString adress, QString accountNumber, QString rest, QString fias) : fias(fias)
{
    this->fio = fio;
    //this->els = els;
    this->adress = adress;
    this->accountNumber = accountNumber;
    this->rest = rest;
    this->pomNunber = "-1";
}

QString Account::getString()
{
    return fio + ";" + els + ";" + fias + ";" + adress + ";" + accountNumber + ";" + rest;
}

void Account::setEls(QString els)
{
    this->els = els;
}

void Account::setFias(QString fias)
{
    this->fias = fias;
}

void Account::setPomNumber(QString number)
{
    this->pomNunber = number;
}
