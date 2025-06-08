#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../game/desk.h"
#include "../game/botplayer.h"
#include "../game/hand.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QCloseEvent>

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

    void on_actionExit_triggered();
    void on_actionHit_triggered();
    void on_actionStand_triggered();
    void on_actionNewGame_triggered();
    void on_actionRule1_triggered();
    void on_actionRule2_triggered();
    void on_actionMute_triggered();
    void on_actionUnmute_triggered();

private:
    void startNewGame();
    void displayCards(const std::vector<Card>& cards, QLayout* layout);
    void displayBotCards(const std::vector<Card>& cards, QLayout* layout, bool revealAll);
    void updateUI();
    void botTurn();
    void endGame(const QString &result);
    void startBackgroundMusic();
    void playNextTrack();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);
    void closeEvent(QCloseEvent *event);

    Ui::MainWindow *ui;

    Desk m_desk;
    Hand m_playerHand;
    BotPlayer m_bot;
    bool m_playerTurn;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QStringList m_playlist;
    int m_currentTrackIndex;
};
#endif // MAINWINDOW_H
