#ifndef MYDAO_H
#define MYDAO_H

#include <QDebug>
#include <QtSql>
#include "myentity.h"
#include "messages.h"
#include "filehandler.h"

class MyDao
{
    //Q_OBJECT

public:
    MyDao();
    
    void loadDBCongigurations();

    //getters
    MyEntity getById(QString id);
    MyEntity getByNickname(QString nickname);
    MyEntity getByEmail(QString email);
    MyEntity getByPhoneNumber(QString phoneNumber);
    QStringList getContactNicknameListByUserId(QString id);
    QString getContactNicknameByUserId(QString id, QString userId);
    QString getLastOnline(MyEntity user);
    QList<QString> getAllPrivatesIdById(QString id1);
    QList<QString> getAllPublicsNamesById(QString id);
    QList<MyEntity> getAllPublicUsers(QString publicId);
    QString getChatIdByItsName(QString chatName);
    Messages getMessageById(QString id);
    QList<Messages> getNewMessagesToUserId(QString id);
    QList<Messages> getAllMessagesFromPrivate(QString user1Id, QString user2Id);
    QList<Messages> getAllMessagesFromPublic(QString publicId);
    QString getCurrentTime();
    int getMaxOnlineUsersCount();

    //setters
    void setEntity(MyEntity user);
    bool entityExecute(MyEntity user);
    void registerDataExecute(MyEntity user);
    void setPrivate(QString id, QString privateId);
    void setMessage(Messages message, bool isChat);
    void setNewChat(QString chatName);
    void setNewPublicUser(QString chatId ,QString userId);
    void setMaxOnlineUsersCount(int value);

    //delete
    void deleteById(QString id);
    void deleteByNickname(QString nickname);
    void deleteContact(QString id, QString contactId);
    void deleteMessageById(QString id);

    //update
    void updateEntity(MyEntity user);
    void addContact(QString id, QString contactId);
    void setContactName(QString id, QString contact_id, QString contactName);
    void updateLastOnline(QString nickname);

private:
    void connectToDB();
    QSqlDatabase db;
    QString dbDriverName;
    QString hostName;
    int port;
    QString dbName;
    QString dbUsername;
    QString dbPassword;

    MyEntity user;
    Messages message;
};

#endif // MYDAO_H
