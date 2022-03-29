#pragma once

#include <QtWidgets/QWidget>
#include "ui_Re2DFA.h"
#include <stack>
#include <set>
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

const QString showString(QString origin);
QString addConOp2Re(QString re);
QString Re2RePo(QString re);

static set<char> Char = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    ','  // ¦Å
};
