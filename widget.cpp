#include "widget.h"
#include <QDebug>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMap>
#include <QShortcut>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 输入框
    QLabel *nameLabel = new QLabel(tr("Name:"));
    nameLine = new QLineEdit;
    nameLine->setReadOnly(true);
    QLabel *groupLabel = new QLabel(tr("Group:"));
    groupLine = new QLineEdit;
    groupLine->setReadOnly(true);
    QLabel *phoneLabel = new QLabel(tr("Phone Number:"));
    phoneLine = new QLineEdit;
    phoneLine->setReadOnly(true);
    QLabel *commentLabel = new QLabel(tr("Comment:"));
    commentText = new QTextEdit;
    commentText->setReadOnly(true);

    // 按钮组1
    addButton = new QPushButton(tr("&Add"));
    addButton->show();
    editButton = new QPushButton(tr("&Edit"));
    editButton->show();
    deleteButton = new QPushButton(tr("&Delete"));
    deleteButton->show();
    searchButton = new QPushButton(tr("&Search"));
    searchButton->show();
    loadButton = new QPushButton(tr("&Load"));
    loadButton->show();
    saveButton = new QPushButton(tr("&Save"));
    saveButton->show();

    // 按钮提示
    loadButton->setToolTip(tr("Load contacts from a file"));
    saveButton->setToolTip(tr("Save contacts to a file"));

    // 按钮组2
    finishButton = new QPushButton(tr("&Finish"));
    finishButton->hide();
    cancelButton = new QPushButton(tr("&Cancel"));
    cancelButton->hide();

    // 按钮组3
    previousButton = new QPushButton(tr("&Previous"));
    previousButton->show();
    nextButton = new QPushButton(tr("&Next"));
    nextButton->show();

    // 搜索框
    dialog = new FindDialog(this);

    // 信号槽
    connect(addButton, &QPushButton::clicked, this, &Widget::addContent);
    connect(editButton, &QPushButton::clicked, this, &Widget::editContent);
    connect(deleteButton, &QPushButton::clicked, this, &Widget::deleteContent);
    connect(searchButton, &QPushButton::clicked, this, &Widget::searchContent);
    connect(loadButton, &QPushButton::clicked, this, &Widget::loadFromFile);
    connect(saveButton, &QPushButton::clicked, this, &Widget::saveToFile);

    connect(finishButton, &QPushButton::clicked, this, &Widget::finishContent);
    connect(cancelButton, &QPushButton::clicked, this, &Widget::cancelContent);
    connect(previousButton, &QPushButton::clicked, this, &Widget::previous);
    connect(nextButton, &QPushButton::clicked, this, &Widget::next);

    // 侧边按钮组1布局
    QVBoxLayout *buttonLayout1 = new QVBoxLayout;
    buttonLayout1->addWidget(addButton);
    buttonLayout1->addWidget(editButton);
    buttonLayout1->addWidget(deleteButton);
    buttonLayout1->addWidget(searchButton);
    buttonLayout1->addWidget(loadButton);
    buttonLayout1->addWidget(saveButton);
    buttonLayout1->addStretch();

    // 低栏按钮组3布局
    QHBoxLayout *buttonLayout3 = new QHBoxLayout;
    buttonLayout3->addWidget(previousButton);
    buttonLayout3->addWidget(nextButton);

    // 主布局
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLine, 0, 1);
    mainLayout->addWidget(finishButton, 0, 2);

    mainLayout->addWidget(groupLabel, 1, 0);
    mainLayout->addWidget(groupLine, 1, 1);
    mainLayout->addWidget(cancelButton, 1, 2);

    mainLayout->addWidget(phoneLabel, 2, 0);
    mainLayout->addWidget(phoneLine, 2, 1);

    mainLayout->addWidget(commentLabel, 3, 0, Qt::AlignTop);
    mainLayout->addWidget(commentText, 3, 1);
    mainLayout->addLayout(buttonLayout1, 3, 2);
    mainLayout->addLayout(buttonLayout3, 4, 1);

    setLayout(mainLayout);
    setWindowTitle(tr("Simple Contacts Book"));

    // 按钮快捷键
    addButton->setShortcut(QKeySequence::New);
    deleteButton->setShortcut(QKeySequence::Delete);
    searchButton->setShortcut(QKeySequence::Find);
    loadButton->setShortcut(QKeySequence::Open);
    saveButton->setShortcut(QKeySequence::Save);
    cancelButton->setShortcut(QKeySequence::Cancel);
    previousButton->setShortcut(QKeySequence::MoveToPreviousPage);
    nextButton->setShortcut(QKeySequence::MoveToNextPage);

    updateInterface(NavigationMode);
}

