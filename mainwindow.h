#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QEvent>
#include "calc.h"
#include "date.h"
#include "utility.h"

class MainWindow : public QWidget
{
    Q_OBJECT
    QPushButton *load_file;
    QComboBox *choose_action;
    QTextEdit *show_text;
    QPushButton *button_OK;
    QHBoxLayout *linear_layout;
    QVBoxLayout *left_layout;
    QVBoxLayout *right_layout1;
    QVBoxLayout *right_layout2;
    QVBoxLayout *right_layout3;
    QHBoxLayout *main_layout;
    QLineEdit *edit_begin_month;
    QLabel *label_begin_month;
    QLineEdit *edit_begin_day;
    QLabel *label_begin_day;
    QLineEdit *edit_begin_hour;
    QLabel *label_begin_hour;
    QLineEdit *edit_begin_minute;
    QLabel *label_begin_minute;
    QLineEdit *edit_end_month;
    QLabel *label_end_month;
    QLineEdit *edit_end_day;
    QLabel *label_end_day;
    QLineEdit *edit_end_hour;
    QLabel *label_end_hour;
    QLineEdit *edit_end_minute;
    QLabel *label_end_minute;
    QLineEdit *edit_step_month;
    QLabel *label_step_month;
    QLineEdit *edit_step_day;
    QLabel *label_step_day;
    QLineEdit *edit_step_hour;
    QLabel *label_step_hour;
    QLineEdit *edit_step_minute;
    QLabel *label_step_minute;
    Calc calc;
    date begin_date;
    date end_date;
public:
    explicit MainWindow(QWidget *parent = 0);

private:


protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
    void open_file(const QString& file);
    void do_action();
    void updateDate();
private slots:

};

#endif // MAINWINDOW_H

