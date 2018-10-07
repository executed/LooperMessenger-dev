#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <QDialog>
#include <QTcpSocket>
#include <QMessageBox>
#include <QDebug>
#include "myclient.h"
#include "mainwindow.h"

class MyClient;
class MainWindow;

namespace Ui {
class Authentication;
}

class Authentication : public QDialog
{
    Q_OBJECT

public:
    explicit Authentication(QWidget *parent = 0);
    ~Authentication();

    QString hostAddress = "46.151.253.84";
    quint16 hostPort = 8189;

private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);

    //gui slots
    void on_pbLoginMain_clicked();

    void on_pbSignupMain_clicked();

    void on_pbBackMain_clicked();

    void on_pbLoginLogin_clicked();


    void on_pbSignupSignup_clicked();

    void on_pushButton_clicked();

    void on_pbHostAddressMain_clicked();

private:
    void sendLoginData(QString nickname, QString password);
    void sendSignupData(QString nickname, QString password, QString email, QString phoneNumber);
    Ui::Authentication *ui;
    MainWindow *mainWindow;

    QTcpSocket *_sok;
    bool isSignUp;
    quint16 _blockSize;
    QString _name;
    ////////////////
};

#endif // AUTHENTICATION_H
