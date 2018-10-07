#include "myserver.h"
#include <iostream>

MyServer::MyServer(QWidget *widget, QObject *parent) :QTcpServer(parent)
{
    _widget = widget;
    db = new MyDao();
    maxOnlineUsersCount = db->getMaxOnlineUsersCount();
}

bool MyServer::doStartServer(QHostAddress addr, qint16 port)
{
    if (!listen(addr, port))
    {
        qDebug() << "Server not started at" << addr << ":" << port;
        return false;
    }
    qDebug() << "Server running at" << addr << ":" << port;
    return true;
}

void MyServer::doSendToAllUserJoin(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    //write message in buffer that user has joined
    out << (quint16)0 << MyClient::comUserJoin << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void MyServer::doSendToAllUserLeft(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comUserLeft << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() != name && _clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void MyServer::doSendToAllServerMessage(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comPublicServerMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getAutched())
            _clients.at(i)->_sok->write(block);
}

void MyServer::doSendServerMessageToUsers(QString message, const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << MyClient::comPrivateServerMessage << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int j = 0; j < _clients.length(); ++j)
        if (users.contains(_clients.at(j)->getName()))
            _clients.at(j)->_sok->write(block);
}

void MyServer::doSendMessageToUsers(QString value, QString fromId, QString toName, bool isChat) const
{
    QList<MyClient *> list;
    QString data = "";
    data += (isChat) ? "1" : "0";
    data += "}|{";
    data += toName;
    data += "}|{";
    data += fromId;
    data += "}|{";
    data += value;
    data += "}|{";
    data += db->getCurrentTime();

    if(isChat){
        QList<MyEntity> users = db->getAllPublicUsers(db->getChatIdByItsName(toName));
        for(int i=0; i<_clients.size();i++){

            for(int j=0; j < users.size(); j++){

                if(_clients.at(i)->_name == users.at(j).getNickname()){
                    list.append(_clients.at(i));
                }
            }
        }

        for(int i=0; i<list.size(); i++) list.at(i)->doSendMessage(MyClient::comNewMessageIn, data);

    }
    else {
        //if private message
        for(int i=0; i<_clients.size(); i++){
            if(_clients.at(i)->_name == fromId || _clients.at(i)->_name == toName){
                _clients.at(i)->doSendMessage(MyClient::comNewMessageIn, data);
            }
        }
    }
}

QStringList MyServer::getUsersOnline() const
{
    QStringList l;
    foreach (MyClient * c, _clients)
        if (c->getAutched())
            l << c->getName();
    return l;
}

bool MyServer::isNameValid(QString name) const
{
    if (name.length() > 30 || name.length() < 4)
        return false;
    QRegExp r("[A-Za-z0-9_]+");

    MyEntity user = db->getByNickname(name);

    return r.exactMatch(name) && user.getNickname() == NULL;
}

bool MyServer::userExists(QString nick){

    QString us = db->getByNickname(nick).getNickname();
    bool exists = !(db->getByNickname(nick).getNickname().isNull());

    return exists;
}

void MyServer::addNewPrivateForUsers(QString user1, QString user2){
    db->setPrivate(user1, user2);
    db->setPrivate(user2, user1);
}

void MyServer::addNewContact(QString user1, QString user2){
    db->addContact(user1, user2);
    db->addContact(user2, user1);
}

//
bool MyServer::isPasswordValid(QString password) const
{
    if (password.length() < 5)
        return false;
    return true;
}

bool MyServer::isEmailValid(QString email) const{

    MyEntity user = db->getByEmail(email);

    bool consistsDog = email.indexOf('@') != -1;
    bool firstUser = user.getEmail() == NULL;

    if(!consistsDog || !firstUser){
        return false;
    }
    return true;
}

//

bool MyServer::isNameUsed(QString name) const
{
    for (int i = 0; i < _clients.length(); ++i)
        if (_clients.at(i)->getName() == name)
            return true;
    return false;
}

