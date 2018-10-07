#include "mydao.h"

#include <QDebug>
#include <QtSql>

MyDao::MyDao(){
    loadDBCongigurations();
    connectToDB();
}

QString MyDao::getCurrentTime(){
    QSqlQuery query(db);
    QString sql = "SELECT NOW();";
    query.prepare(sql);
    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";
    if(query.first()) return query.value(0).toString();
}

MyEntity MyDao::getById(QString id){

    QSqlQuery query(db);
    QString sql = "SELECT * FROM users WHERE id = ?;";
    query.prepare(sql);
    query.addBindValue(id);

    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";

    MyEntity user;
    while(query.next()){
        user.setId(query.value(0).toString());
        user.setNickname(query.value(1).toString());
        user.setPassword(query.value(2).toString());
        user.setEmail(query.value(3).toString());
    }
    return user;
}

MyEntity MyDao::getByNickname(QString nickname){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM users WHERE nickname = ?;";
    query.prepare(sql);
    query.addBindValue(nickname);

    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";

    MyEntity searchedUser;
    while(query.next()){
        searchedUser.setId(query.value(0).toString());
        searchedUser.setNickname(query.value(1).toString());
        searchedUser.setPassword(query.value(2).toString());
        searchedUser.setEmail(query.value(3).toString());

    }
    return searchedUser;
}

MyEntity MyDao::getByEmail(QString email){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM users WHERE email = ?;";
    query.prepare(sql);
    query.addBindValue(email);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    MyEntity user;
    while(query.next()){
        user.setId(query.value(0).toString());
        user.setNickname(query.value(1).toString());
        user.setPassword(query.value(2).toString());
        user.setEmail(query.value(3).toString());
    }
    return user;
}

MyEntity MyDao::getByPhoneNumber(QString phoneNumber){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM users WHERE phone_number = ?;";
    query.prepare(sql);
    query.addBindValue(phoneNumber);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    MyEntity user;
    while(query.next()){
        user.setId(query.value(0).toString());
        user.setNickname(query.value(1).toString());
        user.setPassword(query.value(2).toString());
        user.setEmail(query.value(3).toString());
    }
    return user;
}

//getLastOnline

