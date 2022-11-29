#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

class FindDialog : public QDialog
{
    Q_OBJECT

public:
    FindDialog(QWidget *parent = nullptr);
    QString getFindNameText();
    QString getFindGroupText();

public slots:
    void findNameClicked();
    void findGroupClicked();

private:
    QPushButton *findNameButton;
    QPushButton *findGroupButton;
    QLineEdit *lineEdit;
    QString findNameText;
    QString findGroupText;
};

#endif // FINDDIALOG_H
