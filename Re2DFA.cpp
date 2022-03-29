#include "Re2DFA.h"

Re2DFA::Re2DFA(QWidget *parent) : QWidget(parent) {
    // 主窗口
    ui.setupUi(this);
    setWindowTitle("Regular To DFA - LetMeFly");
    setFixedSize(1310, 865);

    // 便捷输入
    ui.pushButton_Connect->setToolTip("And Character（中文的“`”）");
    ui.pushButton_Empty->setToolTip("Empty Character");
    ui.pushButton_Or->setToolTip("Or Character");
    ui.pushButton_Repetition->setToolTip("Repetition Character");
    ui.pushButton_Leftbracket->setToolTip("Left Bracket");
    ui.pushButton_Rightbracket->setToolTip("Right Bracket");
}

void Re2DFA::on_pushButton_clicked() {
    ui.label_ReversePolish->setText(Re2RePo(ui.lineEdit->text()));
}

void Re2DFA::on_pushButton_Connect_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "·");
}

void Re2DFA::on_pushButton_Empty_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "ε");
}

void Re2DFA::on_pushButton_Or_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "|");
}


void Re2DFA::on_pushButton_Repetition_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "*");
}

void Re2DFA::on_pushButton_Leftbracket_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "(");
}

void Re2DFA::on_pushButton_Rightbracket_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + ")");
}

const QString Re2RePo(QString re) {
    std::string s = re.toStdString();
    
    return QString::fromStdString(s);
}