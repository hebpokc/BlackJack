#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MenuWindow; }
QT_END_NAMESPACE

class MenuWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MenuWindow(QWidget *parent = nullptr);
    ~MenuWindow();

private slots:
    void on_playBot_clicked();
    void on_playPlayer_clicked();
    void on_exit_clicked();

private:
    Ui::MenuWindow *ui;
};
