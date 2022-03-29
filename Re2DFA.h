#pragma once

#include <QtWidgets/QWidget>
#include "ui_Re2DFA.h"
#include <stack>
using namespace std;

class Re2DFA : public QWidget {
    Q_OBJECT

public:
    Re2DFA(QWidget *parent = Q_NULLPTR);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Connect_clicked();

    void on_pushButton_Empty_clicked();

    void on_pushButton_Or_clicked();

    void on_pushButton_Repetition_clicked();

    void on_pushButton_Leftbracket_clicked();

    void on_pushButton_Rightbracket_clicked();

private:
    Ui::Re2DFAClass ui;
};

QString addConOp2Re(QString re);
QString Re2RePo(QString re);
