#ifndef INPUTDIALOG_H
#define INPUTDIALOG_H

#include <QWidget>
#include <QDateEdit>
#include <QCheckBox>

namespace Ui {
class InputDialog;
}

#define DATE_NUM 6
typedef enum
{
   SEL_SHOW,
   SEL_NO_SHOW,
   SEL_LEAVE,
   SEL_DC,

   SEL_NUM,
}SELCTION_E;

typedef enum
{
   TITLE_DATE,
   TITLE_SEL,
   TITLE_NOTE,

   TITLE_NUM
}TITLE_E;

class QtAttendanceMain;
class InputDialog : public QWidget
{
    Q_OBJECT

public:
    explicit InputDialog(QWidget *parent = nullptr);
    ~InputDialog();
    QString getId(void);
    void setId(QString id);
    QString getStrFromSelectedIndex (int selectedIndex);
    int getIntFromSelectedIndexStr (QString selectedIndexStr);
    QtAttendanceMain* mainPtr;

private slots:
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
    Ui::InputDialog *ui;
    QDateEdit* dateList[DATE_NUM];
    QCheckBox* checkBoxList[DATE_NUM];
};

#endif // INPUTDIALOG_H
