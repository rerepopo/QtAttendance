#ifndef QTATTENDANCEMAIN_H
#define QTATTENDANCEMAIN_H

#include <QWidget>
#include "inputdialog.h"
#include "editdialog.h"
#include "rankdialog.h"
QT_BEGIN_NAMESPACE
namespace Ui { class QtAttendanceMain; }
QT_END_NAMESPACE



class QtAttendanceMain : public QWidget
{
    Q_OBJECT

public:
    QtAttendanceMain(QWidget *parent = nullptr);
    ~QtAttendanceMain();
   void setOneDay(QDate inputDate, int selectedIndex);
   void setOneRow(QDate inputDate, int selectedIndex, QString note, bool needHighLight);
   void deleteOneRow(int rowIndex);
   void updateRate(void);
   void saveToCsv(void);
   void loadFromCsv(QString id);
   void closeEvent(QCloseEvent *event);
private slots:
    void on_pushButton_input_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_rank_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::QtAttendanceMain *ui;
    InputDialog inputDialog;
    EditDialog  editDialog;
    RankDialog  rankDialog;
};
#endif // QTATTENDANCEMAIN_H
