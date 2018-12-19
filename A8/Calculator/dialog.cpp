#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::setNumber(QString str)
{
    if(QString::compare(ui->lineEdit_op->text(),"") == 0) {
        ui->lineEdit_first->setText(ui->lineEdit_first->text() + str);
    }
    else {
        ui->lineEdit_second->setText(ui->lineEdit_second->text() + str);
    }
}

void Dialog::on_pushButton_1_clicked()
{
    setNumber("1");
}

void Dialog::on_pushButton_2_clicked()
{
    setNumber("2");
}

void Dialog::on_pushButton_3_clicked()
{
    setNumber("3");
}

void Dialog::on_pushButton_4_clicked()
{
    setNumber("4");
}

void Dialog::on_pushButton_5_clicked()
{
    setNumber("5");
}

void Dialog::on_pushButton_6_clicked()
{
    setNumber("6");
}

void Dialog::on_pushButton_7_clicked()
{
    setNumber("7");
}

void Dialog::on_pushButton_8_clicked()
{
    setNumber("8");
}

void Dialog::on_pushButton_9_clicked()
{
    setNumber("9");
}

void Dialog::on_pushButton_0_clicked()
{
    setNumber("0");
}

void Dialog::on_pushButton_clean_clicked()
{
    ui->lineEdit_first->setText("");
    ui->lineEdit_op->setText("");
    ui->lineEdit_result->setText("");
    ui->lineEdit_second->setText("");
}

void Dialog::on_pushButton_add_clicked()
{
    ui->lineEdit_op->setText("+");
}

void Dialog::on_pushButton_minus_clicked()
{
    ui->lineEdit_op->setText("-");
}

void Dialog::on_pushButton_multi_clicked()
{
    ui->lineEdit_op->setText("*");
}

void Dialog::on_pushButton_div_clicked()
{
    ui->lineEdit_op->setText("/");
}

void Dialog::on_pushButton_result_clicked()
{
    if(ui->lineEdit_op->text() == "+")
        ui->lineEdit_result->setText(QString::number(ui->lineEdit_first->text().toInt(NULL, 10) + ui->lineEdit_second->text().toInt(NULL, 10)));

    else if (ui->lineEdit_op->text() == "-")
        ui->lineEdit_result->setText(QString::number(ui->lineEdit_first->text().toInt(NULL, 10) - ui->lineEdit_second->text().toInt(NULL, 10)));

    else if (ui->lineEdit_op->text() == "*")
        ui->lineEdit_result->setText(QString::number(ui->lineEdit_first->text().toInt(NULL, 10) * ui->lineEdit_second->text().toInt(NULL, 10)));

    else if (ui->lineEdit_op->text() == "/")
        ui->lineEdit_result->setText(QString::number(ui->lineEdit_first->text().toInt(NULL, 10) / ui->lineEdit_second->text().toInt(NULL, 10)));
}



void Dialog::on_pushButton_close_clicked()
{
    close();
}
