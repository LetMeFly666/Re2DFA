#include "Re2DFA.h"

/**错误码errorCode
* 0   无误
* 1   )找不到(
* 2   双目运算符无法出栈两个字符NFA
*/
int errorCode = 0;

Re2DFA::Re2DFA(QWidget *parent) : QWidget(parent) {
    // 主窗口
    ui.setupUi(this);
    setWindowTitle("Regular To DFA - LetMeFly");
    setFixedSize(1310, 865);
    ui.label_Link->installEventFilter(this);

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
    CONTINUE_WHEN_NOT_ERRORCODE;
    QString reversePolish = re2RePo(reFormatted);
    ui.label_ReversePolish->setText(showString(reversePolish));
    CONTINUE_WHEN_NOT_ERRORCODE;
    rePo2DFA(reversePolish);
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

bool Re2DFA::eventFilter(QObject* watched, QEvent* event) {
    if (watched == ui.label_Link && event->type() == QEvent::MouseButtonPress) {
        QDesktopServices::openUrl(QUrl(QLatin1String("https://re2dfa.letmefly.xyz")));
    }
    return QWidget::eventFilter(watched, event);
}

NFA::NFA(bool isEnd) : isEnd(isEnd) {

}

NFA::NFA(char c) {
    isEnd = false;  // 自己不是终止状态
    NFA* end = new NFA(true);
    singleEnd = end;
    add2({ c, end });  // 连接了一个终止状态的无子NFA
}

void NFA::add2(NFA2 toWho) {
    to.push_back(toWho);
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
    for (int i = 0; i < s.size(); i++) {
        if ((unsigned char)s[i] == 194 && i + 1 < s.size() && (unsigned char)s[i + 1] == 183) {  // ·
            temp += '.';
            i++;
        }
        else if ((unsigned char)s[i] == 206 && i + 1 < s.size() && (unsigned char)s[i + 1] == 181) {  // ε
            temp += ',';
            i++;
        }
        else {
            temp += s[i];
        }
    }
    s = temp;
    string ans;
    for (int i = 0; i < s.size(); i++) {
        ans += s[i];
        if ((Char.count(s[i]) && i + 1 < s.size() && Char.count(s[i + 1])) || (Char.count(s[i]) && i + 1 < s.size() && s[i + 1] == '(') || (s[i] == ')' && i + 1 < s.size() && Char.count(s[i + 1]))) {
            ans += '.';
        }
    }
    return QString::fromStdString(ans);
}

QString re2RePo(QString re) {
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

void rePo2DFA(QString rePo) {
    string s = rePo.toStdString();
    stack<NFA*> st;
    for (int i = 0; i < s.size(); i++) {
        if (Char.count(s[i])) {
            st.push(new NFA(s[i]));
        }
        else if (s[i] == '|') {
            if (st.size() < 2) {
                errorCode = 2;
                return;
            }
            NFA* back = st.top();
            st.pop();
            NFA* front = st.top();
            st.pop();
            NFA* begin = new NFA();
            NFA* end = new NFA(true);
            begin->add2({ ',', front });
            begin->add2({ ',', back });
            begin->singleEnd = end;
            front->singleEnd->isEnd = false;
            front->singleEnd->add2({ ',', end });
            end->singleEnd->isEnd = false;
            end->singleEnd->add2({ ',', end });
        }
    }
}