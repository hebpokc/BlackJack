#ifndef MULTIPLAYERMAINWINDOW_H
#define MULTIPLAYERMAINWINDOW_H

#include <QMainWindow>
#include "../game/desk.h"
#include "../game/botplayer.h"
#include "../game/hand.h"
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QStringList>
#include <QCloseEvent>

namespace Ui {
class MultiplayerMainWindow;
}

class MultiplayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MultiplayerMainWindow(QWidget *parent = nullptr);
    ~MultiplayerMainWindow();

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
    void updateUI();
    void updatePlayerScores();
    void startNewGame();
    void displayCards(const std::vector<Card>& cards, QLayout* layout);
    void checkGameOver();
    void switchTurns();
    void endGame(const QString &result);
    void closeEvent(QCloseEvent *event);
    void startBackgroundMusic();
    void playNextTrack();
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

    Ui::MultiplayerMainWindow *ui;

    Desk m_desk;
    Hand m_player1Hand;
    Hand m_player2Hand;
    bool m_isPlayer1Turn;
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;
    QStringList m_playlist;
    int m_currentTrackIndex;
};

#endif // MULTIPLAYERMAINWINDOW_H
