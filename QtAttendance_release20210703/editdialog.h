#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QWidget>

namespace Ui {
class EditDialog;
}
class QtAttendanceMain;
class EditDialog : public QWidget
{
    Q_OBJECT

public:
    explicit EditDialog(QWidget *parent = nullptr);
    ~EditDialog();
    QtAttendanceMain* mainPtr;
    QString getId(void);
    void setId(QString id);
    void setIndex(int index);
    void setDate(QDate date);
    void setSelection(int sel);
    void setNote(QString note);

private slots:
    void on_pushButton_delete_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    QStringList sel_str=
    {
       "1. 出席",
       "2. 曠課",
       "3. 請假",
       "4. DC"
    };
    Ui::EditDialog *ui;
};

#endif // EDITDIALOG_H
