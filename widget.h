#ifndef WIDGET_H
#define WIDGET_H

#include "finddialog.h"
#include <QWidget>
#include <QMap>

class QLineEdit;
class QTextEdit;
class QPushButton;

enum Mode
{
    NavigationMode,
    AddingMode,
    EditingMode
};

typedef struct contact
{
    QString group;
    QString num;
    QString comment;

    bool operator==(const contact &other) const
    {
        return group == other.group && num == other.num && comment == other.comment;
    }

    bool operator!=(const contact &other) const
    {
        return group != other.group || num != other.num || comment != other.comment;
    }

} Contact;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void updateInterface(Mode mode);

    // 通讯录数据
    QMap<QString, Contact> contacts;
    QString oldName;
    Contact oldData;

    // 输入框
    QLineEdit *nameLine;
    QLineEdit *groupLine;
    QLineEdit *phoneLine;
    QTextEdit *commentText;

    // 按钮组1
    QPushButton *addButton;
    QPushButton *editButton;
    QPushButton *deleteButton;
    QPushButton *searchButton;
    QPushButton *loadButton;
    QPushButton *saveButton;

    // 按钮组2
    QPushButton *finishButton;
    QPushButton *cancelButton;

    // 按钮组3
    QPushButton *previousButton;
    QPushButton *nextButton;

    // 状态机
    Mode currentMode;

    // 搜索框
    FindDialog *dialog;

private slots:
    void addContent();
    void editContent();
    void deleteContent();
    void searchContent();
    void loadFromFile();
    void saveToFile();

    void finishContent();
    void cancelContent();

    void previous();
    void next();
};
#endif // WIDGET_H
