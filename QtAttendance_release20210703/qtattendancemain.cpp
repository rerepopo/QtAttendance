#include "qtattendancemain.h"
#include "ui_qtattendancemain.h"
#include <QMessageBox>
#include <QDir>
QtAttendanceMain::QtAttendanceMain(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtAttendanceMain)
{
    inputDialog.mainPtr= this;
    editDialog.mainPtr = this;
    rankDialog.mainPtr = this;
    ui->setupUi(this);
    ui->label_top_id->setTextInteractionFlags(Qt::TextSelectableByMouse);//Make QLabel text selectable. https://stackoverflow.com/questions/17957749/make-qlabel-text-selectable/17957750
    ui->label_top_attend->setTextInteractionFlags(Qt::TextSelectableByMouse);//Make QLabel text selectable. https://stackoverflow.com/questions/17957749/make-qlabel-text-selectable/17957750

    //表格
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格變為禁止編輯
    //ui->tableWidget->clearContents();//移除:: 用途:換ID顯示前，先清掉內容
    ui->tableWidget->setRowCount(0);//資料深度
    //ui->tableWidget->setSortingEnabled(true); 自由排序會不容易刪除項目
    //月曆翻頁
    ui->calendarWidget_1->showPreviousMonth();
    ui->calendarWidget_1->showPreviousMonth();
    ui->calendarWidget_2->showPreviousMonth();
}

QtAttendanceMain::~QtAttendanceMain()
{
    delete ui;
}

void QtAttendanceMain::setOneDay(QDate inputDate, int selectedIndex)
{
   // 月曆 上色 範例
    QTextCharFormat boldFormat;
        boldFormat.setFontWeight(QFont::Bold);

    QTextCharFormat formatShow = boldFormat;
        formatShow.setBackground(Qt::green);
    QTextCharFormat formatNoshow = boldFormat;
        formatNoshow.setBackground(Qt::red);
    QTextCharFormat formatLeave  = boldFormat;
        formatLeave.setBackground(Qt::yellow);
    QTextCharFormat formatDc  = boldFormat;
        formatDc.setBackground(Qt::gray);

        switch (selectedIndex)
        {
            case SEL_SHOW :
                ui->calendarWidget_1->setDateTextFormat(inputDate,formatShow);
                ui->calendarWidget_2->setDateTextFormat(inputDate,formatShow);
                ui->calendarWidget_3->setDateTextFormat(inputDate,formatShow);
            break;
            case SEL_NO_SHOW :
                ui->calendarWidget_1->setDateTextFormat(inputDate,formatNoshow);
                ui->calendarWidget_2->setDateTextFormat(inputDate,formatNoshow);
                ui->calendarWidget_3->setDateTextFormat(inputDate,formatNoshow);
            break;
            case SEL_LEAVE :
                ui->calendarWidget_1->setDateTextFormat(inputDate,formatLeave);
                ui->calendarWidget_2->setDateTextFormat(inputDate,formatLeave);
                ui->calendarWidget_3->setDateTextFormat(inputDate,formatLeave);
            break;
            case SEL_DC :
                ui->calendarWidget_1->setDateTextFormat(inputDate,formatDc);
                ui->calendarWidget_2->setDateTextFormat(inputDate,formatDc);
                ui->calendarWidget_3->setDateTextFormat(inputDate,formatDc);
            break;
            default:
                Q_ASSERT(0);
            break;
        }
}

void QtAttendanceMain::setOneRow(QDate inputDate, int selectedIndex, QString note, bool needHighLight)
{
    int rowCount = ui->tableWidget->rowCount();

    ui->tableWidget->sortItems(TITLE_DATE);//預設順序: 小到大

    //表格 插入 row 範例
    //qDebug() << "rowCount: "<< rowCount << "input data: "<< inputDate;
    QDate preDate;
    QDate nextDate;

    for (int index = 0; index <= rowCount; index++ )
    {
        if (index==rowCount)//hit end
        {
            ui->tableWidget->insertRow( index );
            ui->tableWidget->setItem(index,TITLE_DATE,new QTableWidgetItem(inputDate.toString("yyyy/MM/dd")));
            ui->tableWidget->setItem(index,TITLE_SEL,new QTableWidgetItem(inputDialog.getStrFromSelectedIndex(selectedIndex)));
            ui->tableWidget->setItem(index,TITLE_NOTE,new QTableWidgetItem(note));//   QString::number(rowCount)
            ui->tableWidget->scrollToBottom();
            if(needHighLight)
            {
                ui->tableWidget->item(index,TITLE_DATE)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->item(index,TITLE_SEL)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->item(index,TITLE_NOTE)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->scrollToItem( ui->tableWidget->item(index,TITLE_DATE) );
            }
            setOneDay(inputDate, selectedIndex);
            updateRate();
            saveToCsv();
            break;
        }

        if (index==0)
        {
            preDate = QDate::fromString("1111/01/01","yyyy/MM/dd");
        }
        else
        {
            preDate = QDate::fromString(ui->tableWidget->item(index-1,TITLE_DATE)->text(),"yyyy/MM/dd");
        }
        nextDate = QDate::fromString(ui->tableWidget->item(index,TITLE_DATE)->text(),"yyyy/MM/dd");

        if((preDate<inputDate)&&(inputDate<=nextDate))
        {
            ui->tableWidget->insertRow( index );
            ui->tableWidget->setItem(index,TITLE_DATE,new QTableWidgetItem(inputDate.toString("yyyy/MM/dd")));
            ui->tableWidget->setItem(index,TITLE_SEL,new QTableWidgetItem(inputDialog.getStrFromSelectedIndex(selectedIndex)));
            ui->tableWidget->setItem(index,TITLE_NOTE,new QTableWidgetItem(note));//   QString::number(rowCount)
            ui->tableWidget->scrollToBottom();
            if(needHighLight)
            {
                ui->tableWidget->item(index,TITLE_DATE)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->item(index,TITLE_SEL)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->item(index,TITLE_NOTE)->setBackground(QBrush( QColor(Qt::yellow)));
                ui->tableWidget->scrollToItem( ui->tableWidget->item(index,TITLE_DATE) );
            }
            setOneDay(inputDate, selectedIndex);
            updateRate();
            saveToCsv();
            break;
        }
    }

}

