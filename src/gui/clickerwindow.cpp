#include "clickerwindow.h"
#include "mainwindow.h"
#include "ui_clickerwindow.h"
#include <QMessageBox>
#include <QTimer>
#include <QTime>

ClickerWindow::ClickerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClickerWindow)
    , m_clickCount(0)
    , m_reward(0)
    , m_remainingTime(30)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/game/images/resources/clicker_background.png");
    bkgnd = bkgnd.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(bkgnd));
    this->setPalette(palette);

    // Инициализация таймера
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &ClickerWindow::updateTimer);

    // Инициализация времени
    m_time.start();
    m_timer->start(1000);

    ui->labelClicks->setText("Клики: 0");
    ui->labelReward->setText("Награда: 0 монет 🪙");
    ui->labelTimeRemaining->setText("Оставшееся время: 30 секунд");
}

ClickerWindow::~ClickerWindow()
{
    delete ui;
}

void ClickerWindow::on_clickButton_clicked()
{
    m_clickCount++;
    ui->labelClicks->setText("Клики: " + QString::number(m_clickCount));

    // Обновляем награду
    if (m_clickCount >= 120) {
        m_reward = 60 + (m_clickCount - 50) * 2; // За каждый следующий блок из 10 кликов +10 монет
    } else if (m_clickCount >= 90) {
        m_reward = 25;
    } else if (m_clickCount >= 60) {
        m_reward = 5;
    }

    ui->labelReward->setText("Награда: " + QString::number(m_reward) + " монет 🪙");

    if (m_time.elapsed() >= 30000) {
        endGame();
    }
}

void ClickerWindow::updateTimer()
{

    // Уменьшаем оставшееся время
    m_remainingTime = 30 - m_time.elapsed() / 1000;

    // Обновляем текстовое поле для оставшегося времени
    ui->labelTimeRemaining->setText("Оставшееся время: " + QString::number(m_remainingTime) + " секунд");
    if (m_time.elapsed() >= 30000) {
        endGame();
    }
}

void ClickerWindow::endGame()
{
    m_timer->stop();

    QMessageBox::information(this, "Игра окончена", "Вы сделали " + QString::number(m_clickCount) + " кликов.\nНаграда: " + QString::number(m_reward) + " монет 🪙");

    // Обновляем баланс в главном окне
    MainWindow *mainWindow = qobject_cast<MainWindow*>(parent());
    if (mainWindow) {
        mainWindow->updateBalance(m_reward);
    }

    this->close();
}
