#ifndef  MESSAGES_H
#define  MESSAGES_H

#include <QDebug>

class Messages
{

public:
    //constructors
    Messages(){

    }
    Messages(QString id, QString fromId, QString toId, QString content, QString createdAt){
        this->id = id;
        this->fromId = fromId;
        this->toId = toId;
        this->content = content;
        this->createdAt = createdAt;
    }

    //getters
    QString getId(){
        return id;
    }
    QString getFromId(){
        return fromId;
    }
    QString getToId(){
        return toId;
    }
    QString getToChatId(){
        return toChatId;
    }
    QString getContent(){
        return content;
    }
    QString getCreatedAt(){
        return createdAt;
    }

    //setters
    void setId(QString id){
        this->id = id;
    }
    void setFromId(QString id){
        this->fromId = id;
    }
    void setToId(QString id){
        this->toId = id;
    }
    void setToChatId(QString id){
        this->toChatId = id;
    }
    void setContent(QString content){
        this->content = content;
    }
    void setCreatedAt(QString createdAt){
        this->createdAt = createdAt;
    }

private:
    Messages *message;
    QString id;
    QString fromId;
    QString toId;
    QString toChatId;
    QString content;
    QString createdAt;
};

#endif // MESSAGES_H
