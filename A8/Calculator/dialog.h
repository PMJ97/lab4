#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    void setNumber(QString str);
    ~Dialog();

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_0_clicked();

    void on_pushButton_clean_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_minus_clicked();

    void on_pushButton_multi_clicked();

    void on_pushButton_div_clicked();

    void on_pushButton_result_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
