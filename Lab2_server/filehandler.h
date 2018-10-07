#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QtCore>
#include <QDebug>
#include <QDir>
#include "string_constants.h"

class FileHandler
{
public:
    //constructors
    FileHandler();
    FileHandler(QString fileName);
    //methods
    void setCurrentFile(QFile file);
    QString getPropertyByKey(QString key);
    bool loadConfigurationFile();
    void writeLine(QString line);
    bool isEmpty();
    bool deleteFile();
    void close();
private:
    //methods
    bool closeConnection();
    bool createFile(QString filePath);
    bool deleteFile(QString filePath);
    //variables
    QFile *_currentFile;
    //constants
    const QString DEFAULT_CONFIG_FILE_NAME = ":/configurations/config.txt";
};

#endif // FILEHANDLER_H