void QtAttendanceMain::deleteOneRow(int rowIndex)
{
    //clean月曆
    QDate date= QDate::fromString(ui->tableWidget->item(rowIndex,TITLE_DATE)->text(),"yyyy/MM/dd");
    QTextCharFormat format;
    ui->calendarWidget_1->setDateTextFormat(date,format); // If date is null, all date formats are cleared.
    ui->calendarWidget_2->setDateTextFormat(date,format); // If date is null, all date formats are cleared.
    ui->calendarWidget_3->setDateTextFormat(date,format); // If date is null, all date formats are cleared.

    //clean table
    ui->tableWidget->removeRow(rowIndex);
}

void QtAttendanceMain::updateRate()
{
    int rowCount = ui->tableWidget->rowCount();
    double rate = 0; //出席率，DC之後重新開始算= (出席次數/出席次數+請假次數+擴課次數)
    double totalDays = 0;
    double showDays = 0;

    for (int index = 0; index < rowCount; index++ )
    {
        QString selectedIndexStr = ui->tableWidget->item(index,TITLE_SEL)->text();
        //qDebug() << "selectedIndexStr: "<< selectedIndexStr << "getInt: "<< inputDialog.getIntFromSelectedIndexStr(selectedIndexStr);
        SELCTION_E sel= (SELCTION_E)inputDialog.getIntFromSelectedIndexStr(selectedIndexStr);
        if (sel==SEL_DC)
        {
            totalDays = 0;
            showDays = 0;
        }
        else
        {
            totalDays++;
            if(sel==SEL_SHOW)
            {
                showDays++;
            }
        }
    }


    if (totalDays!=0)
    {
        rate = showDays/totalDays*100;
        ui->label_top_attend->setText("出席率: "+QString::number(rate)+"%");
    }
    else
    {
        ui->label_top_attend->setText("出席率: DC");
    }
}

void QtAttendanceMain::saveToCsv()
{
    int rowCount = ui->tableWidget->rowCount();
    QString id = ui->label_top_id->text().replace(QString("ID: "), QString(""));
    QString fileName(id+".csv");//ref https://forum.qt.io/topic/72088/check-if-file-exists-qt/3
    QFile file(fileName);

    file.open(QIODevice::WriteOnly | QIODevice::Truncate  | QIODevice::Text );// //https://www.itread01.com/content/1550390409.html

    //file.write("Qt Programming");
    for (int row = 0; row <rowCount; row++)
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);
            out << ui->tableWidget->item(row,TITLE_DATE)->text() <<","
                << ui->tableWidget->item(row,TITLE_SEL)->text() <<","
                << ui->tableWidget->item(row,TITLE_NOTE)->text()<<"\n" ;
    }

    file.close();
}

