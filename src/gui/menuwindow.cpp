#include "MenuWindow.h"
#include "ui_MenuWindow.h"
#include <QApplication>
#include <QMessageBox>
#include "mainwindow.h"

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
    QMessageBox::information(this, "Играть против игрока", "Запуск игры против другого игрока (заглушка)");
    // TODO: Запуск игрового окна для 2 игроков
}

void MenuWindow::on_exit_clicked()
{
    QApplication::quit();
}
