#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QObject>
class Account
{

    QString fio;
    QString els;
    QString fias;
    QString adress;
    QString accountNumber;
    QString summa;
    int pomNunber;

public:
    explicit Account(QString fio, QString adress, QString accountNumber, QString summa, QString fias = "");
    QString getString(QString period);
    void setEls(QString els);
    void setFias(QString fias);
    void setPomNumber(int number);
    bool operator==(const Account other)const
    {
        return this->accountNumber == other.accountNumber;
    }
    bool operator!=(const Account other)const
    {
        return this->accountNumber != other.accountNumber;
    }

    QString getAccountNumber()const;
};
#endif // ACCOUNT_H
