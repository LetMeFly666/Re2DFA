#pragma once

#include <QtWidgets/QWidget>
#include "ui_Re2DFA.h"

class Re2DFA : public QWidget {
    Q_OBJECT

public:
    Re2DFA(QWidget *parent = Q_NULLPTR);

private:
    Ui::Re2DFAClass ui;
};
