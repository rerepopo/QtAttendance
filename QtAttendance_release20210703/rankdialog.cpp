#include "rankdialog.h"
#include "ui_rankdialog.h"
#include <QDir>
#include "qtattendancemain.h"

RankDialog::RankDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RankDialog)
{
    ui->setupUi(this);

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//表格變為禁止編輯
    //ui->tableWidget->clearContents();//移除:: 用途:換ID顯示前，先清掉內容
    ui->tableWidget->setRowCount(0);//資料深度
    ui->tableWidget->setSortingEnabled(true);
}

RankDialog::~RankDialog()
{
    delete ui;
}

void RankDialog::runRank()
{
    QDir dir;
    QStringList filters;

    filters << "*.csv" ; //<< "*.cxx" << "*.cc"
    dir.setNameFilters(filters);

    QStringList list = dir.entryList();

    this->setWindowTitle("出席排名 "+ QString::number(list.size())+"位");
    //clean all
    ui->tableWidget->clearContents();//移除:: 用途:換ID顯示前，先清掉內容
    ui->tableWidget->setRowCount(0);//資料深度
    ui->tableWidget->sortItems(0);//不知道甚麼bug  一定要先排過第0欄 才能輸入@@@@@@@@@@@@
    for (int x=0;x<list.size() ; x++) //x index: list for files name
    {
        QString fileName = list[x];
        QFile file(fileName);
        file.open(QIODevice::ReadOnly | QIODevice::Truncate  | QIODevice::Text );// //https://www.itread01.com/content/1550390409.html

        QString data =  file.readAll();        //qDebug () << "data in file:" << data ;

        QStringList dataList = data.split('\n');

        double rate = 0; //出席率，DC之後重新開始算= (出席次數/出席次數+請假次數+擴課次數)
        double totalDays = 0;
        double showDays = 0;

        for (int row=0; row<dataList.size(); row++)  //row index: list for row data list in file
        {
            QStringList rowList = dataList[row].split(',');
            if (rowList.size()==TITLE_NUM)
            {
                SELCTION_E sel= (SELCTION_E)  (rowList[TITLE_SEL].left(1).toInt()-1);
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
            else
            {
                //qDebug () << "[warning]row: " << row << "    data in file:" << rowList;
            }
        }

        QString rateStr;
        if (totalDays!=0)
        {
            rate = showDays/totalDays*100;
            rateStr = QString::number(rate, 'f', 1);
            rateStr = rateStr.rightJustified(5, ' ', true);// 不補空格的話，排序會錯
        }
        else
        {
            rateStr = "DC";
        }


        ui->tableWidget->insertRow( x );
        ui->tableWidget->setItem(x,0,new QTableWidgetItem(fileName.replace(".csv","")));
        ui->tableWidget->setItem(x,1,new QTableWidgetItem(rateStr));
        ui->tableWidget->setItem(x,2,new QTableWidgetItem(QString::number(showDays, 'f', 0)));
        ui->tableWidget->setItem(x,3,new QTableWidgetItem(QString::number(totalDays, 'f', 0)));

        ui->tableWidget->item(x,0)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->tableWidget->item(x,1)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->tableWidget->item(x,2)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        ui->tableWidget->item(x,3)->setTextAlignment(Qt::AlignRight|Qt::AlignVCenter);
        file.close();
    }

    ui->tableWidget->sortItems(1);//預設小到大
}

void RankDialog::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //qDebug() << "ID "   << ui->tableWidget->item(row,0)->text();
    mainPtr->loadFromCsv(ui->tableWidget->item(row,0)->text());
}