Widget::~Widget()
{
}

void Widget::updateInterface(Mode mode)
{
    currentMode = mode;

    switch(currentMode)
    {

    case AddingMode:
    case EditingMode:

        nameLine->setReadOnly(false);
        nameLine->setFocus(Qt::OtherFocusReason);
        groupLine->setReadOnly(false);
        phoneLine->setReadOnly(false);
        commentText->setReadOnly(false);

        addButton->setEnabled(false);
        editButton->setEnabled(false);
        deleteButton->setEnabled(false);

        nextButton->setEnabled(false);
        previousButton->setEnabled(false);

        finishButton->show();
        cancelButton->show();
        break;

    case NavigationMode:
        if(contacts.isEmpty())
        {
            nameLine->clear();
            groupLine->clear();
            phoneLine->clear();
            commentText->clear();
        }

        nameLine->setReadOnly(true);
        groupLine->setReadOnly(true);
        phoneLine->setReadOnly(true);
        commentText->setReadOnly(true);

        addButton->setEnabled(true);

        int number = contacts.size();
        editButton->setEnabled(number >= 1);
        deleteButton->setEnabled(number >= 1);
        nextButton->setEnabled(number > 1);
        previousButton->setEnabled(number > 1);
        searchButton->setEnabled(number >= 1);

        finishButton->hide();
        cancelButton->hide();
        break;
    }
}

void Widget::addContent()
{
    oldName = nameLine->text();
    oldData.group = groupLine->text();
    oldData.num = phoneLine->text();
    oldData.comment = commentText->toPlainText();

    nameLine->clear();
    groupLine->clear();
    phoneLine->clear();
    commentText->clear();

    updateInterface(AddingMode);
    return;
}

void Widget::editContent()
{
    oldName = nameLine->text();
    oldData.group = groupLine->text();
    oldData.num = phoneLine->text();
    oldData.comment = commentText->toPlainText();

    updateInterface(EditingMode);
    return;
}

void Widget::deleteContent()
{
    QString name = nameLine->text();

    if (contacts.contains(name)) {

        int button = QMessageBox::question(this,
            tr("Confirm Remove"),
            tr("Are you sure you want to remove \"%1\"?").arg(name),
            QMessageBox::Yes | QMessageBox::No);

        if (button == QMessageBox::Yes) {

            previous();
            contacts.remove(name);

            QMessageBox::information(this, tr("Remove Successful"),
                tr("\"%1\" has been removed from your address book.").arg(name));
        }
    }

    updateInterface(NavigationMode);
    return;
}

void Widget::searchContent()
{
    dialog->show();

    if (dialog->exec() == QDialog::Accepted)
    {
        QString contactName = dialog->getFindNameText();
        QString contactGroup = dialog->getFindGroupText();

        if(!contactName.isEmpty())
        {
            if (contacts.contains(contactName))
            {
                nameLine->setText(contactName);
                groupLine->setText(contacts.value(contactName).group);
                phoneLine->setText(contacts.value(contactName).num);
                commentText->setText(contacts.value(contactName).comment);
            }
            else
            {
                QMessageBox::information(this, tr("Contact Not Found"),
                    tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
                return;
            }
        }
        else if(!contactGroup.isEmpty())
        {
            QMap<QString, Contact>::iterator i = contacts.begin();
            while(i.value().group != contactGroup)
            {
                if(i == contacts.end())
                    break;

                i++;
            }

            if (i.value().group == contactGroup)
            {
                nameLine->setText(i.key());
                groupLine->setText(i.value().group);
                phoneLine->setText(i.value().num);
                commentText->setText(i.value().comment);
            }
            else
            {
                QMessageBox::information(this, tr("Contact Not Found"),
                    tr("Sorry, \"%1\" is not in your address book.").arg(contactName));
                return;
            }
        }
    }

    updateInterface(NavigationMode);
}

void Widget::loadFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Address Book"), "",
                                                    tr("Address Book (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    else
    {

        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly | QFile::Text))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        // 读入数据
        QTextStream in(&file);
        QString dataAll;
        QStringList wordList;
        contacts.clear();						// 清除已有数据

        dataAll = in.readAll();
        wordList = dataAll.split(",");
        file.close();

        qint64 i = 0;
        QString name;
        Contact data;
        while(i < wordList.size())
        {
            name = wordList.value(i++);
            data.group = wordList.value(i++);
            data.num = wordList.value(i++);
            data.comment = wordList.value(i++);

            contacts.insert(name, data);
        }

        qDebug() << wordList;

        if (contacts.isEmpty())
        {
            QMessageBox::information(this, tr("No contacts in file"),
                                     tr("The file you are attempting to open contains no contacts."));
        }
        else
        {
            QMap<QString, contact>::iterator i = contacts.begin();
            i++;
            nameLine->setText(i.key());
            groupLine->setText(i.value().group);
            phoneLine->setText(i.value().num);
            commentText->setText(i.value().comment);
        }
    }

    updateInterface(NavigationMode);
}

