#include "qtattendancemain.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtAttendanceMain mainWinsow;
    mainWinsow.show();
    return a.exec();
}
//[要放下]Win xp 不支援
