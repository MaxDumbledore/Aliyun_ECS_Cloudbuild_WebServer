#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test.h"
#include "askdialog.h"
#include <QJsonDocument>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800,1000);

    test=new Test(this);

    connect(ui->createUserButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::CREATE_USER.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["username"]=user.name;
            j["password"]=user.password;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;
        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();

        conn=connect(test,&Test::ok,this,[=](){
            auto name=cj["username"].toString();
            ui->userComboBox->addItem(name);
            nameToUser[name]={name,cj["password"].toString(),QString(),QString(),false};
        });

        test->createUser(cj);
    });

    connect(ui->loginButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::LOGIN.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["username"]=user.name;
            j["password"]=user.password;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        conn=connect(test,&Test::ok,this,[=](const QString &s){
            auto &user=nameToUser[cj["username"].toString()];
            user.online=true;
            if(!user.token.isNull())
                tokenToName.remove(user.token);
            user.token=s;
            tokenToName[user.token]=user.name;
        });

        test->loginUser(cj);
    });

    connect(ui->userInfoButton,&QPushButton::clicked,this,[this]{
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::GET_USER_BY_NAME.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
            j["username"]=ui->userComboBox->currentText();

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(ok)
            test->getUserByName(QJsonDocument::fromJson(content.toUtf8()).object());
    });

    connect(ui->createRoomButton,&QPushButton::clicked,this,[this]{
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::CREATE_ROOM.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["token"]=user.token;
        }
        if(ui->roomComboBox->currentIndex()!=-1)
        {
            const auto &room=idToRoom[ui->roomComboBox->currentText()];
            j["name"]=room.name;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        conn=connect(test,&Test::ok,this,[=](const QString &s){
            ui->roomComboBox->addItem(s);
            idToRoom[s]={s,cj["name"].toString()};
        });

        test->createRoom(cj);
    });

    connect(ui->enterRoomButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::ENTER_ROOM.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["token"]=user.token;
        }
        if(ui->roomComboBox->currentIndex()!=-1)
            j["roomId"]=ui->roomComboBox->currentText();

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        conn=connect(test,&Test::ok,this,[=](const QString &){
            auto token=cj["token"].toString();
            if(tokenToName.count(token))
            {
                auto &user=nameToUser[tokenToName[cj["token"].toString()]];
                user.currentRoom=cj["roomId"].toInt();
            }
        });

        test->enterRoom(cj);
    });

    connect(ui->leaveRoomButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::LEAVE_ROOM.toUtf8()).object();
        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["token"]=user.token;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        conn=connect(test,&Test::ok,this,[=](const QString &){
            auto token=cj["token"].toString();
            if(tokenToName.count(token))
            {
                auto &user=nameToUser[tokenToName[cj["token"].toString()]];
                user.currentRoom=QString();
            }
        });

        test->leaveRoom(cj);
    });

    connect(ui->roomNameButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::ROOM_INFO.toUtf8()).object();
        if(ui->roomComboBox->currentIndex()!=-1)
            j["roomId"]=ui->roomComboBox->currentText();

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        test->getRoom(cj);
    });

    connect(ui->roomUserButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::ROOM_INFO.toUtf8()).object();
        if(ui->roomComboBox->currentIndex()!=-1)
            j["roomId"]=ui->roomComboBox->currentText();

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        test->getUserListFromRoom(cj);
    });

    connect(ui->roomListButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::ROOM_LIST.toUtf8()).object();

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        test->listRooms(cj);
    });

    connect(ui->sendButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::SEND_MESSAGE.toUtf8()).object();

        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["token"]=user.token;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        test->sendMessage(cj);
    });

    connect(ui->retrieveButton,&QPushButton::clicked,this,[this](){
        disconnect(conn);
        QJsonObject j=QJsonDocument::fromJson(AskTemplate::RETRIEVE_MESSAGE.toUtf8()).object();

        if(ui->userComboBox->currentIndex()!=-1)
        {
            const auto &user=nameToUser[ui->userComboBox->currentText()];
            j["token"]=user.token;
        }

        bool ok;
        auto content=AskDialog::getContent(this,QJsonDocument(j).toJson(),&ok);
        if(!ok)
            return ;

        QJsonObject cj=QJsonDocument::fromJson(content.toUtf8()).object();
        test->retrieveMessage(cj);
    });


    connect(test,&Test::responseReceived,ui->responseTextEdit,&QTextEdit::setText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

