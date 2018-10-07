#include "authentication.h"
#include "ui_authentication.h"

Authentication::Authentication(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authentication)
{
    _sok = new QTcpSocket(this);
    connect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(_sok, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(_sok, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(_sok, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

    ui->setupUi(this);
    isSignUp = false;
    ui->leErrorMain->hide();
    ui->frameLogin->hide();
    ui->frameSignup->hide();
    ui->pbBackMain->hide();
    ui->widgetSettingsMain->hide();
}

Authentication::~Authentication()
{
    delete ui;
}

void Authentication::sendLoginData(QString nickname, QString password){

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comLoginReq;
    //data insertion
    out << nickname;
    out << password;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);

}

void Authentication::sendSignupData(QString nickname, QString password, QString email, QString phoneNumber){

    _blockSize = 0;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0;
    out << (quint8)MyClient::comSignupReq;
    //data insertion
    out << nickname;
    out << password;
    out << email;
    out << phoneNumber;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    _sok->write(block);
}

void Authentication::onSokDisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        ui->leErrorMain->setText("Remote host closed error");
        break;
    case QAbstractSocket::HostNotFoundError:
    {
       ui->leErrorMain->setText("The host wasn't found");
    }
        break;
    case QAbstractSocket::ConnectionRefusedError:
    {
        QMessageBox::information(this, "Error", "The connection was refused by the peer");
        ui->leErrorMain->setText("Error The connection was refused by the peer.");
        break;
    default:
    {
        QString errorMsg = "Error: The following error occurred: " + _sok->errorString();
        ui->leErrorMain->setText(errorMsg);
    }
    }
}
}

void Authentication::onSokReadyRead()
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
        case MyClient::comAutchSuccess:
        {
        disconnect(_sok, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
        disconnect(_sok, SIGNAL(connected()), this, SLOT(onSokConnected()));
        disconnect(_sok, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
        disconnect(_sok, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));

            if(isSignUp){
                _name = ui->leNicknameSignup->text();
            }
            else{
                _name = ui->leNicknameLogin->text();
            }

        mainWindow = new MainWindow;
        mainWindow->setSok(_sok);
        mainWindow->setName(_name);

        mainWindow->configurationSetup();
        this->hide();
        }
        break;

        case MyClient::comAutchFail:
        {        
            QMessageBox::information(this, "Error", "Authentication fail");
            _sok->disconnectFromHost();
        }
        break;

        case MyClient::comErrNameInvalid:
        {
        QMessageBox::information(this, "Error", "This name is invalid");
            ui->leErrorMain->setText("Error: This name is invalid.");
            _sok->disconnectFromHost();
        }
        break;

        case MyClient::comErrNameUsed:
        {
        QMessageBox::information(this, "Error", "This name is already used");
            ui->leErrorMain->setText("Error: This name is already used.");
            _sok->disconnectFromHost();
        }
        break;
    }
}

void Authentication::onSokConnected(){

    if(isSignUp){

        QString nickname = ui->leNicknameSignup->text();
        QString password = ui->lePasswordSignup->text();
        QString email = ui->leEmailSignup->text();
        QString phoneNumber = ui->lePhoneSignup->text();
        sendSignupData(nickname, password, email, phoneNumber);
    }else{

        QString nickname = ui->leNicknameLogin->text();
        QString password = ui->lePasswordLogin->text();
        sendLoginData(nickname, password);
    }
}

void Authentication::onSokDisconnected()
{
    //do nothing
}


//GUI
void Authentication::on_pbLoginMain_clicked()
{
    ui->pbBackMain->show();
    ui->leErrorMain->hide();
    ui->leAuthenticationStatus->setText("     Login");
    ui->splitter->hide();
    ui->frameLogin->show();
}

void Authentication::on_pbSignupMain_clicked()
{
    ui->pbBackMain->show();
    ui->leAuthenticationStatus->setText("   Sign Up");
    ui->splitter->hide();
    ui->frameSignup->show();
}

void Authentication::on_pbBackMain_clicked()
{
    ui->frameLogin->hide();
    ui->frameSignup->hide();
    ui->leAuthenticationStatus->setText("Authentication");
    ui->splitter->show();
}

void Authentication::on_pbLoginLogin_clicked()
{
    isSignUp = false;
    _sok->connectToHost(hostAddress, hostPort);
}


void Authentication::on_pbSignupSignup_clicked()
{
    isSignUp = true;
    _sok->connectToHost(hostAddress, hostPort);
}

void Authentication::on_pushButton_clicked()
{
    ui->widgetSettingsMain->show();
}

void Authentication::on_pbHostAddressMain_clicked()
{
    QString ip = ui->leIPMain->text();
    QString port = ui->lePortMain->text();
    hostAddress = ip;
    hostPort = port.toInt();
    ui->widgetSettingsMain->hide();
}
