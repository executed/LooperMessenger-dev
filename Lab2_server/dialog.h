#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtGui>
#include <QtCore>
#include "myserver.h"
#include "string_constants.h"

namespace Ui {
    class Dialog;
}

class Dialog : public QDialog, STRING_CONSTANTS
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    MyServer *_serv;
    QHostAddress addr;
    bool startOfApp = true;
    void addToLog(QString text, QColor color);

signals:
    void messageFromGui(QString message, const QStringList &users);

public slots:
    void onAddUserToGui(QString name);
    void onRemoveUserFromGui(QString name);
    void onMessageToGui(QString message, QString from, const QStringList &users);
    void onAddLogToGui(QString string, QColor color);

private slots:

    void on_pbSend_clicked();
    void on_pbStartStop_toggled(bool checked);    
};

#endif // DIALOG_H
