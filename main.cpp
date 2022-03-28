#include "Re2DFA.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Re2DFA w;
    w.show();
    return a.exec();
}
