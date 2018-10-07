#ifndef MYCLIENT_H
#define MYCLIENT_H
#include <QtCore>

class MyClient
{

public:
    static const QString constNameUnknown;
    static const quint8 comLoginReq = 0;
    static const quint8 comSignupReq = 1;
    static const quint8 comUsersOnline = 2;
    static const quint8 comUserJoin = 3;
    static const quint8 comUserLeft = 4;
    //static const quint8 comMessageToAll = 5;
    static const quint8 comMessageToUsers = 6;
    static const quint8 comPublicServerMessage = 7;
    static const quint8 comPrivateServerMessage = 8;
    static const quint8 comAutchSuccess = 9;
    static const quint8 comErrNameInvalid = 201;
    static const quint8 comErrNameUsed = 202;
    static const quint8 comMsgTitlesReq = 203;
    static const quint8 comMsgTitlesResp = 204;
    static const quint8 comAutchFail = 205;
    static const quint8 comNewMessagesReq = 206;
    static const quint8 comPrivateHistoryReq = 207;
    static const quint8 comChatHistoryReq = 208;
    static const quint8 comPrivateHistoryResp = 209;
    static const quint8 comChatHistoryResp = 210;
    static const quint8 comNewMessageSent = 211;
    static const quint8 comNewMessageIn = 212;
    static const quint8 comNicknameExistanceCheck = 213;
    static const quint8 comNickExist = 214;
    static const quint8 comNewChatReq = 215;
    static const quint8 comNewChatReqSuccess = 216;
    static const quint8 comNewContactReq = 217;
    static const quint8 comNewContactSuccess = 218;
    static const quint8 comNewContactFail = 219;
    static const quint8 comNewGroupChatReq = 220;
    static const quint8 comNewGroupChatReqSuccess = 221;
    static const quint8 comNewUserToChatReq = 222;
    static const quint8 comNewUserToChatReqSuccess = 223;
};

#endif // MYCLIENT_H
