#include <QApplication>
#include "dialog.h"
#include "string_constants.h"
#include "filehandler.h"

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);

int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
 
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    return a.exec();
}

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    STRING_CONSTANTS CONSTANT;
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, CONSTANT.DEBUG_REGEX, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, CONSTANT.WARNING_REGEX, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, CONSTANT.CRITICAL_REGEX, localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, CONSTANT.FATAL_REGEX, localMsg.constData(), context.file, context.line, context.function);
        abort();
    }
}
