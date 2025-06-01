#include "MenuWindow.h"
#include "ui_MenuWindow.h"
#include <QApplication>
#include <QMessageBox>

MenuWindow::MenuWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MenuWindow)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/new/prefix1/background.jpg");
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
    QMessageBox::information(this, "Играть против бота", "Запуск игры против бота (заглушка)");
    // TODO: Запуск игрового окна против бота
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
