#include "Re2DFA.h"

/**错误码errorCode
* 0   无误
* 1   )找不到(
*/
int errorCode = 0;

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
    QString reFormatted = addConOp2Re(ui.lineEdit->text());
    ui.label_ReformatReString->setText(showString(reFormatted));
    // TODO: EOORO的处理
    QString reversePolish = Re2RePo(reFormatted);
    ui.label_ReversePolish->setText(showString(reversePolish));
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

/* 将用,.代替的字符串转换为ε·代表的字符串 */
const QString showString(QString origin) {
    string s = origin.toStdString();
    string ans;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] == ',')
            ans += "ε";
        else if (s[i] == '.')
            ans += "·";
        else
            ans += s[i];
    }
    return QString::fromStdString(ans);
}

/* 添加“·”到正则表达式 */
QString addConOp2Re(QString re) {
    string s = re.toStdString();
    string temp;
    for (int i = 0; i + 1 < s.size(); i++) {
        if ((unsigned char)s[i] == 194 && (unsigned char)s[i + 1] == 183) {  // ·
            temp += '.';
            i++;
        }
        else if ((unsigned char)s[i] == 206 && (unsigned char)s[i + 1] == 181) {  // ε
            temp += ',';
            i++;
        }
        else {
            temp += s[i];
        }
    }
    temp += s[s.size() - 1];
    s = temp;
    string ans;
    for (int i = 0; i + 1 < s.size(); i++) {
        ans += s[i];
        if ((Char.count(s[i]) && Char.count(s[i + 1])) || (Char.count(s[i]) && s[i + 1] == '(') || (s[i] == ')' && Char.count(s[i + 1]))) {
            ans += '.';
        }
    }
    ans += s[s.size() - 1];
    return QString::fromStdString(ans);
}

QString Re2RePo(QString re) {
    string s = re.toStdString();
    string ans;
    stack<char> op;
    /* 比较a和b的优先级(不包含括号) a>b返回正数，a<b返回负数，a=b返回0 */
    auto priority = [](char a, char b) {
        // * > · > |
        auto c2i = [](char c) {
            if (c == '*') {
                return 2;
            }
            else if (c == '.') {
                return 1;
            }
            else if (c == '|') {
                return 0;
            }
            else {
                return -1;
            }
        };
        return c2i(a) - c2i(b);
    };
    for (int i = 0; i < s.size(); i++) {
        if (Char.count(s[i])) {
            ans += s[i];
            continue;
        }
        if (s[i] == '(') {
            op.push(s[i]);
        }
        else if (s[i] == ')') {
            while (true) {
                if (op.empty()) {
                    errorCode = 1;
                    return "Error1: )without(";
                }
                if (op.top() == '(') {
                    op.pop();
                    break;
                }
                ans += op.top();
                op.pop();
            }
        }
        else {
            while (true) {
                if (op.empty() || op.top() == '(' || priority(op.top(), s[i]) < 0) {
                    op.push(s[i]);
                    break;
                }
                else {
                    ans += op.top();
                    op.pop();
                }
            }
        }
    }
    while (op.size()) {
        ans += op.top();
        op.pop();
    }
    return QString::fromStdString(ans);
}