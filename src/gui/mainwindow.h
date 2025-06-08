#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../game/desk.h"
#include "../game/botplayer.h"
#include "../game/hand.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonHit_clicked();
    void on_buttonStand_clicked();
    void on_buttonNewGame_clicked();

private:
    void startNewGame();
    void displayCards(const std::vector<Card>& cards, QLayout* layout);
    void displayBotCards(const std::vector<Card>& cards, QLayout* layout, bool revealAll);
    void updateUI();
    void botTurn();
    void endGame(const QString &result);

    Ui::MainWindow *ui;

    Desk m_desk;
    Hand m_playerHand;
    BotPlayer m_bot;
    bool m_playerTurn;
};
#endif // MAINWINDOW_H
