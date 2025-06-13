#ifndef CLICKERWINDOW_H
#define CLICKERWINDOW_H

#include <QMainWindow>
#include <QElapsedTimer>
#include <QTimer>

namespace Ui {
class ClickerWindow;
}

class ClickerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClickerWindow(QWidget *parent = nullptr);
    ~ClickerWindow();

private slots:
    void on_clickButton_clicked();

private:
    Ui::ClickerWindow *ui;

    void updateTimer();
    void endGame();

    int m_clickCount;
    int m_reward;
    QTimer *m_timer;
    QElapsedTimer m_time;
    int m_remainingTime;
};

#endif // CLICKERWINDOW_H
