#include "editdialog.h"
#include "ui_editdialog.h"
#include "qtattendancemain.h"
#include <QMessageBox>

EditDialog::EditDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditDialog)
{
    ui->setupUi(this);
    ui->label_id_1->setTextInteractionFlags(Qt::TextSelectableByMouse);//Make QLabel text selectable. https://stackoverflow.com/questions/17957749/make-qlabel-text-selectable/17957750
    ui->comboBox_selection->addItems(sel_str); //@@@@重複定義 很醜
}

EditDialog::~EditDialog()
{
    delete ui;
}

QString EditDialog::getId()
{
    return ui->label_id_1->text();
}

void EditDialog::setId(QString id)
{
    ui->label_id_1->setText(id);
}

void EditDialog::setIndex(int index)
{
    ui->label_rowIndex_1->setText(QString::number(index));
}

void EditDialog::setDate(QDate date)
{
    ui->dateEdit_01->setDate(date); //@@ 轉換格式該怎麼區分才好，應該處理在誰家?
}

void EditDialog::setSelection(int sel)
{
    ui->comboBox_selection->setCurrentIndex(sel);
}

void EditDialog::setNote(QString note)
{
    ui->lineEdit_note->setText(note);
    ui->lineEdit_note->setFocus();
}

void EditDialog::on_pushButton_delete_clicked()
{
    int rowIndex =ui->label_rowIndex_1->text().toInt();
    mainPtr->deleteOneRow(rowIndex-1); //@@ 顯示上 第一項為1  座標上 第一項為0
    mainPtr->saveToCsv();
    mainPtr->updateRate(); //@@ 一直接續的行為  要不要打包在一起
    this->close();
}

void EditDialog::on_pushButton_save_clicked()
{
    int rowIndex =ui->label_rowIndex_1->text().toInt();
    bool passCheck = true;
    int dayOfWeek = ui->dateEdit_01->date().dayOfWeek();
    if (dayOfWeek==Qt::Saturday||dayOfWeek==Qt::Sunday)
    {
        passCheck = false;
        QMessageBox::information(this, "錯誤", "只能輸入平日\nitem: "+ QString::number(1) +"\n"+ ui->dateEdit_01->date().toString("yyyy/MM/dd"));
    };

    if(passCheck)
    {
       mainPtr->deleteOneRow(rowIndex-1); //@@ 顯示上 第一項為1  座標上 第一項為0
       mainPtr->setOneRow(ui->dateEdit_01->date(), ui->comboBox_selection->currentIndex(),ui->lineEdit_note->text(), true);
       this->close();
    }
}

void EditDialog::on_pushButton_cancel_clicked()
{
    this->close();
}
