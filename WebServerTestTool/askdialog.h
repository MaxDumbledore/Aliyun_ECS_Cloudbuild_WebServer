#ifndef ASKDIALOG_H
#define ASKDIALOG_H

#include <QInputDialog>

namespace AskTemplate
{
inline const QString CREATE_USER="{\n"
    "\"username\": \"string\",\n"
    "\"firstName\": \"string\",\n"
    "\"lastName\": \"string\",\n"
    "\"email\": \"string\",\n"
    "\"password\": \"string\",\n"
    "\"phone\": \"string\"\n"
    "}";
inline const QString LOGIN="{\n"
    "\"username\": \"string\",\n"
    "\"password\": \"string\"\n"
    "}";
inline const QString GET_USER_BY_NAME="{\n"
    "\"username\": \"string\"\n"
    "}";
inline const QString CREATE_ROOM="{\n"
    "\"name\": \"string\",\n"
    "\"token\": \"string\"\n"
    "}";
inline const QString ENTER_ROOM="{\n"
    "\"roomId\": \"string\",\n"
    "\"token\": \"string\"\n"
    "}";
inline const QString LEAVE_ROOM="{\n"
    "\"token\": \"string\"\n"
    "}";
inline const QString ROOM_INFO="{\n"
    "\"roomId\": \"string\"\n"
    "}";
inline const QString ROOM_LIST="{\n"
    "\"pageIndex\": 0,\n"
    "\"pageSize\": 100\n"
    "}";
inline const QString SEND_MESSAGE="{\n"
    "\"token\": \"string\",\n"
    "\"id\": \"1\",\n"
    "\"text\": \"string\"\n"
    "}";
inline const QString RETRIEVE_MESSAGE="{\n"
    "\"token\": \"string\",\n"
    "\"pageIndex\": -1,\n"
    "\"pageSize\": 100\n"
    "}";
}

class AskDialog : public QInputDialog
{
    Q_OBJECT
public:

    static QString getContent(QWidget *parent=nullptr,const QString &t=QString(),bool *ok=nullptr);
signals:
private:
    explicit AskDialog(const QString &initial, QWidget *parent = nullptr);
};

#endif // ASKDIALOG_H
