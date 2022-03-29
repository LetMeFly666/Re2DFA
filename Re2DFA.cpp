#include "Re2DFA.h"

Re2DFA::Re2DFA(QWidget *parent) : QWidget(parent) {
    // 主窗口
    ui.setupUi(this);
    setWindowTitle("Regular To DFA - LetMeFly");
    setFixedSize(1310, 865);

    // 便捷输入
    ui.pushButton_Connect->setToolTip("连接符（中文的“`”）");
}

void Re2DFA::on_pushButton_clicked() {
    
}

void Re2DFA::on_pushButton_Connect_clicked() {
    ui.lineEdit->setText(ui.lineEdit->text() + "·");
}

void Re2DFA::on_pushButton_Empty_clicked() {

}

void Re2DFA::on_pushButton_Or_clicked() {

}

void Re2DFA::on_pushButton_Leftbracket_clicked() {

}

void Re2DFA::on_pushButton_Rightbracket_clicked() {

}
