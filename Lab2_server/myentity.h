#ifndef MYENTITY_H
#define MYENTITY_H

#include <QDebug>

class MyEntity
{

public:
    MyEntity();
    MyEntity(QString nickname, QString password);
    MyEntity(QString nickname, QString password, QString email, QString phoneNumber);

    QString getId();
    QString getNickname() const;
    QString getPassword();
    QString getEmail();
    QString getLastOnline();
    QString getPhoneNumber();
    void setId(QString id);
    void setNickname(QString id);
    void setPassword(QString id);
    void setEmail(QString id);
    void setLastOnline(QString lastOnline);
    void setPhoneNumber(QString phoneNumber);

private:
    QString id;
    QString nickname;
    QString password;
    QString email;
    QString lastOnline;
    QString phoneNumber;
};

#endif // MYENTITY_H
