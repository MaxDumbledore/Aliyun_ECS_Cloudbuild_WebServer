#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QJsonObject>

class QNetworkAccessManager;
class QNetworkReply;

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);
public slots:
    void createRoom(const QJsonObject &j);

    void enterRoom(const QJsonObject &j);

    void leaveRoom(const QJsonObject &j);

    void getRoom(const QJsonObject &j);

    void getUserListFromRoom(const QJsonObject &j);

    void listRooms(const QJsonObject &j);

    void sendMessage(const QJsonObject &j);

    void retrieveMessage(const QJsonObject &j);

    void createUser(const QJsonObject &j);

    void loginUser(const QJsonObject &j);

    void getUserByName(const QJsonObject &j);
signals:
    void ok(const QString &);

    void responseReceived(const QString &);
private:
    QNetworkAccessManager *naManager;
};

#endif // TEST_H
