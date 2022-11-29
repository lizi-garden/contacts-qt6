#include "finddialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMessageBox>

FindDialog::FindDialog(QWidget *parent)
    : QDialog(parent)
{
    QLabel *findLabel = new QLabel(tr("Enter the name of a contact:"));
    lineEdit = new QLineEdit;

    findNameButton = new QPushButton(tr("&Find Name"));
    findGroupButton = new QPushButton(tr("&Find Group"));
    findNameText = "";
    findGroupText = "";

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(findLabel);
    layout->addWidget(lineEdit);
    layout->addWidget(findNameButton);
    layout->addWidget(findGroupButton);

    setLayout(layout);
    setWindowTitle(tr("Find a Contact"));
    connect(findNameButton, &QPushButton::clicked, this, &FindDialog::findNameClicked);
    connect(findNameButton, &QPushButton::clicked, this, &FindDialog::accept);
    connect(findGroupButton, &QPushButton::clicked, this, &FindDialog::findGroupClicked);
    connect(findGroupButton, &QPushButton::clicked, this, &FindDialog::accept);
}

QString FindDialog::getFindNameText()
{
    return findNameText;
}

QString FindDialog::getFindGroupText()
{
    return findGroupText;
}

void FindDialog::findNameClicked()
{
    QString text = lineEdit->text();

    if (text.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name."));
        return;
    }
    else
    {
        findNameText = text;
        findGroupText = "";
        lineEdit->clear();
        hide();
    }
}

void FindDialog::findGroupClicked()
{
    QString text = lineEdit->text();

    if (text.isEmpty())
    {
        QMessageBox::information(this, tr("Empty Field"),
            tr("Please enter a name."));
        return;
    }
    else
    {
        findGroupText = text;
        findNameText = "";
        lineEdit->clear();
        hide();
    }
}
