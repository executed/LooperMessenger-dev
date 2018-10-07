#include "myentity.h"

MyEntity::MyEntity()
{
}

MyEntity::MyEntity(QString nickname, QString password)
{
    MyEntity::setNickname(nickname);
    MyEntity::setPassword(password);
    MyEntity::setEmail(email);
}

MyEntity::MyEntity(QString nickname, QString password, QString email, QString phoneNumber)
{
    MyEntity::setNickname(nickname);
    MyEntity::setPassword(password);
    MyEntity::setEmail(email);
    MyEntity::setPhoneNumber(phoneNumber);
}

QString MyEntity::getId(){
    return id;
}

QString MyEntity::getNickname() const{
    return nickname;
}

QString MyEntity::getPassword(){
    return password;
}

QString MyEntity::getEmail(){
    return email;
}

QString MyEntity::getLastOnline(){
    return lastOnline;
}

QString MyEntity::getPhoneNumber(){
    return phoneNumber;
}

void MyEntity::setId(QString id){
    this->id = id;
}

void MyEntity::setNickname(QString nickname){
    this->nickname = nickname;
}

void MyEntity::setPassword(QString password){
    this->password = password;
}

void MyEntity::setEmail(QString email){
    this->email = email;
}

void MyEntity::setLastOnline(QString lastOnline){
    this->lastOnline = lastOnline;
}

void MyEntity::setPhoneNumber(QString phoneNumber){
    this->phoneNumber = phoneNumber;
}