void MyDao::deleteById(QString id){
    QSqlQuery query(db);
    QString sql = "DELETE FROM users where id = ?;";
    query.prepare(sql);
    query.addBindValue(id);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::deleteByNickname(QString nickname){
    QSqlQuery query(db);
    QString sql = "DELETE FROM users where nickname = ?;";
    query.prepare(sql);
    query.addBindValue(nickname);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::updateLastOnline(QString nickname){
    QSqlQuery query(db);
    QString sql = "UPDATE users SET last_online = NOW() WHERE nickname = ?;";
    query.prepare(sql);
    query.addBindValue(nickname);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::setEntity(MyEntity user){
    this->user = user;
}

void MyDao::setPrivate(QString id, QString privateId){
    QSqlQuery query(db);
    QString sql = "INSERT INTO privates (id, user_2_id) VALUES (?, ?);";
    query.prepare(sql);
    query.addBindValue(getByNickname(id).getId());
    query.addBindValue(getByNickname(privateId).getId());
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

bool MyDao::entityExecute(MyEntity user){
    this->user = user;

    QSqlQuery query(db);
    QString sql = "INSERT INTO users (nickname, password, email, phone_number) VALUES (?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(user.getNickname());
    query.addBindValue(user.getPassword());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getPhoneNumber());

    if(!query.exec()){
        qDebug() << db.lastError().text() << "'\n'";
        return false;
    }
    return true;
}

void MyDao::registerDataExecute(MyEntity user){
    this->user = user;

    QSqlQuery query(db);
    QString sql = "INSERT INTO users (nickname, password, email, phone_number) VALUES (?, ?, ?, ?)";
    query.prepare(sql);
    query.addBindValue(user.getNickname());
    query.addBindValue(user.getPassword());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getPhoneNumber());

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::updateEntity(MyEntity user){
    this->user = user;

    QSqlQuery query(db);
    QString sql = "UPDATE user SET nickname = ?, password = ?, email = ? WHERE nickname = ?;";
    query.prepare(sql);
    query.addBindValue(user.getNickname());
    query.addBindValue(user.getPassword());
    query.addBindValue(user.getEmail());
    query.addBindValue(user.getNickname());

    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::addContact(QString id, QString contactId){
    QSqlQuery query(db);
    QString sql = "INSERT INTO contacts (id, user_renamed, contact_id) VALUES (?,'',?)";
    query.prepare(sql);
    query.addBindValue(getByNickname(id).getId());
    query.addBindValue(getByNickname(contactId).getId());

    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";
}

QList<QString> MyDao::getAllPrivatesIdById(QString id){
    QList<QString> privates;

    QSqlQuery query(db);
    QString sql = "SELECT (user_2_id) FROM privates WHERE id = ? ORDER BY last_msg;";
    query.prepare(sql);
    query.addBindValue(id);
    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";

    while(query.next()){
        privates.append(query.value(0).toString());
    }
    return privates;
}

QList<QString> MyDao::getAllPublicsNamesById(QString id){
    QList<QString> publics;

    QSqlQuery query(db);
    QString sql = "SELECT (id) FROM public_users WHERE user_id = ?;";
    query.prepare(sql);
    query.addBindValue(id);
    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";

    while(query.next()){
        QString publicId = query.value(0).toString();
        QSqlQuery query2(db);
        QString sql = "SELECT (public_name) FROM publics WHERE id = ? ORDER BY last_msg;";
        query2.prepare(sql);
        query2.addBindValue(publicId);
        if(!query2.exec()) qDebug() << db.lastError().text() << "'\n'";
        if(query2.first()) publics.append(query2.value(0).toString());
    }

    return publics;
}

//messages
Messages MyDao::getMessageById(QString id){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM messages WHERE id = ?;";
    query.prepare(sql);
    query.addBindValue(id);

    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    while(query.next()){
        message.setId(query.value(0).toString());
        message.setFromId(query.value(1).toString());
        message.setToId(query.value(2).toString());
        message.setToChatId(query.value(3).toString());
        message.setContent(query.value(4).toString());
        message.setCreatedAt(query.value(5).toString());
    }
    return message;
}

QList<Messages> MyDao::getNewMessagesToUserId(QString id){
    QSqlQuery query(db);
    QString sql = "SELECT last_online FROM users WHERE id = ?;";
    query.prepare(sql);
    query.addBindValue(id);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
    QString lastOnline;
    if(query.first()) lastOnline = query.value(0).toString();

    QSqlQuery query2(db);
    QString sql2 = "SELECT * FROM messages WHERE created_at > ? && to_id = ? ORDER BY created_at;";
    query2.prepare(sql2);
    query2.addBindValue(lastOnline);
    query2.addBindValue(id);
    QList<Messages> list;
    while(query2.next()){
        Messages message;
        message.setId(query2.value(0).toString());
        message.setFromId(query2.value(1).toString());
        message.setToId(query2.value(2).toString());
        message.setToChatId(query2.value(3).toString());
        message.setContent(query2.value(4).toString());
        message.setCreatedAt(query2.value(5).toString());
        list.append(message);
    }
    return list;
}

QList<Messages> MyDao::getAllMessagesFromPrivate(QString user1Id, QString user2Id){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM messages WHERE (from_id = ? || from_id = ?) && (to_id = ? || to_id = ?);";
    query.prepare(sql);
    query.addBindValue(user1Id);
    query.addBindValue(user2Id);
    query.addBindValue(user1Id);
    query.addBindValue(user2Id);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    QList<Messages> list;
    while(query.next()){
        Messages message;
        message.setId(query.value(0).toString());
        message.setFromId(query.value(1).toString());
        message.setToId(query.value(2).toString());
        message.setToChatId(query.value(3).toString());
        message.setContent(query.value(4).toString());
        message.setCreatedAt(query.value(5).toString());
        list.append(message);
    }
    return list;
}

QList<Messages> MyDao::getAllMessagesFromPublic(QString publicId){
    QSqlQuery query(db);
    QString sql = "SELECT * FROM messages WHERE to_chat_id = ?;";
    query.prepare(sql);
    query.addBindValue(publicId);
    if(!query.exec()) qDebug() << db.lastError().text() << "'\n'";

    QList<Messages> list;
    while(query.next()){
        Messages message;
        message.setId(query.value(0).toString());
        message.setFromId(query.value(1).toString());
        message.setToId(query.value(2).toString());
        message.setToChatId(query.value(3).toString());
        message.setContent(query.value(4).toString());
        message.setCreatedAt(query.value(5).toString());
        list.append(message);
    }
    return list;
}

void MyDao::setMessage(Messages message, bool isChat){
    QSqlQuery query(db);
    QString sql;
    if(isChat) sql = "INSERT INTO messages (from_id, to_chat_id, content, created_at) VALUES (?, ?, ?, NOW())";
    else sql = "INSERT INTO messages (from_id, to_id, content, created_at) VALUES (?, ?, ?, NOW())";
    query.prepare(sql);

    query.addBindValue(message.getFromId());
    if(isChat) query.addBindValue(message.getToChatId());
    else query.addBindValue(message.getToId());
    query.addBindValue(message.getContent());
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

QString MyDao::getChatIdByItsName(QString chatName){
    QSqlQuery query(db);
    QString sql = "SELECT (id) FROM publics WHERE public_name = ?;";
    query.prepare(sql);
    query.addBindValue(chatName);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    if(query.first()) return query.value(0).toString();
}

QList<MyEntity> MyDao::getAllPublicUsers(QString publicId){
    QSqlQuery query(db);
    QString sql = "SELECT user_id FROM public_users WHERE id = ?;";
    query.prepare(sql);
    query.addBindValue(publicId);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
    
    QList<MyEntity> entities;
    while (query.next()) {
        entities.append(getById(query.value(0).toString()));
    }
    return entities;
}

void MyDao::setNewChat(QString chatName){
    QSqlQuery query(db);
    QString sql = "INSERT INTO publics (public_name) VALUES (?);";
    query.prepare(sql);
    query.addBindValue(chatName);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::setNewPublicUser(QString chatId, QString userId){
    QSqlQuery query(db);
    QString sql = "INSERT INTO public_users (id, user_id) VALUES (?, ?)";
    query.prepare(sql);
    query.addBindValue(chatId);
    query.addBindValue(userId);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

int MyDao::getMaxOnlineUsersCount(){
    QSqlQuery query(db);
    QString sql = "SELECT maxUsersOnline FROM server_data;";
    query.prepare(sql);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";

    if(query.first()) return query.value(0).toString().toInt();
}

void MyDao::setMaxOnlineUsersCount(int value){
    QSqlQuery query(db);
    QString sql = "update server_data SET maxUsersOnline = ?, date = NOW();";
    query.prepare(sql);
    query.addBindValue(value);
    if(!query.exec())  qDebug() << db.lastError().text() << "'\n'";
}

void MyDao::connectToDB(){
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(dbUsername);
    db.setPassword(dbPassword);

    if(!db.open()){
        qDebug() << "Database " << dbName << " wasn't opened." << "'\n'";
         qDebug() << db.lastError().text() << "'\n'";
        //error output
    } else qDebug() << "Database " << dbDriverName << ": '" << dbName << "' was connected." << "'\n'";
}

void MyDao::loadDBCongigurations(){
    FileHandler fileHandler;
    fileHandler.loadConfigurationFile();
    db = QSqlDatabase::addDatabase(fileHandler.getPropertyByKey("dbDriverName"));
    hostName = fileHandler.getPropertyByKey("hostName");
    port = fileHandler.getPropertyByKey("port").toInt();
    dbName = fileHandler.getPropertyByKey("dbName");
    dbUsername = fileHandler.getPropertyByKey("dbUsername");
    dbPassword = fileHandler.getPropertyByKey("dbPassword");
}


