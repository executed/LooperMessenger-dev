#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    _serv = new MyServer(this, this);

    connect(this, SIGNAL(messageFromGui(QString,QStringList)), _serv, SLOT(onMessageFromGui(QString,QStringList)));
    connect(_serv, SIGNAL(addLogToGui(QString,QColor)), this, SLOT(onAddLogToGui(QString,QColor)));

    startOfApp = false;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onAddUserToGui(QString name)
{
    ui->lwUsers->addItem(name);
    ui->lwUsers->item(ui->lwUsers->count()-1)->setTextColor(Qt::white);
    int onlineUsersCount = ui->lcdOnlineCountMain->intValue();
    ui->lcdOnlineCountMain->display(++onlineUsersCount);
}

void Dialog::onRemoveUserFromGui(QString name)
{
    for (int i = 0; i < ui->lwUsers->count(); ++i)
        if (ui->lwUsers->item(i)->text() == name)
        {
            ui->lwUsers->takeItem(i);
            break;
        }
    int onlineUsersCount = ui->lcdOnlineCountMain->intValue();
    ui->lcdOnlineCountMain->display(--onlineUsersCount);
}

void Dialog::onMessageToGui(QString message, QString from, const QStringList &users)
{
    if (users.isEmpty())
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" message from "+from+": "+message+" to all");
    else
    {
        ui->lwLog->insertItem(0, QTime::currentTime().toString()+" message from "+from+": "+message+" to "+users.join(","));
        ui->lwLog->item(0)->setTextColor(Qt::blue);
    }
}

void Dialog::onAddLogToGui(QString string, QColor color)
{
    addToLog(string, color);
}

void Dialog::on_pbSend_clicked()
{
    if (_serv->getOnlineCount() == 0)
    {
        QMessageBox::information(this, "", NO_CLIENTS);
        return;
    }
    QStringList l;
    l.append( ui->leNicknameMain->text());
    emit messageFromGui(ui->pteMessage->document()->toPlainText(), l);
    ui->pteMessage->clear();
    addToLog(PM_SENT + l.first(), Qt::white);

}


void Dialog::on_pbStartStop_toggled(bool checked)
{
    if(startOfApp) return;

    if (checked)
    {
        addToLog(SERV_STOPPED+_serv->serverAddress().toString()+":"+QString::number(_serv->serverPort()), Qt::yellow);
        _serv->close();
        ui->pbStartStop->setText(PB_START_SERV);
    }
    else
    {
        if (!addr.setAddress(ui->leHost->text()))
        {
            addToLog(INVALID_ADDR + ui->leHost->text(), Qt::red);
            return;
        }
        if (_serv->doStartServer(addr, ui->lePort->text().toInt()))
        {
            addToLog(SERV_STARTED+ui->leHost->text()+":"+ui->lePort->text(), Qt::green);
            ui->pbStartStop->setText(PB_STOP_SERV);
        }
        else
        {
            addToLog(SERV_NOT_STARTED+ui->leHost->text()+":"+ui->lePort->text(), Qt::red);
            ui->pbStartStop->setChecked(true);
        }
    }
}

void Dialog::addToLog(QString text, QColor color)
{
    ui->lwLog->insertItem(0, QTime::currentTime().toString()+text);
    ui->lwLog->item(0)->setTextColor(color);
}
