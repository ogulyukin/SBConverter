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

public:
    explicit Account(QString fio, QString adress, QString accountNumber, QString summa, QString fias = "acde8f61-fd56-4cec-a4df-1f033e2a63ee");
    QString getString();
    void setEls(QString els);
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
