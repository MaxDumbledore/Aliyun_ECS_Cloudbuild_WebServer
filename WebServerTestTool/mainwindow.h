#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

class Test;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    struct User
    {
        QString name,password,token,currentRoom;
        bool online;
    };

    struct Room
    {
        QString id, name;
    };

private:
    Ui::MainWindow *ui;
    Test *test;

    QMap<QString, User> nameToUser;
    QMap<QString, QString> tokenToName;
    QMap<QString, Room> idToRoom;

    QMetaObject::Connection conn;
};
#endif // MAINWINDOW_H
