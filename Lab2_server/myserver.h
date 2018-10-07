#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QDebug>
#include "myclient.h"
#include "mydao.h"
#include "myentity.h"
#include "messages.h"
class MyClient;
class MyDao;
class MyEntity;
class Messages;

class MyServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit MyServer(QWidget *widget = 0, QObject *parent = 0);
    bool doStartServer(QHostAddress addr, qint16 port);
    void doSendToAllUserJoin(QString name);
    void doSendToAllUserLeft(QString name);
    void doSendToAllMessage(QString message, QString fromUsername);
    void doSendToAllServerMessage(QString message);
    void doSendServerMessageToUsers(QString message, const QStringList &users);
    void doSendMessageToUsers(QString value, QString fromId, QString toName, bool isChat) const;
    QStringList getUsersOnline() const;
    bool isNameValid(QString name) const;
    bool isNameUsed(QString name) const;
    bool isPasswordValid(QString password) const;
    bool isEmailValid(QString email) const;
    bool isLoginDataValid(QString nickname, QString password);
    bool isSignupDataValid(QString nickname, QString password, QString email, QString phoneNumber);
    bool signupDataToDBSave(QString nickname, QString password, QString email, QString phoneNumber);
    QList<QString> getPrivatesNicknames(QString nickname);
    void updateLastOnlineOnDisconnect(QString nickname);
    QList<QString> getPublicsNames(QString nickname);
    QList<QString> getAllMessagesFromPrivateHistory(QString user1, QString user2);
    QList<QString> getAllMessagesFromChatHistory(QString username, QString chatName);
    void saveMessage(QString value, QString fromId, QString toId, bool isChat);
    bool userExists(QString nick);
    void addNewPrivateForUsers(QString user1, QString user2);
    void addNewContact(QString user1, QString user2);
    void createNewChat(QString chatName);
    void setNewPublicUser(QString publicName, QString userName);
    int getOnlineCount();

signals:
    void addLogToGui(QString string, QColor color = Qt::black);

public slots:
    void onMessageFromGui(QString message, const QStringList &users);
    void onRemoveUser(MyClient *client);

protected:
    void incomingConnection(qintptr handle);

private:
    int maxOnlineUsersCount;

    QList<MyClient *> _clients;
    QWidget *_widget;
    MyDao *db;

};

#endif // MYSERVER_H
