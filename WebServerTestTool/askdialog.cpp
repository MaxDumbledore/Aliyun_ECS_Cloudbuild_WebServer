#include "askdialog.h"
#include "ui_askdialog.h"

AskDialog::AskDialog(AskType askType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AskDialog)
{
    ui->setupUi(this);
    setFixedSize(400,500);
    QString s;
    switch (askType) {
    case USER_INFO:
        s="{\n"
          "\"username\": \"string\",\n"
          "\"firstName\": \"string\",\n"
          "\"lastName\": \"string\",\n"
          "\"email\": \"string\",\n"
          "\"password\": \"string\",\n"
          "\"phone\": \"string\"\n"
          "}";
    default:
        break;
    }
    ui->textEdit->setText(s);
}

AskDialog::~AskDialog()
{
    delete ui;
}

QString AskDialog::getContent()
{
    return ui->textEdit->toPlainText();
}
