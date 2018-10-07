#include <QApplication>
#include "authentication.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Authentication auth;

    auth.show();

    return a.exec();
}
