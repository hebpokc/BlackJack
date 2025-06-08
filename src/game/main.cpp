#include <QApplication>
#include "../gui/MenuWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon(":/menu/images/resources/blackjack_icon.png"));

    MenuWindow w;
    w.show();

    return app.exec();
}
