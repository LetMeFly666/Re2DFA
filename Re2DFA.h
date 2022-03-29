#pragma once

#include <QtWidgets/QWidget>
#include <QDesktopServices>
#include <QUrl>
#include "ui_Re2DFA.h"
//#include <utility>  // pair
#include <stack>
#include <set>

using namespace std;

#define CONTINUE_WHEN_NOT_ERRORCODE if (errorCode) return
#define NFA2 pair<char, NFA*>

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
    bool eventFilter(QObject* watched, QEvent* event);
};

/* NFA是一个状态圈+数个向后的边 */
class NFA {
private:
    vector<NFA2> to;
public:
    bool isEnd;
    NFA(bool isEnd = false);
    NFA(char c);  // 构建单字符的基本NFA
    void add2(NFA2 toWho);
    /* 初期构建NFA时，每个NFA只有一个开始状态和一个结束状态。
    *  这个指针主要用于快速定位某个初始状态对应的结束状态。
    *  中间很多NFA的singleEnd都是错误的（空或错）
    *  除了刚构建NFA时的小技巧，尽量不要用这个变量！！！
    */
    NFA* singleEnd;
};

const QString showString(QString origin);
QString addConOp2Re(QString re);
QString re2RePo(QString re);
void rePo2DFA(QString rePo);

static set<char> Char = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    ','  // ε
};
