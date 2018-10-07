#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QList>
#include <QListWidgetItem>
#include <QTcpSocket>
#include "authentication.h"
#include "myclient.h"
#include "messagewindow.h"
#include <QList>
class Authentication;
class MyClient;
class MessageWindow;

namespace Ui {
class MainWindow;
}

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setSok(QTcpSocket *_sok);
    void connectSocket();
    void setName(QString name);
    void msgBoxRequest();
    void msgTitlesSetting(QStringList list);
    void configurationSetup();
    void setAuthenticationInstance(Authentication *instance);
    void newChatRequest(QString toNickname);
    void newContactReq(QString nickname);
    void configurateItems(int itemsSize);

private slots:
    void onSokConnected();
    void onSokDisconnected();
    void onSokReadyRead();
    void onSokDisplayError(QAbstractSocket::SocketError socketError);

    void on_lwMain_itemClicked(QListWidgetItem *item);

    void on_pbSearchMain_clicked();

    void on_pbBackSearch_clicked();

    void on_pbMessageSearch_clicked();

    void on_pbAddMain_clicked();

    void on_pbAddNewChatMain_clicked();

    void on_pbNewChatMain_clicked();

    void on_pbBackNewChatMain_clicked();

    void on_pbRefreshMain_clicked();

private:
    Ui::MainWindow *mainWnd;
    MessageWindow *msgWnd;
    Authentication *auth;
    QTcpSocket *_sok;
    QList<QListWidgetItem> *msgList;
    quint16 _blockSize;
    QString _name;
};

#endif // MAINWINDOW_H