void MyServer::incomingConnection(qintptr handle)
{
    MyClient *client = new MyClient(handle, this, this);

    connect(client, SIGNAL(addUserToGui(QString)), _widget, SLOT(onAddUserToGui(QString)));
    connect(client, SIGNAL(removeUserFromGui(QString)), _widget, SLOT(onRemoveUserFromGui(QString)));
    connect(client, SIGNAL(removeUser(MyClient*)), this, SLOT(onRemoveUser(MyClient*)));
    _clients.append(client);
    if(_clients.size() > maxOnlineUsersCount){
        maxOnlineUsersCount = _clients.size();
        db->setMaxOnlineUsersCount(maxOnlineUsersCount);
    }
}

void MyServer::onRemoveUser(MyClient *client)
{
    _clients.removeAt(_clients.indexOf(client));
}

void MyServer::onMessageFromGui(QString message, const QStringList &users)
{
    if (users.isEmpty())
        doSendToAllServerMessage(message);
    else
        doSendServerMessageToUsers(message, users);
}

bool MyServer::isLoginDataValid(QString nickname, QString password){
    MyEntity user;

    user = db->getByNickname(nickname);
    if(user.getNickname() == nickname && user.getPassword() == password)
        return true;

    return false;
}

bool MyServer::isSignupDataValid(QString nickname, QString password, QString email, QString phoneNumber){
    if(isNameValid(nickname) && isPasswordValid(password) && isEmailValid(email))
        return true;

    return false;
}

bool MyServer::signupDataToDBSave(QString nickname, QString password, QString email, QString phoneNumber){
    MyEntity user;
    user.setNickname(nickname);
    user.setPassword(password);
    user.setEmail(email);
    user.setPhoneNumber(phoneNumber);

    if(!db->entityExecute(user)) return false;
    return true;
}

QList<QString> MyServer::getPrivatesNicknames(QString nickname){
    MyEntity user = db->getByNickname(nickname);
    QString id = user.getId();
    QList<QString> privates = db->getAllPrivatesIdById(id);
    QList<QString> nicknames;
    for(int i =0; i<privates.size(); i++){
        MyEntity user2 = db->getById(privates.at(i));
        nicknames.append(user2.getNickname());
    }
    return nicknames;
}

QList<QString> MyServer::getPublicsNames(QString nickname){
    MyEntity user = db->getByNickname(nickname);
    QString id = user.getId();
    QList<QString> names = db->getAllPublicsNamesById(id);

    return names;
}

void MyServer::updateLastOnlineOnDisconnect(QString nickname){
    db->updateLastOnline(nickname);
}

QList<QString> MyServer::getAllMessagesFromPrivateHistory(QString user1, QString user2){
    QList<Messages> messageList = db->getAllMessagesFromPrivate(db->getByNickname(user1).getId(), db->getByNickname(user2).getId());
    QList<QString> messageData;
    for(int i=0; i<messageList.size(); i++){
        Messages msg = messageList.at(i);
        QString data = "";
        data += db->getById(msg.getFromId()).getNickname();
        data += "}|{";
        data += msg.getContent();
        data += "}|{";
        data += msg.getCreatedAt();
        messageData.append(data);
    }
    return messageData;
}

QList<QString> MyServer::getAllMessagesFromChatHistory(QString username, QString chatName){
    QString chatId = db->getChatIdByItsName(chatName);
    QList<Messages> messageList = db->getAllMessagesFromPublic(chatId);
    QList<QString> messageData;
    for(int i=0; i<messageList.size(); i++){
        Messages msg = messageList.at(i);
        QString data = "";
        data += db->getById(msg.getFromId()).getNickname();
        data += "}|{";
        data += msg.getContent();
        data += "}|{";
        data += msg.getCreatedAt();
        messageData.append(data);
    }
    return messageData;
}

void MyServer::saveMessage(QString value, QString fromId, QString toId, bool isChat){

    Messages message;
    message.setFromId(db->getByNickname(fromId).getId());

    if(isChat) message.setToChatId(db->getChatIdByItsName(toId));
    else message.setToId(db->getByNickname(toId).getId());

    message.setContent(value);

    db->setMessage(message, isChat);
}

void MyServer::createNewChat(QString chatName){
    db->setNewChat(chatName);
}

void MyServer::setNewPublicUser(QString publicName, QString userName){

    QString userId = db->getByNickname(userName).getId();
    if(userId.isNull()) return;
    db->setNewPublicUser(db->getChatIdByItsName(publicName), userId);
}

int MyServer::getOnlineCount(){
    return _clients.size();
}

