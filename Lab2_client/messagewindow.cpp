#include "messagewindow.h"
#include "ui_messagewindow.h"

MessageWindow::MessageWindow(QWidget *parent) :
    QDialog(parent),
    frame(new Ui::MessageWindow)
{
    frame->setupUi(this);
}

MessageWindow::~MessageWindow()
{
    delete frame;
}

void MessageWindow::setAuthenticationInstance(Authentication *auth){
    this->auth = auth;
}

void MessageWindow::setMainWindowInstance(MainWindow *mainWindow){
    this->mainWindow = mainWindow;
}

void MessageWindow::setSok(QTcpSocket *_sok){
    this->_sok = _sok;
}

void MessageWindow::setName(QString name){
    this->_name = name;
}

void MessageWindow::configurationSetup(QString header, bool isChat){
    if(!isChat) frame->pbAddNewChatUser->hide();
    frame->widgetNewChatUserMessage->hide();
    frame->twMessage->clear();
    frame->twMessage->clearContents();
    frame->twMessage->clearSpans();
    frame->twMessage->clearSelection();
    connect(this->_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(this->frame->leTextMessage, SIGNAL(returnPressed()), this, SLOT(on_pbSendMessage_clicked()));
    frame->pbBackMessage->setAutoDefault(false);
    this->isChat = isChat;
    setHeaderString(header);

    frame->twMessage->scrollToBottom();
    if(isChat) publicMessageHistoryReq();
    else privateMessageHistoryReq();
}

void MessageWindow::setHeaderString(QString str){
    frame->leNicknameMessage->setLineWidth(str.length()+5);
    frame->leNicknameMessage->setText(str);
}

void MessageWindow::onSokReadyRead()
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


    switch (command)
    {
        case MyClient::comPrivateHistoryResp:
        {
        frame->twMessage->clear();
        frame->twMessage->clearContents();
        frame->twMessage->clearSpans();
        frame->twMessage->clearSelection();
            int msgListSize;
            in >> msgListSize;
            frame->twMessage->setRowCount(msgListSize);
            for(int i=0; i<msgListSize; i++){
                QString msg;
                in >> msg;
                QList<QString> str = msg.split("}|{");

                QTableWidgetItem *header = new QTableWidgetItem;
                header->setText(str.at(0));
                header->setBackgroundColor(QColor(160, 160, 160));
                frame->twMessage->setVerticalHeaderItem(i, header);

                for(int j=1; j<3; j++){
                    QTableWidgetItem *item = new QTableWidgetItem();
                    if(j == 2) item->setText(str.at(j).right(8));
                    else item->setText(str.at(j));
                    frame->twMessage->setItem(i, j-1, item);
                }
                frame->twMessage->scrollToBottom();
            }
            this->show();
            mainWindow->hide();
        }
        break;
    case MyClient::comChatHistoryResp:
    {
        frame->twMessage->clear();
        frame->twMessage->clearContents();
        frame->twMessage->clearSpans();
        frame->twMessage->clearSelection();
        int msgListSize;
        in >> msgListSize;
        frame->twMessage->setRowCount(msgListSize);
        for(int i=0; i<msgListSize; i++){
            QString msg;
            in >> msg;
            QList<QString> str = msg.split("}|{");

            QTableWidgetItem *header = new QTableWidgetItem;
            header->setText(str.at(0));
            header->setBackgroundColor(QColor(160, 160, 160));
            frame->twMessage->setVerticalHeaderItem(i, header);

            for(int j=1; j<3; j++){
                QTableWidgetItem *item = new QTableWidgetItem();
                if(j == 2) item->setText(str.at(j).right(8));
                else item->setText(str.at(j));
                frame->twMessage->setItem(i, j-1, item);
            }
        }
        frame->twMessage->scrollToBottom();
        this->show();
        mainWindow->hide();
    }
    break;

    case MyClient::comNewMessageIn:
    {
        QString message;
        in >> message;

        QList<QString> list = message.split("}|{");

        bool isChatMessageFromServer;
        isChatMessageFromServer = list.at(0).toInt();

        QString chatName = list.at(1);
        QString fromName = list.at(2);

        if(!isChatMessageFromServer){
            if(chatName != frame->leNicknameMessage->text() && chatName == _name
                    && fromName != frame->leNicknameMessage->text()){
                break;
                return;
            }
        } else{
            if(chatName != frame->leNicknameMessage->text().toLower()){
                break;
                return;
            }
        }

        frame->twMessage->setRowCount(frame->twMessage->rowCount()+1);

        QTableWidgetItem *header = new QTableWidgetItem;
        header->setText(list.at(2));
        header->setBackgroundColor(QColor(160, 160, 160));
        frame->twMessage->setVerticalHeaderItem(frame->twMessage->rowCount()-1, header);

        for(int j = 0; j < 2; j++){
            QTableWidgetItem *item = new QTableWidgetItem();
            if(j == 1) item->setText(list.at(4).right(8));
            else item->setText(list.at(3));
            frame->twMessage->setItem(frame->twMessage->rowCount()-1, j, item);
        }
        frame->twMessage->scrollToBottom();
    }
    break;
    case MyClient::comNewUserToChatReqSuccess:
    {
        frame->widgetNewChatUserMessage->hide();
    }
    break;
    case MyClient::comPrivateServerMessage:
    {
        QString serverMessage;
        in >> serverMessage;
        QMessageBox::information(this, "Server message", serverMessage);
    }
    break;
    }
}

void MessageWindow::privateMessageHistoryReq(){

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comPrivateHistoryReq;
    QString name = _name;
    QString name2 = frame->leNicknameMessage->text();
    out << _name;
    out << frame->leNicknameMessage->text();

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MessageWindow::publicMessageHistoryReq(){

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comChatHistoryReq;
    out << _name;
    out << frame->leNicknameMessage->text().toLower();

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MessageWindow::setMessageHistory(QList<QString>){
    //TODO
}

void MessageWindow::on_pbBackMessage_clicked()
{
    disconnect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    mainWindow->connectSocket();
    mainWindow->msgBoxRequest();
    mainWindow->show();
    this->hide();
}

void MessageWindow::on_pbSendMessage_clicked()
{
    
    quint8 command = MyClient::comNewMessageSent;
    QString message = frame->leTextMessage->text();
    QString toUsername;
    if(!isChat) toUsername = frame->leNicknameMessage->text();
    else toUsername = frame->leNicknameMessage->text().toLower();

    sendMessage(command, toUsername, message);

    frame->leTextMessage->setText("");
}

void MessageWindow::sendMessage(quint8 comm, QString toName, QString value){
    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)comm;
    out << isChat;
    out << toName;
    out << value;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MessageWindow::on_pbAddNewChatUser_clicked()
{
   frame->widgetNewChatUserMessage->show();
}

void MessageWindow::on_pbAddNewUserToChatMessage_clicked()
{
    QString userName = frame->leNewUserToChatMessage->text();

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comNewUserToChatReq;
    out << frame->leNicknameMessage->text().toLower();
    out << frame->leNewUserToChatMessage->text();

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MessageWindow::on_pbBackNewChatUserMessage_clicked()
{
    frame->widgetNewChatUserMessage->hide();
}
