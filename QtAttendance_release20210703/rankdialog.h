#ifndef RANKDIALOG_H
#define RANKDIALOG_H

#include <QWidget>
#include "inputdialog.h"
namespace Ui {
class RankDialog;
}

class QtAttendanceMain;
class RankDialog : public QWidget
{
    Q_OBJECT

public:
    explicit RankDialog(QWidget *parent = nullptr);
    ~RankDialog();
    QtAttendanceMain* mainPtr;
    void runRank (void);

private slots:
    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    Ui::RankDialog *ui;
};

#endif // RANKDIALOG_H
