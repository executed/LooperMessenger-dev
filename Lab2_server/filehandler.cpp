#include "filehandler.h"
#include <QMessageBox>

FileHandler::FileHandler(){}

FileHandler::FileHandler(QString fileName){

    if(fileName.isNull() || fileName.isEmpty()){
        qDebug() << "Invalid argument: fileName is empty or null";
        return;
    }
    QFile newFile(fileName);
    if(!newFile.exists()) qDebug() << "File wasn't created";
    else this->_currentFile = &newFile;
    //if(newFile.open(QIODevice::WriteOnly)){
        //QTextStream stream(&newFile);
        //stream<<"hello world\ni";
        //newFile.close();
        //qDebug() << "ProjectPath: " + DEFAULT_CONFIG_FILE_NAME;
        //qDebug() << "Info was written to the file";
    //} else qDebug() << "File wasn't loaded";
}

void FileHandler::setCurrentFile(QFile file){ this->_currentFile = &file; }

QString FileHandler::getPropertyByKey(QString key){

    //if(!_currentFile->exists()) exit(1);
    if(this->_currentFile == NULL) qDebug() << "You must set file first. Use method setCurrentFile(QFile).";
    else if(key.isNull() || key.isEmpty()) qDebug() << "Invalid argument: key value is empty or null";
    else{
        QFile file(DEFAULT_CONFIG_FILE_NAME);
        if (file.open(QIODevice::ReadOnly)){
            while(!file.atEnd()){
                QString line = file.readLine();
                QString currentLine = line.left(line.indexOf('\n'));
                QStringList keyAndValue = currentLine.split("=");
                if(keyAndValue.contains(key)){
                    file.close();
                    return keyAndValue.at(1).toLocal8Bit().constData();
                }
            }
            qDebug() << "Such property does't exist: " << key << endl;
            file.close();
        }
    }
}

bool FileHandler::loadConfigurationFile(){
    QFile file(DEFAULT_CONFIG_FILE_NAME);
    file.open( QIODevice::WriteOnly | QIODevice::Append );
    //if(file.pos() == 0){ //is empty
     //   qDebug() << "Configuration file is empty" << endl;
       // file.close();
        return false;
    //}
    //setCurrentFile(file);
    this->_currentFile = &file;
    file.close();
    return true;
}






