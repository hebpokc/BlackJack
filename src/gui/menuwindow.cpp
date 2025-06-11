#include "MenuWindow.h"
#include "ui_MenuWindow.h"
#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"
#include "multiplayermainwindow.h"

MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/menu/images/resources/background.jpg");
    bkgnd = bkgnd.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(bkgnd));
    this->setPalette(palette);
}

MenuWindow::~MenuWindow()
{
    delete ui;
}

void MenuWindow::on_playBot_clicked()
{
    MainWindow *game = new MainWindow(this);
    game->setAttribute(Qt::WA_DeleteOnClose);
    game->show();
    this->hide();
}

void MenuWindow::on_playPlayer_clicked()
{
    MultiplayerMainWindow *game = new MultiplayerMainWindow(this);
    game->show();
    this->hide();
}

void MenuWindow::on_exit_clicked()
{
    QApplication::quit();
}
