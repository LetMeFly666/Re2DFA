#include "Re2DFA.h"

/**错误码errorCode
* 0   无误
* 1   )找不到(
* 2   双目运算符无法出栈两个NFA
* 3   单目运算符无法出栈单个NFA
* 4   NFA构建完毕后栈中FNA个数不为一
* 5   出现不受支持的字符
* 6   内置文件丢失
* 7   无写文件权限
*/
int errorCode = 0;
set<char> charsAppearedWithoutEmpty;

Re2DFA::Re2DFA(QWidget *parent) : QWidget(parent) {
    // 主窗口
    ui.setupUi(this);
    setWindowTitle("Regular To DFA - LetMeFly");
    setFixedSize(1310, 865);
    initTabwidget(ui);
    ui.label_Link->installEventFilter(this);
    ui.widget_htmlDoc->load(QUrl("https://re2dfa.letmefly.xyz/"));

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
    NFA* NFABegin = rePo2NFA(reversePolish);
    CONTINUE_WHEN_NOT_ERRORCODE;
    visualizeNFA(NFABegin, ui);
    CONTINUE_WHEN_NOT_ERRORCODE;
    TableWithBeginEnd NFAStateTable = NFA2NFAStateTable(NFABegin, ui);
    DFA* DFABegin = table2DFA(NFAStateTable, NFABegin->singleEnd);
    visualizeDFA(DFABegin, ui.widget_htmlDFA, "outputDFA.html");
    CONTINUE_WHEN_NOT_ERRORCODE;
    DFA* DFASBegin = simplifyDFA(DFABegin, ui);
    CONTINUE_WHEN_NOT_ERRORCODE;
    visualizeDFA(DFASBegin, ui.widget_htmlDFA_simplified, "outputDFASimplified.html");
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

DFA::DFA(bool isEnd) : isEnd(isEnd) {

}

DFA::~DFA() {
    delete this;
}

void DFA::add2(DFA2 toWho) {
    to.push_back(toWho);
}

string Visualizer::getFileData(const char* fileName) {
    string data;
    string oneLine;
    ifstream istr(fileName, ios::in);
    if (!istr.is_open()) {
        errorCode = 6;
        return (string)"";
    }
    while (getline(istr, oneLine)) {
        data += oneLine + "\n";
    }
    istr.close();
    return data;
}

string Visualizer::showChar(char c) {
    if (c == ',')
        return (string)"ε";
    string s;
    s += c;
    return s;
}

void Visualizer::toFile(const char* fileName, string& data, int& errorCode) {
    ofstream ostr(fileName, ios::out);
    if (!ostr.is_open()) {
        errorCode = 7;
        return;
    }
    ostr << data;
    ostr.close();
}

void initTabwidget(Ui::Re2DFAClass& ui) {
    errorCode = 0;
    charsAppearedWithoutEmpty.clear();
    ui.label_ReformatReString->setText("\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\271\266\347\202\271\345\207\273\350\275\254\346\215\242\346\214\211\351\222\256");  // 请输入正则表达式并点击转换按钮
    ui.label_ReversePolish->setText("\350\257\267\350\276\223\345\205\245\346\255\243\345\210\231\350\241\250\350\276\276\345\274\217\345\271\266\347\202\271\345\207\273\350\275\254\346\215\242\346\214\211\351\222\256");  // 请输入正则表达式并点击转换按钮
    ui.widget_htmlNFA->load(QUrl("file:///initialDFA.html"));
    ui.widget_htmlDFA->load(QUrl("file:///initialDFA.html"));
    ui.widget_htmlDFA_simplified->load(QUrl("file:///initialDFA.html"));
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

/* 添加“·”到正则表达式，并将“·”转为“.”、“ε”转为“,” */
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
        // 字符
        if (Char.count(s[i])) {
            ans += s[i];
            if (s[i] != ',')
                charsAppearedWithoutEmpty.insert(s[i]);
            continue;
        }
        // 运算符
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

NFA* rePo2NFA(QString rePo) {
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
        else if (s[i] == '*') {
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

void visualizeNFA(NFA* head, Ui::Re2DFAClass& ui) {
    Visualizer vis;
    string data = vis.getFileData("DFA_head.html");
    if (errorCode) return;
    // #region: generate mermaid code
    data += "End((End))\nstyle End stroke-width:5px\n";
    set<NFAf2> alreadyPath;
    map<NFA*, string> nodeMap;
    nodeMap[head] = "Begin";
    nodeMap[head->singleEnd] = "End";
    queue<NFA*> nodes;
    nodes.push(head);
    int nodeNum = 1;
    while (nodes.size()) {
        NFA* node = nodes.front();
        nodes.pop();
        for (NFA2 to : node->to) {
            if (alreadyPath.count({node, to})) {
                continue;
            }
            alreadyPath.insert({ node, to });
            NFA* nextNode = to.second;
            if (!nodeMap.count(nextNode)) {
                nodes.push(nextNode);
                nodeMap[nextNode] = to_string(nodeNum++);
                data += nodeMap[nextNode] + "((" + nodeMap[nextNode] + "))\n";
            }
            data += nodeMap[node] + " --" + vis.showChar(to.first) + "--> " + nodeMap[nextNode] + "\n";
        }
    }
    // #endregion
    data += vis.getFileData("DFA_tail.html");
    vis.toFile("outputNFA.html", data, errorCode);
    // 显示
    ui.widget_htmlNFA->load(QUrl("file:///outputNFA.html"));
}

TableWithBeginEnd NFA2NFAStateTable(NFA* head, Ui::Re2DFAClass& ui) {
    auto getEmptyClosure = [](State nowState) {  // 经过数个ε
        State newState = nowState;
        queue<NFA*> q;
        for (NFA* thisNode : nowState) {
            q.push(thisNode);
        }
        while (q.size()) {
            NFA* thisNode = q.front();
            q.pop();
            for (NFA2 to : thisNode->to) {
                if (to.first == ',') {
                    NFA* toNode = to.second;
                    if (!newState.count(toNode)) {
                        newState.insert(toNode);
                        q.push(toNode);
                    }
                }
            }
        }
        return newState;
    };
    auto state2state = [getEmptyClosure](State nowState, char c) {
        State newState;
        for (NFA* thisNode : nowState) {  // 每个节点走1次char
            for (NFA2 to : thisNode->to) {
                if (to.first == c) {
                    newState.insert(to.second);
                }
            }
        }
        newState = getEmptyClosure(newState);
        return newState;
    };

    Table table;
    queue<State> toCal;
    set<State> alreadyInQueue;
    State beginState = getEmptyClosure({ head });
    toCal.push(beginState);
    alreadyInQueue.insert(beginState);

    while (toCal.size()) {
        State thisState = toCal.front();
        toCal.pop();
        for (char c : charsAppearedWithoutEmpty) {
            State newState = state2state(thisState, c);
            if (newState.empty()) {  // 不需要全空的状态！
                continue;
            }
            table[thisState][c] = newState;
            if (!alreadyInQueue.count(newState)) {
                toCal.push(newState);
                alreadyInQueue.insert(newState);
            }
        }
    }

    TableWithBeginEnd tableWithBegin = { table, beginState, head->singleEnd };
    return tableWithBegin;
}

DFA* table2DFA(TableWithBeginEnd tableWithBegin, NFA* NFAOnlyEnd) {
    DFA* head = new DFA();
    auto [table, beginState, NFAEnd] = tableWithBegin;
    map<State, DFA*> ma;
    queue<State> q;
    ma[beginState] = head;
    q.push(beginState);
    while (q.size()) {
        State thisState = q.front();
        DFA* thisDFA = ma[thisState];
        q.pop();
        for (pair<char, State> to : table[thisState]) {
            State toState = to.second;
            if (!ma.count(toState)) {
                q.push(toState);
                ma[toState] = new DFA(toState.count(NFAEnd));
            }
            DFA* toDFA = ma[toState];
            thisDFA->add2({ to.first, toDFA });
        }
    }
    return head;
}

void visualizeDFA(DFA* head, QWebEngineView* displayAtWhere, const char* outputFilename) {
    Visualizer vis;
    string data = vis.getFileData("DFA_head.html");
    if (errorCode) return;
    // #region: generate mermaid code
    set<DFAf2> alreadyPath;
    map<DFA*, string> nodeMap;
    nodeMap[head] = "Begin";
    queue<DFA*> nodes;
    nodes.push(head);
    int nodeNum = 1;
    while (nodes.size()) {
        DFA* node = nodes.front();
        nodes.pop();
        for (DFA2 to : node->to) {
            if (alreadyPath.count({ node, to })) {
                continue;
            }
            alreadyPath.insert({ node, to });
            DFA* nextNode = to.second;
            if (!nodeMap.count(nextNode)) {
                nodes.push(nextNode);
                nodeMap[nextNode] = to_string(nodeNum++);
                data += nodeMap[nextNode] + "((" + nodeMap[nextNode] + "))\n";
                if (nextNode->isEnd) {
                    data += "style " + nodeMap[nextNode] + " stroke-width:5px\n";
                }
            }
            data += nodeMap[node] + " --" + vis.showChar(to.first) + "--> " + nodeMap[nextNode] + "\n";
        }
    }
    // #endregion
    data += vis.getFileData("DFA_tail.html");
    vis.toFile(outputFilename, data, errorCode);
    displayAtWhere->load(QUrl(("file:///" + string(outputFilename)).c_str()));
    return;
}

DFA* simplifyDFA(DFA* head, Ui::Re2DFAClass& ui) {
    /* DFA Head -> <Area Code, All DFAs> */
    auto getInitialAreacode = [](DFA* head) {
        set<DFA*> visited;
        queue<DFA*> q;
        map<DFA*, int> DFA2Areacode;
        q.push(head);
        visited.insert(head);
        while (q.size()) {
            DFA* thisDFA = q.front();
            q.pop();
            DFA2Areacode[thisDFA] = thisDFA->isEnd;  // 先按终止状态分为两组
            for (DFA2 to : thisDFA->to) {
                DFA* newDFA = to.second;
                if (!visited.count(newDFA)) {
                    visited.insert(newDFA);
                    q.push(newDFA);
                }
            }
        }
        return make_pair(DFA2Areacode, visited);
    };
    /* 为每个区号建立DFA */
    auto fromAreacode2DFA = [](map<DFA*, int> ma, set<DFA*> allDFAs) {
        map<int, DFA*> ans;
        set<int> alreadyAreacode;
        for (DFA* thisDFA : allDFAs) {
            int areaCode = ma[thisDFA];
            if (alreadyAreacode.count(areaCode)) {
                continue;
            }
            alreadyAreacode.insert(areaCode);
            DFA* newDFA = new DFA(thisDFA->isEnd);
            ans[areaCode] = newDFA;
        }
        return ans;
    };
    auto [DFA2Areacode , allDFAs] = getInitialAreacode(head);  // 区号
    while (true) {
        map<DFA*, int> thisAreacode;
        int cntAreacode = 0;
        map<map<char, int>, int> tempAreacode;
        for (DFA* thisDFA : allDFAs) {
            map<char, int> thisDFA2s;
            for (DFA2 to : thisDFA->to) {
                char path = to.first;
                DFA* toDFA = to.second;
                thisDFA2s[path] = DFA2Areacode[toDFA];
            }
            if (tempAreacode.count(thisDFA2s)) {
                thisAreacode[thisDFA] = tempAreacode[thisDFA2s];
            }
            else {
                thisAreacode[thisDFA] = tempAreacode[thisDFA2s] = cntAreacode++;
            }
        }
        if (thisAreacode == DFA2Areacode)
            break;
        DFA2Areacode = thisAreacode;
    }
    map<int, DFA*> areacode2DFA = fromAreacode2DFA(DFA2Areacode, allDFAs);  // 区号->DFA
    set<int> alreadyAreacode;
    for (DFA* thisDFA : allDFAs) {
        int thisAreacode = DFA2Areacode[thisDFA];
        if (alreadyAreacode.count(thisAreacode)) {
            continue;
        }
        alreadyAreacode.insert(thisAreacode);
        DFA* newDFA = areacode2DFA[thisAreacode];
        for (auto [path, toDFA] : thisDFA->to) {
            newDFA->add2({ path, areacode2DFA[DFA2Areacode[toDFA]] });
        }
    }
    DFA* headS = areacode2DFA[DFA2Areacode[head]];
    return headS;
}