void QtAttendanceMain::loadFromCsv(QString id)
{
    if (editDialog.getId()!=id)
    {
        editDialog.close();
    }
    if (inputDialog.getId()!=id)
    {
        inputDialog.close();
    }

    //set to label_top_id:
    ui->label_top_id->setText(QString("ID: ")+id);

    QString fileName(id+".csv");//ref https://forum.qt.io/topic/72088/check-if-file-exists-qt/3	    QFile file(fileName);

    //clean all
    ui->tableWidget->clearContents();//移除:: 用途:換ID顯示前，先清掉內容
    ui->tableWidget->setRowCount(0);//資料深度
    //clean月曆
    QDate date;
    QTextCharFormat format;
    ui->calendarWidget_1->setDateTextFormat(date,format); // If date is null, all date formats are cleared.
    ui->calendarWidget_2->setDateTextFormat(date,format); // If date is null, all date formats are cleared.
    ui->calendarWidget_3->setDateTextFormat(date,format); // If date is null, all date formats are cleared.

    //read from file
    if(QFileInfo::exists(fileName))
    {
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Truncate  | QIODevice::Text );// //https://www.itread01.com/content/1550390409.html

        QString data =  file.readAll();
        //qDebug () << "data in file:" << data ;

        QStringList dataList = data.split('\n');

        int index=0;

        for (int row=0; row<dataList.size(); row++)
        {
            QStringList rowList = dataList[row].split(',');
            if (rowList.size()==TITLE_NUM)
            {
                /*ui->tableWidget->insertRow( index );
                ui->tableWidget->setItem(index,TITLE_DATE,new QTableWidgetItem(rowList[TITLE_DATE]));
                ui->tableWidget->setItem(index,TITLE_SEL,new QTableWidgetItem(rowList[TITLE_SEL]));
                ui->tableWidget->setItem(index,TITLE_NOTE,new QTableWidgetItem(rowList[TITLE_NOTE]));*/
                setOneRow(QDate::fromString(rowList[TITLE_DATE],"yyyy/MM/dd"),
                          rowList[TITLE_SEL].left(1).toInt()-1,
                          rowList[TITLE_NOTE],
                          false);
                //qDebug () << "[info]index: " << index << QDate::fromString(rowList[TITLE_DATE],"yyyy/MM/dd") << rowList[TITLE_SEL].left(1).toInt();
                index++;
            }
            else
            {
                //qDebug () << "[warning]row: " << row << "    data in file:" << rowList;
            }
        }

        file.close();
        updateRate();
    }
    else
    {
        //QMessageBox::information(this, QString("ID: ")+id, "檔案不存在");
        ui->label_top_attend->setText("檔案不存在");
    }
}

void QtAttendanceMain::closeEvent(QCloseEvent *event)
{   //virtual function
    // do what you need here
    inputDialog.close();
    editDialog.close();
    rankDialog.close();
    // then call parent's procedure
    QWidget::closeEvent(event);
}



void QtAttendanceMain::on_pushButton_input_clicked()
{
    QString inputStr = ui->lineEdit->text().trimmed();

    QRegularExpression re("^[0-9]+$");// "^[A-Za-z0-9]+$"  ref https://dotblogs.com.tw/spark/2012/12/12/85594
    QRegularExpressionMatch match = re.match(inputStr);

    if (match.hasMatch())
    {
        loadFromCsv(inputStr);

        QPoint p = this->pos();
        QSize  s = this->frameSize();
        //inputDialog.setGeometry(p.x()+s.width(), p.y(), s.width()*1/2, s.height());//show on right
        if(!(inputDialog.isVisible())  )
        {
           inputDialog.move(p.x()+s.width(), p.y());//show on left?
        }
        inputDialog.setWindowTitle("出席輸入 ID:"+inputStr);
        inputDialog.setId(inputStr);
        inputDialog.show();
        inputDialog.activateWindow();
        inputDialog.setWindowState(Qt::WindowState::WindowActive);

    }
    else
    {
        QMessageBox::information(this, " ID 格式錯誤", "只能輸入數字 \n[^[0-9]+$]");
    }


}

void QtAttendanceMain::on_pushButton_search_clicked()
{
    QString inputStr = ui->lineEdit->text().trimmed();

    QRegularExpression re("^[0-9]+$");// "^[A-Za-z0-9]+$"  ref https://dotblogs.com.tw/spark/2012/12/12/85594
    QRegularExpressionMatch match = re.match(inputStr);

    if (match.hasMatch())
    {
        loadFromCsv(inputStr);
    }
    else
    {
        QMessageBox::information(this, " ID 格式錯誤", "只能輸入數字 \n[^[0-9]+$]");
    }

}

void QtAttendanceMain::on_pushButton_rank_clicked()
{
    QPoint p = this->pos();
    QSize  s = this->frameSize();
    //inputDialog.setGeometry(p.x()+s.width(), p.y(), s.width()*1/2, s.height());//show on right
    if(!(rankDialog.isVisible())  )
    {
       rankDialog.move(p.x()+s.width(), p.y());//show on left?
    }

    rankDialog.runRank();

    rankDialog.show();
    rankDialog.activateWindow();
    rankDialog.setWindowState(Qt::WindowState::WindowActive);
}

void QtAttendanceMain::on_tableWidget_cellDoubleClicked(int row, int column)
{
    QString inputStr = ui->label_top_id->text().replace(QString("ID: "), QString(""));

    editDialog.setWindowTitle("編輯 "+ui->label_top_id->text());
    editDialog.setId(inputStr);
    editDialog.setIndex(row+1);
    editDialog.setDate(QDate::fromString(ui->tableWidget->item(row,TITLE_DATE)->text(),"yyyy/MM/dd"));
    editDialog.setSelection(inputDialog.getIntFromSelectedIndexStr(ui->tableWidget->item(row,TITLE_SEL)->text()));
    editDialog.setNote(ui->tableWidget->item(row,TITLE_NOTE)->text());


    editDialog.show();
    editDialog.activateWindow();
    editDialog.setWindowState(Qt::WindowState::WindowActive);
}
