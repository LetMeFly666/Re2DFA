#include "Re2DFA.h"

/**错误码errorCode
* 0   无误
* 1   )找不到(
* 2   双目运算符无法出栈两个NFA
* 3   单目运算符无法出栈单个NFA
* 4   NFA构建完毕后栈中FNA个数不为一
* 5   出现不受支持的字符
*/
int errorCode = 0;

Re2DFA::Re2DFA(QWidget *parent) : QWidget(parent) {
    // 主窗口
    ui.setupUi(this);
    setWindowTitle("Regular To DFA - LetMeFly");
    setFixedSize(1310, 865);
    ui.label_Link->installEventFilter(this);
    ui.widget_htmlDoc->load(QUrl("https://re2dfa.letmefly.xyz/"));
    ui.widget_htmlNFA->load(QUrl("file:///del.html"));
    initTabwidget(ui);
    

    // 便捷输入
    ui.pushButton_Connect->setToolTip("And Character（中文的“`”）");
    ui.pushButton_Empty->setToolTip("Empty Character");
    ui.pushButton_Or->setToolTip("Or Character");
    ui.pushButton_Repetition->setToolTip("Repetition Character");
    ui.pushButton_Leftbracket->setToolTip("Left Bracket");
    ui.pushButton_Rightbracket->setToolTip("Right Bracket");
}

void Re2DFA::on_pushButton_clicked() {
    initTabwidget(ui);
    QString reFormatted = addConOp2Re(ui.lineEdit->text());
    ui.label_ReformatReString->setText(showString(reFormatted));
    CONTINUE_WHEN_NOT_ERRORCODE;
    QString reversePolish = re2RePo(reFormatted);
    ui.label_ReversePolish->setText(showString(reversePolish));
    CONTINUE_WHEN_NOT_ERRORCODE;
    NFA* begin = rePo2DFA(reversePolish);
    CONTINUE_WHEN_NOT_ERRORCODE;

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
    singleEnd = nullptr;
}

NFA::NFA(char c) {
    isEnd = false;  // 自己不是终止状态
    NFA* end = new NFA(true);
    singleEnd = end;
    add2({ c, end });  // 连接了一个终止状态的无子NFA
}

NFA::~NFA() {
    delete this;
}

void NFA::add2(NFA2 toWho) {
    to.push_back(toWho);
}

void initTabwidget(Ui::Re2DFAClass& ui) {
    ui.label_ReformatReString->setText("\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\271\266\347\202\271\345\207\273\350\275\254\346\215\242\346\214\211\351\222\256");  // 请输入正则表达式并点击转换按钮
    ui.label_ReversePolish->setText("\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\271\266\347\202\271\345\207\273\350\275\254\346\215\242\346\214\211\351\222\256");  // 请输入正则表达式并点击转换按钮
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
        else if (Op.count(s[i]) || Char.count(s[i])) {
            temp += s[i];
        }
        else {
            errorCode = 5;
            // 第{i + 1}个字符{错误字符}为不受支持的字符
            string error = "\347\254\254" + to_string((i + 1)) + "\344\270\252\345\255\227\347\254\246'" + s[i] + "'\344\270\272\344\270\215\345\217\227\346\224\257\346\214\201\347\232\204\345\255\227\347\254\246";
            return QString::fromStdString(error);
        }
    }
    s = temp;
    string ans;
    for (int i = 0; i < s.size(); i++) {
        ans += s[i];
        if ((Char.count(s[i]) && i + 1 < s.size() && Char.count(s[i + 1])) 
            || (Char.count(s[i]) && i + 1 < s.size() && s[i + 1] == '(') 
            || (s[i] == ')' && i + 1 < s.size() && Char.count(s[i + 1]))
            || (s[i] == '*' && i + 1 < s.size() && Char.count(s[i + 1]))
            || (s[i] == '*' && i + 1 < s.size() && s[i + 1] == '(')
            ) {
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

NFA* rePo2DFA(QString rePo) {
    string s = rePo.toStdString();
    stack<NFA*> st;
    for (int i = 0; i < s.size(); i++) {
        if (Char.count(s[i])) {
            st.push(new NFA(s[i]));
        }
        else if (s[i] == '|') {
            if (st.size() < 2) {
                errorCode = 2;
                return nullptr;
            }
            NFA* back = st.top();
            st.pop();
            NFA* front = st.top();
            st.pop();
            NFA* begin = new NFA();
            NFA* end = new NFA(true);
            begin->singleEnd = end;
            begin->add2({ ',', front });
            begin->add2({ ',', back });
            front->singleEnd->isEnd = false;
            front->singleEnd->add2({ ',', end });
            back->singleEnd->isEnd = false;
            back->singleEnd->add2({ ',', end });
            st.push(begin);
        }
        else if (s[i] == '.') {
            if (st.size() < 2) {
                errorCode = 2;
                return nullptr;
            }
            NFA* back = st.top();
            st.pop();
            NFA* front = st.top();
            st.pop();
            NFA* begin = new NFA();
            NFA* end = new NFA(true);
            begin->singleEnd = end;
            begin->add2({ ',', front });
            front->singleEnd->isEnd = false;
            front->singleEnd->add2({ ',', back });
            back->singleEnd->isEnd = false;
            back->singleEnd->add2({ ',', end });
            st.push(begin);
        }
        else if (s[i] == ',') {
            if (st.empty()) {
                errorCode = 3;
                return nullptr;
            }
            NFA* a = st.top();
            st.pop();
            NFA* begin = new NFA();
            NFA* end = new NFA(true);
            begin->singleEnd = end;
            begin->add2({ ',', a });
            begin->add2({ ',', end });
            a->singleEnd->add2({ ',', a });
            a->singleEnd->add2({ ',', end });
            a->singleEnd->isEnd = false;
            st.push(begin);
        }
    }
    if (st.size() != 1) {
        errorCode = 4;
        return nullptr;
    }
    return st.top();
}
