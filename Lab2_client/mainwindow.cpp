#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QDialog(parent),
    mainWnd(new Ui::MainWindow)
{
    mainWnd->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete mainWnd;
}

void MainWindow::setAuthenticationInstance(Authentication *instance){
    this->auth = instance;
}

void MainWindow::configurationSetup(){
    connect(this->_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(this->_sok, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(this->_sok, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(this->_sok, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
    msgBoxRequest();

    mainWnd->widgetNewChatMain->hide();
    mainWnd->widgetSearch->hide();
    mainWnd->lbErrorSearch->hide();
    this->show();
}

void MainWindow::setSok(QTcpSocket *_sok){
    this->_sok = _sok;
}

void MainWindow::setName(QString name){
    this->_name = name;
}

void MainWindow::msgBoxRequest(){
    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comMsgTitlesReq;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MainWindow::onSokConnected(){
}

void MainWindow::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        QMessageBox::information(this, "Error", "Remote host closed");
        break;
    case QAbstractSocket::HostNotFoundError:
    {
       QMessageBox::information(this, "Error", "Host not found");
    }
        break;
    case QAbstractSocket::ConnectionRefusedError:
    {
        QMessageBox::information(this, "Error", "The connection was refused by the peer");
        break;
    default:
    {
        QString errorMsg = "Error: The following error occurred: " + _sok->errorString();
        QMessageBox::information(this, "Error", errorMsg);
    }
    }
}
}

void MainWindow::onSokDisconnected()
{
    //empty
}

void MainWindow::onSokReadyRead()
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
        case MyClient::comMsgTitlesResp:
        {
            QString str;
            in >> str;
            QStringList list = str.split(",");
            mainWnd->lwMain->clear();
            mainWnd->lwMain->addItems(list);
            configurateItems(list.size());
        }
        break;

        case MyClient::comNickExist:
        {
            bool exists;
            in >> exists;

            if(!exists){
                mainWnd->lbNicknameSearch->hide();
                mainWnd->pbMessageSearch->hide();
                mainWnd->pbAddMain->hide();
                mainWnd->lbErrorSearch->setText("Sorry, user not found");
                mainWnd->widgetSearch->show();
                mainWnd->lbErrorSearch->show();
                break;
            }
            mainWnd->lbErrorSearch->hide();
            mainWnd->lbNicknameSearch->show();
            mainWnd->pbMessageSearch->show();
            mainWnd->pbAddMain->show();
            QString nickname;
            in >> nickname;

            mainWnd->lbNicknameSearch->setText(nickname);
            mainWnd->widgetSearch->show();
        }
        break;
        case MyClient::comNewContactSuccess:
        {
            mainWnd->pbBackSearch->setEnabled(true);
            QMessageBox::information(this, "Success", "User added to your contact list");
        }
        break;
        case MyClient::comNewContactFail:
        {
            mainWnd->pbBackSearch->setEnabled(true);
            QMessageBox::information(this, "Error", "User wasn't added to your contact list");
        }
        break;

        case MyClient::comNewChatReqSuccess:
        {
        mainWnd->widgetSearch->hide();
        msgBoxRequest();
        }
        break;

        case MyClient::comNewMessageIn:
        {
        QString message;
        in >> message;
        }
        break;

        case MyClient::comNewGroupChatReqSuccess:
        {
        mainWnd->widgetNewChatMain->hide();
        msgBoxRequest();
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

void MainWindow::msgTitlesSetting(QStringList list){
    for(int i=0; i<list.size();i++){
        QListWidgetItem *item;
        item->setText(list.at(i));
        item->setBackgroundColor(QColor::fromRgb(87,89,93));
        mainWnd->lwMain->addItem(item);
    }
}

void MainWindow::newChatRequest(QString toNickname){
    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comNewChatReq;
    out << toNickname;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MainWindow::newContactReq(QString nickname){
    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comNewContactReq;
    out << nickname;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MainWindow::connectSocket(){
    connect(this->_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
}

void MainWindow::configurateItems(int size){
       mainWnd->lwMain->ScrollPerPixel;
    for(int i = 0; i < size; i++){

        mainWnd->lwMain->item(i)->setTextColor(Qt::white);
        QColor grey(154, 156, 158);
        mainWnd->lwMain->item(i)->setBackgroundColor(grey);
    }
}

void MainWindow::on_lwMain_itemClicked(QListWidgetItem *item)
{
    msgWnd = new MessageWindow();
    QString itemText = item->text();
    QString kind = itemText.left(itemText.indexOf(":"));
    disconnect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));

    if(kind == "Private"){
        QString name = itemText.right(itemText.size()-itemText.indexOf(":")-2);

        msgWnd->setSok(_sok);
        msgWnd->setName(_name);
        //this->hide();
        msgWnd->configurationSetup(name, false);
        msgWnd->setMainWindowInstance(this);
    }
    if(kind == "Group"){
        QString name = itemText.right(itemText.size()-itemText.indexOf(":")-2);

        msgWnd->setSok(_sok);
        msgWnd->setName(_name);
        //this->hide();
        msgWnd->configurationSetup(name.toUpper(), true);
        msgWnd->setMainWindowInstance(this);
    }

}

void MainWindow::on_pbSearchMain_clicked()
{
    QString userSearched = mainWnd->leSearchMain->text();

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comNicknameExistanceCheck;
    out << userSearched;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MainWindow::on_pbBackSearch_clicked()
{
    mainWnd->widgetSearch->hide();
}

void MainWindow::on_pbMessageSearch_clicked()
{
    newChatRequest(mainWnd->lbNicknameSearch->text());
}

void MainWindow::on_pbAddMain_clicked()
{
    mainWnd->pbBackSearch->setEnabled(false);
    newContactReq(mainWnd->lbNicknameSearch->text());
}

void MainWindow::on_pbAddNewChatMain_clicked()
{
    mainWnd->widgetNewChatMain->show();
}

void MainWindow::on_pbNewChatMain_clicked()
{
    QString chatName = mainWnd->leNewChatMain->text();

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comNewGroupChatReq;
    out << chatName;
    out << _name;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void MainWindow::on_pbBackNewChatMain_clicked()
{
    mainWnd->widgetNewChatMain->hide();
}

void MainWindow::on_pbRefreshMain_clicked()
{
    msgBoxRequest();
}
