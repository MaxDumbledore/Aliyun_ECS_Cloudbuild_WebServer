#include "askdialog.h"

AskDialog::AskDialog(const QString&initial, QWidget *parent) : QInputDialog(parent)
{
    setOptions(QInputDialog::UsePlainTextEditForTextInput);
    setFixedSize(400,500);
    setWindowTitle("Dialog");
    setTextValue(initial);
}

QString AskDialog::getContent(QWidget *parent,const QString &t, bool *ok)
{
    AskDialog dialog(t,parent);
    const int ret=dialog.exec();
    if (ok)
        *ok = !!ret;
    if (ret)
        return dialog.textValue();
    return QString();
}
