#include "myclient.h"
#include <QWidget>
#include <QMessageBox>

const QString MyClient::constNameUnknown = QString(".Unknown");

MyClient::MyClient(int desc, MyServer *serv, QObject *parent) :QObject(parent)
{
    _serv = serv;
    _isAutched = false;
    _name = constNameUnknown;
    _blockSize = 0;
    _sok = new QTcpSocket(this);

    _sok->setSocketDescriptor(desc);

    connect(_sok, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));

    qDebug() << "Client connected" << desc;
}

MyClient::~MyClient()
{

}

void MyClient::onConnect()
{
    //empty
}

void MyClient::onDisconnect()
{
    qDebug() << "Client disconnected";
    if (_isAutched)
    {
        _serv->updateLastOnlineOnDisconnect(_name);

        emit removeUserFromGui(_name);
        emit removeUser(this);
    }
    deleteLater();
}

void MyClient::onError(QAbstractSocket::SocketError socketError) const
{

    QWidget w;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(&w, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(&w, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(&w, "Error", "The following error occurred: "+_sok->errorString());
    }
}

void MyClient::onReadyRead()
{
    QDataStream in(_sok);

    if (_blockSize == 0) {

        if (_sok->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> _blockSize;
        qDebug() << "_blockSize now " << _blockSize;
    }

    if (_sok->bytesAvailable() < _blockSize)
        return;
    else

        _blockSize = 0;

    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    if (!_isAutched && command != comLoginReq && command != comSignupReq)
        return;

    switch(command)
    {

        case comLoginReq:
        {

            QString nickname;
            QString password;
            in >> nickname;
            in >> password;

            if(!_serv->isLoginDataValid(nickname, password)){
                doSendCommand(comAutchFail);
                return;
            }

            _name = nickname;
            _isAutched = true;
            doSendCommand(comAutchSuccess);
            addUserToGui(_name);
        }
        break;
    case comSignupReq:
    {
        QString nickname, password, email, phoneNumber;
        in >> nickname;
        in >> password;
        in >> email;
        in >> phoneNumber;

       if(! _serv->isSignupDataValid(nickname, password, email, phoneNumber)){
           doSendCommand(comAutchFail);
           return;
       }
       if(!_serv->signupDataToDBSave(nickname, password, email, phoneNumber))
       {
           doSendCommand(comAutchFail);
           return;
       }
        _isAutched = true;
        _name = nickname;
        doSendCommand(comAutchSuccess);
        addUserToGui(_name);
    }
    break;

        case comMessageToUsers:
        {
            //TODO
        }
        break;
    case comMsgTitlesReq:
    {
        QList<QString> privates = _serv->getPrivatesNicknames(_name);
        QList<QString> publics = _serv->getPublicsNames(_name);

        doSendChatTitles(comMsgTitlesResp, privates, publics);
    }
    break;
    case comPrivateHistoryReq:
    {
        QString user1, user2;
        in >> user1;
        in >> user2;
        QList<QString> list = _serv->getAllMessagesFromPrivateHistory(user1, user2);
        doSendMessagesHistory( comPrivateHistoryResp, list);
    }
    break;
    case comChatHistoryReq:
    {
        QString user, chatName;
        in >> user;
        in >> chatName;
        QList<QString> list = _serv->getAllMessagesFromChatHistory(user, chatName);
        doSendMessagesHistory( comChatHistoryResp, list);
    }
    break;
    case comNewMessageSent:
    {
        bool isChat;
        in >> isChat;
        QString toName, value;
        in >> toName;
        in >> value;
        
        _serv->doSendMessageToUsers(value, _name, toName, isChat);
        _serv->saveMessage(value, _name, toName, isChat);
    }
    break;
    case comNicknameExistanceCheck:
    {
        QString nickname;
        in >> nickname;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0;
        out << comNickExist;
        bool userExists = _serv->userExists(nickname);
        out << userExists;
        if(userExists) out << nickname;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
    }
    break;
    case comNewChatReq:
    {
        QString nickname;
        in >> nickname;

        _serv->addNewPrivateForUsers(_name, nickname);
        doSendCommand(comNewChatReqSuccess);
    }
    break;
    case comNewContactReq:
    {
        QString nickname;
        in >> nickname;

        _serv->addNewContact(_name, nickname);
        doSendCommand(comNewContactSuccess);
    }
    break;

    case comNewGroupChatReq:
    {
        QString chatName, nicknameCreatedTheChat;
        in >> chatName;
        in >> nicknameCreatedTheChat;

        _serv->createNewChat(chatName);
        _serv->setNewPublicUser(chatName, nicknameCreatedTheChat);

        doSendCommand(comNewGroupChatReqSuccess);
    }
    break;

    case comNewUserToChatReq:
    {
        QString chatName, newUserNickname;
        in >> chatName;
        in >> newUserNickname;

        _serv->setNewPublicUser(chatName, newUserNickname);

        doSendCommand(comNewUserToChatReqSuccess);
    }
    break;
    }
}

void MyClient::doSendCommand(quint8 comm) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
    qDebug() << "Send to" << _name << "command:" << comm;
}

void MyClient::doSendUsersOnline() const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comUsersOnline;
    QStringList l = _serv->getUsersOnline();
    QString s;
    for (int i = 0; i < l.length(); ++i)
        if (l.at(i) != _name)
            s += l.at(i)+(QString)",";
    s.remove(s.length()-1, 1);
    out << s;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
    qDebug() << "Sent user list to" << _name << ":" << s;
}

void MyClient::doSendChatTitles(quint8 comm, QList<QString> privates, QList<QString> publics){
    QString privatePrefix = "Private: ";
    QString publicPrefix = "Group: ";
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    QString s;
    for (int i = 0; i < privates.length(); ++i)
            s += privatePrefix+privates.at(i)+(QString)",";
    for (int j = 0; j < publics.length(); ++j)
            s += publicPrefix+publics.at(j)+(QString)",";
    s.remove(s.length()-1, 1);
    out << s;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MyClient::doSendMessagesHistory(quint8 command, QList<QString> list){

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << command;
    out << list.size();
    for(int i=0; i<list.size(); i++){
        out << list.at(i);
    }
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MyClient::doSendMessage(quint8 command, QString message) const{
    
    QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0;
        out << command;
        out << message;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        _sok->write(block);
}


