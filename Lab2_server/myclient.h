#ifndef MYCLIENT_H
#define MYCLIENT_H

#include <QObject>
#include <QList>
#include <QDebug>
#include <QTcpSocket>
#include <QThreadPool>
#include <QtGui>
#include <QMessageBox>
#include <QRegExp>
#include "myserver.h"
#include "myentity.h"

class MyServer;
class MyEntity;

class MyClient : public QObject
{
    friend class MyServer;
    Q_OBJECT

public:
    static const QString constNameUnknown;
    static const quint8 comLoginReq = 0;
    static const quint8 comSignupReq = 1;
    static const quint8 comUsersOnline = 2;
    static const quint8 comUserJoin = 3;
    static const quint8 comUserLeft = 4;
    static const quint8 comMessageToUsers = 6;
    static const quint8 comPublicServerMessage = 7;
    static const quint8 comPrivateServerMessage = 8;
    static const quint8 comAutchSuccess = 9;
    static const quint8 comErrNameInvalid = 201;
    static const quint8 comErrNameUsed = 202;
    static const quint8 comMsgTitlesReq = 203;
    static const quint8 comMsgTitlesResp = 204;
    static const quint8 comAutchFail = 205;
    static const quint8 comNewMessagesReq = 206;
    static const quint8 comPrivateHistoryReq = 207;
    static const quint8 comChatHistoryReq = 208;
    static const quint8 comPrivateHistoryResp = 209;
    static const quint8 comChatHistoryResp = 210;
    static const quint8 comNewMessageSent = 211;
    static const quint8 comNewMessageIn = 212;
    static const quint8 comNicknameExistanceCheck = 213;
    static const quint8 comNickExist = 214;
    static const quint8 comNewChatReq = 215;
    static const quint8 comNewChatReqSuccess = 216;
    static const quint8 comNewContactReq = 217;
    static const quint8 comNewContactSuccess = 218;
    static const quint8 comNewContactFail = 219;
    static const quint8 comNewGroupChatReq = 220;
    static const quint8 comNewGroupChatReqSuccess = 221;
    static const quint8 comNewUserToChatReq = 222;
    static const quint8 comNewUserToChatReqSuccess = 223;

    explicit MyClient(int desc, MyServer *serv, QObject *parent = 0);
    ~MyClient();
    void setName(QString name) {_name = name;}
    QString getName() const {return _name;}
    bool getAutched() const {return _isAutched;}
    void doSendCommand(quint8 comm) const;
    void doSendUsersOnline() const;
    void doSendChatTitles(quint8 comm, QList<QString> privates, QList<QString> publics);
    void doSendMessagesHistory(quint8 comm, QList<QString> list);
    void doSendMessage(quint8 comm, QString message) const;

signals:
    void addUserToGui(QString name);
    void removeUserFromGui(QString name);
    void removeUser(MyClient *client);
    void messageToGui(QString message, QString from, const QStringList &users);

private slots:
    void onConnect();
    void onDisconnect();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError socketError) const;

private:
    QTcpSocket *_sok;
    MyServer *_serv;
    quint16 _blockSize;
    QString _name;
    bool _isAutched;

};

#endif // MYCLIENT_H
