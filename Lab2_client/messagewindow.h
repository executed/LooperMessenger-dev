#ifndef MESSAGEWINDOW_H
#define MESSAGEWINDOW_H

#include <QDialog>
#include <QTcpSocket>
#include "myclient.h"
#include "authentication.h"
#include "mainwindow.h"
class Authentication;
class MyClient;
class MainWindow;

namespace Ui {
class MessageWindow;
}

class MessageWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MessageWindow(QWidget *parent = 0);
    ~MessageWindow();
    void setHeaderString(QString str);
    void setMessageHistory(QList<QString>);
    void configurationSetup(QString header, bool isChat);
    void setSok(QTcpSocket *_sok);
    void setName(QString name);
    void setMainWindowInstance(MainWindow *mainWindow);
    void setAuthenticationInstance(Authentication *auth);
    void sendMessage(quint8 comm, QString toUsername, QString value);

private slots:
    void onSokReadyRead();

    void on_pbBackMessage_clicked();

    void on_pbSendMessage_clicked();

    void on_pbAddNewChatUser_clicked();

    void on_pbAddNewUserToChatMessage_clicked();

    void on_pbBackNewChatUserMessage_clicked();

private:
    void privateMessageHistoryReq();
    void publicMessageHistoryReq();

    Ui::MessageWindow *frame;
    QTcpSocket *_sok;
    Authentication *auth;
    MainWindow *mainWindow;
    quint16 _blockSize;
    QString _name;
    bool isChat;
};

#endif // MESSAGEWINDOW_H
