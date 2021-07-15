#include "test.h"
#include <QNetworkAccessManager>
#include <QHttpPart>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QUrlQuery>

const QString prefixUrl="http://39.105.8.146:8080/";
//const QString prefixUrl="http://localhost:8080/";

Test::Test(QObject *parent) : QObject(parent)
{
    naManager=new QNetworkAccessManager(this);

    connect(naManager,&QNetworkAccessManager::finished,this,[this](QNetworkReply *reply){
        auto s=reply->readAll();
        emit responseReceived(reply->errorString()+"\n\n"+s);
        if(reply->error()==QNetworkReply::NoError)
            emit ok(s);
        reply->deleteLater();
    });
}

void Test::createRoom(const QJsonObject &j)
{
    QUrl url(prefixUrl+"room");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",("Bearer "+j["token"].toString()).toUtf8());

    auto body=j;
    body.remove("token");
    naManager->post(request,QJsonDocument(body).toJson());
}

void Test::enterRoom(const QJsonObject &j)
{
    QUrl url(prefixUrl+"room/"+j["roomId"].toString()+"/enter");
    QNetworkRequest request(url);

    request.setRawHeader("Authorization",("Bearer "+j["token"].toString()).toUtf8());
    naManager->put(request,"");
}

void Test::leaveRoom(const QJsonObject &j)
{
    QUrl url(prefixUrl+"roomLeave");
    QNetworkRequest request(url);

    request.setRawHeader("Authorization",("Bearer "+j["token"].toString()).toUtf8());
    naManager->put(request,"");
}

void Test::getRoom(const QJsonObject &j)
{
    QUrl url(prefixUrl+"room/"+j["roomId"].toString());
    naManager->get(QNetworkRequest(url));
}

void Test::getUserListFromRoom(const QJsonObject &j)
{
    QUrl url(prefixUrl+"room/"+j["roomId"].toString()+"/users");
    naManager->get(QNetworkRequest(url));
}

void Test::listRooms(const QJsonObject &j)
{
    QUrl url(prefixUrl+"roomList");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");

    naManager->post(request,QJsonDocument(j).toJson());
}

void Test::sendMessage(const QJsonObject &j)
{
    QUrl url(prefixUrl+"message/send");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Authorization",("Bearer "+j["token"].toString()).toUtf8());

    naManager->post(request,QJsonDocument(j).toJson());
}

void Test::retrieveMessage(const QJsonObject &j)
{
    QUrl url(prefixUrl+"message/retrieve");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    request.setRawHeader("Authorization",("Bearer "+j["token"].toString()).toUtf8());

    naManager->post(request,QJsonDocument(j).toJson());
}

void Test::createUser(const QJsonObject &j)
{
    QUrl url(prefixUrl+"user");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    naManager->post(request,QJsonDocument(j).toJson());
}

void Test::loginUser(const QJsonObject &j)
{
    QUrl url(prefixUrl+"userLogin");
    QUrlQuery query;
    for(auto &i:j.keys())
        query.addQueryItem(i,j[i].toString());
    url.setQuery(query);
    naManager->get(QNetworkRequest(url));
}

void Test::getUserByName(const QJsonObject &j)
{
    QUrl url(prefixUrl+"user/"+j["username"].toString());
    naManager->get(QNetworkRequest(url));
}
