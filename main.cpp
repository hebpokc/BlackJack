#include <QApplication>
#include "MenuWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/new/prefix1/blackjack_icon.png"));

    MenuWindow w;
    w.show();

    return app.exec();
}