void Widget::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Address Book"), "",
                                                    tr("Address Book (*.csv);;All Files (*)"));
    if (fileName.isEmpty())
    {
        return;
    }
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QTextStream out(&file);

        // 写入数据
        QMap<QString, Contact>::iterator i = contacts.begin();
        do
        {
            out << i.key() << ","
                << i.value().group << ","
                << i.value().num << ","
                << i.value().comment << ",";
            i++;
        }
        while(i != contacts.end());
    }
}

void Widget::finishContent()
{
    QString name = nameLine->text();
    Contact data;

    data.group = groupLine->text();
    data.num = phoneLine->text();
    data.comment = commentText->toPlainText();

    if(currentMode == AddingMode)
    {
        if(!contacts.contains(name))
        {
            contacts.insert(name, data);
            QMessageBox::information(this, tr("Empty Field"),
                                     tr("\"%1\" has been added to your address book.").arg(name));
        }
        else
        {
            QMessageBox::information(this, tr("Add Unsuccessful."),
                                     tr("Sorry, \"%1\" is already in your address book.").arg(name));
            return;
        }
    }
    else if(currentMode == EditingMode)
    {
        if(oldName != name)
        {
            if(!contacts.contains(name))
            {
                QMessageBox::information(this, tr("Edit Successful"),
                                         tr("\"%1\" has been edited in your address book.").arg(oldName));
                contacts.remove(oldName);
                contacts.insert(name, data);
            }
            else
            {
                QMessageBox::information(this, tr("Edit Unsuccessful"),
                                         tr("Sorry, \"%1\" is already in your address book.").arg(name));
            }
        }
        else if(oldData != data)
        {
            QMessageBox::information(this, tr("Edit Successful"),
                tr("\"%1\" has been edited in your address book.").arg(name));
            contacts[name] = data;
        }
    }

    updateInterface(NavigationMode);
}

void Widget::cancelContent()
{
    nameLine->setText(oldName);
    groupLine->setText(oldData.group);
    phoneLine->setText(oldData.num);
    commentText->setText(oldData.comment);

    updateInterface(NavigationMode);
    return;
}

void Widget::previous()
{
    QString name = nameLine->text();
    QMap<QString, Contact>::iterator i = contacts.find(name);

    if (i == contacts.end()){
        nameLine->clear();
        groupLine->clear();
        phoneLine->clear();
        commentText->clear();
        return;
    }

    if (i == contacts.begin())
        i = contacts.end();

    i--;
    nameLine->setText(i.key());
    groupLine->setText(i.value().group);
    phoneLine->setText(i.value().num);
    commentText->setText(i.value().comment);
}

void Widget::next()
{
    QString name = nameLine->text();
    QMap<QString, Contact>::iterator i = contacts.find(name);

    if (i != contacts.end())
        i++;

    if (i == contacts.end())
        i = contacts.begin();

    nameLine->setText(i.key());
    groupLine->setText(i.value().group);
    phoneLine->setText(i.value().num);
    commentText->setText(i.value().comment);
}

