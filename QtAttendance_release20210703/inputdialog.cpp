#include "inputdialog.h"
#include "ui_inputdialog.h"
#include <QDateTime>
#include "qtattendancemain.h"
#include <QMessageBox>



InputDialog::InputDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);
    ui->label_id_1->setTextInteractionFlags(Qt::TextSelectableByMouse);//Make QLabel text selectable. https://stackoverflow.com/questions/17957749/make-qlabel-text-selectable/17957750
    ui->comboBox_selection->addItems(sel_str);

    //Show today at first
    QDateTime datetime = QDateTime::currentDateTime();
    QDate date = datetime.date();
    ui->dateEdit_01->setDate(date);

    //prepare for loop
    dateList[0]=ui->dateEdit_01;
    dateList[1]=ui->dateEdit_02;
    dateList[2]=ui->dateEdit_03;
    dateList[3]=ui->dateEdit_04;
    dateList[4]=ui->dateEdit_05;
    dateList[5]=ui->dateEdit_06;
    checkBoxList[0]=ui->checkBox_01;
    checkBoxList[1]=ui->checkBox_02;
    checkBoxList[2]=ui->checkBox_03;
    checkBoxList[3]=ui->checkBox_04;
    checkBoxList[4]=ui->checkBox_05;
    checkBoxList[5]=ui->checkBox_06;

    //connect
    for (uint item=0;item<DATE_NUM;item++)
    {
       connect(checkBoxList[item], SIGNAL(clicked(bool)), dateList[item], SLOT(setEnabled(bool)));
    }
}

InputDialog::~InputDialog()
{
    delete ui;
}

QString InputDialog::getId()
{
    return ui->label_id_1->text();
}

void InputDialog::setId(QString id)
{
    ui->label_id_1->setText(id);
}

QString InputDialog::getStrFromSelectedIndex(int selectedIndex)
{
    return sel_str[selectedIndex];
}

int InputDialog::getIntFromSelectedIndexStr(QString selectedIndexStr)
{
    return sel_str.indexOf(selectedIndexStr);
}

void InputDialog::on_pushButton_save_clicked()
{
   bool passCheck = true;
   for (uint item=0;item<DATE_NUM;item++)
   {
      if (checkBoxList[item]->isChecked())
      {
          int dayOfWeek = dateList[item]->date().dayOfWeek();
          if (dayOfWeek==Qt::Saturday||dayOfWeek==Qt::Sunday)
          {
              passCheck = false;
              QMessageBox::information(this, "錯誤", "只能輸入平日\nitem: "+ QString::number(item) +"\n"+ dateList[item]->date().toString("yyyy/MM/dd"));
          };
      }
   }

   if(passCheck)
   {
      for (uint item=0;item<DATE_NUM;item++)
      {
         if (checkBoxList[item]->isChecked())
         {
             mainPtr->setOneRow(dateList[item]->date(), ui->comboBox_selection->currentIndex(),ui->lineEdit_note->text(), true);
         }
      }
   }
}

void InputDialog::on_pushButton_cancel_clicked()
{
    close();
}
