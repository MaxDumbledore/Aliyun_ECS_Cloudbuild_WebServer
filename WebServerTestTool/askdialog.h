#ifndef ASKDIALOG_H
#define ASKDIALOG_H

#include <QDialog>

namespace Ui {
class AskDialog;
}

class AskDialog : public QDialog
{
    Q_OBJECT

public:
    enum AskType{
        USER_INFO
    };

    explicit AskDialog(AskType askType, QWidget *parent = nullptr);
    ~AskDialog();

    QString getContent();

private:
    Ui::AskDialog *ui;
};

#endif // ASKDIALOG_H
