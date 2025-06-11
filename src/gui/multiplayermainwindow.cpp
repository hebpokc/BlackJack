#include "multiplayermainwindow.h"
#include "menuwindow.h"
#include "ui_multiplayermainwindow.h"
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QCloseEvent>

MultiplayerMainWindow::MultiplayerMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MultiplayerMainWindow)
    , m_currentTrackIndex(0)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/game/images/resources/game_background.jpg");
    bkgnd = bkgnd.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(bkgnd));
    this->setPalette(palette);

    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);

    m_player->setAudioOutput(m_audioOutput);

    m_audioOutput->setVolume(50);

    m_playlist << "qrc:/music/resources/music/1.mp3"
               << "qrc:/music/resources/music/2.mp3"
               << "qrc:/music/resources/music/3.mp3"
               << "qrc:/music/resources/music/4.mp3";

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MultiplayerMainWindow::onMediaStatusChanged);

    startBackgroundMusic();

    m_isPlayer1Turn = true;

    startNewGame();
}

MultiplayerMainWindow::~MultiplayerMainWindow()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
}

void MultiplayerMainWindow::startNewGame() {
    m_desk.shuffle();

    m_player1Hand.clear();
    m_player2Hand.clear();

    // Раздаем по 2 карты игрокам
    m_player1Hand.addCard(m_desk.draw());
    m_player1Hand.addCard(m_desk.draw());

    m_player2Hand.addCard(m_desk.draw());
    m_player2Hand.addCard(m_desk.draw());

    updateUI();

    ui->whooseTurn->setText("Текущий ход: игрок 1");

    ui->buttonHit->setEnabled(true);
    ui->buttonStand->setEnabled(true);
}

void MultiplayerMainWindow::updateUI()
{
    QLayout* player1Layout = ui->widgetPlayer1Cards->layout();
    QLayout* player2Layout = ui->widgetPlayer2Cards->layout();
    // Обновляем карты на экране
    displayCards(m_player1Hand.getCards(), player1Layout);
    displayCards(m_player2Hand.getCards(), player2Layout);

    updatePlayerScores();
}

void MultiplayerMainWindow::updatePlayerScores()
{
    // Обновляем счёт для обоих игроков
    ui->labelPlayer1Score->setText("Очки: " + QString::number(m_player1Hand.getValue()));
    ui->labelPlayer2Score->setText("Очки: " + QString::number(m_player2Hand.getValue()));
}

void MultiplayerMainWindow::displayCards(const std::vector<Card>& cards, QLayout* layout)
{
    // Удаляем старые виджеты из layout
    if (!layout) return;

    // Удаляем все виджеты из layout перед добавлением новых
    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater(); // Удаляем старые карты
        }
        delete item;
    }

    // Устанавливаем размер карт
    const int cardWidth = 60;
    const int cardHeight = 90;

    // Добавляем новые QLabel с картинками в layout
    for (const auto& card : cards) {
        QLabel* lbl = new QLabel();
        QPixmap pix(card.getImagePath());

        // Проверка на корректность загрузки изображения
        if (pix.isNull()) {
            qDebug() << "Failed to load image for card: " << card.toString();
        }

        lbl->setPixmap(pix.scaled(cardWidth, cardHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lbl->setFixedSize(cardWidth, cardHeight);  // Устанавливаем фиксированный размер для карт
        layout->addWidget(lbl);
    }
}

void MultiplayerMainWindow::checkGameOver()
{
    if (m_player1Hand.getValue() > 21) {
        endGame("Игрок 2 выиграл!");
        startNewGame();
        return;
    }

    if (m_player2Hand.getValue() > 21) {
        endGame("Игрок 1 выиграл!");
        startNewGame();
        return;
    }

    if (m_player1Hand.getValue() == 21 && m_player2Hand.getValue() == 21) {
        endGame("Ничья! У обоих игроков 21!");
        startNewGame();
    }
}

void MultiplayerMainWindow::endGame(const QString &result) {
    QMessageBox msgBox(this);

    if (result == "Игрок 2 выиграл!") {
        QPixmap pixmap(":/game/images/resources/player2.png");
        pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        msgBox.setIconPixmap(pixmap);
    }
    else if (result == "Ничья! У обоих игроков 21!") {
        QPixmap pixmap(":/game/images/resources/draw.png");
        pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        msgBox.setIconPixmap(pixmap);
    }
    else {
        QPixmap pixmap(":/game/images/resources/player.png");
        pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        msgBox.setIconPixmap(pixmap);
    }

    msgBox.setWindowTitle("Игра окончена");

    msgBox.setText(result);

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
    ui->buttonHit->setEnabled(false);
    ui->buttonStand->setEnabled(false);
}

void MultiplayerMainWindow::on_buttonHit_clicked() {
    if (m_isPlayer1Turn) {
        // Игрок 1 берет карту
        m_player1Hand.addCard(m_desk.draw());
    } else {
        // Игрок 2 берет карту
        m_player2Hand.addCard(m_desk.draw());
    }

    // Обновляем интерфейс с картами
    updateUI();
    checkGameOver();
    switchTurns();
}

void MultiplayerMainWindow::on_buttonStand_clicked() {
    checkGameOver();
    switchTurns();
}

void MultiplayerMainWindow::switchTurns()
{
    // Переключаем ход между игроками
    m_isPlayer1Turn = !m_isPlayer1Turn;

    if (m_isPlayer1Turn) {
        ui->whooseTurn->setText("Текущий ход: игрок 1");
    } else {
        ui->whooseTurn->setText("Текущий ход: игрок 2");
    }

    updateUI();
}

void MultiplayerMainWindow::on_buttonNewGame_clicked() {
    startNewGame();
}

void MultiplayerMainWindow::on_actionExit_triggered()
{
    this->close();
}

void MultiplayerMainWindow::on_actionHit_triggered()
{
    if (m_isPlayer1Turn) {
        // Игрок 1 берет карту
        Card c = m_desk.draw();
        m_player1Hand.addCard(c);
    } else {
        // Игрок 2 берет карту
        Card c = m_desk.draw();
        m_player2Hand.addCard(c);
    }

    // Обновляем интерфейс с картами
    updateUI();
    checkGameOver();
    switchTurns();
}

void MultiplayerMainWindow::on_actionStand_triggered()
{
    switchTurns();
    checkGameOver();
}

void MultiplayerMainWindow::on_actionNewGame_triggered()
{
    startNewGame();
}

void MultiplayerMainWindow::on_actionRule1_triggered()
{
    QMessageBox msgBox(this);

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Правила игры против бота");

    msgBox.setText("1. В игре участвует игрок и бот.\n"
                   "2. Каждый получает по две карты, открытые для игрока и закрытые для бота.\n"
                   "3. Игрок может выбрать: взять карту (Hit) или пропустить ход (Stand).\n"
                   "4. Бот берет карты, пока не наберет определенное количество очков.\n"
                   "5. Побеждает тот, кто ближе к 21 очкам, но не переберет.\n"
                   "6. Если у игрока больше 21 очков, он проигрывает.\n");

    QPixmap pixmap(":/game/images/resources/rules.png");

    const int iconWidth = 64;
    const int iconHeight = 64;
    pixmap = pixmap.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    msgBox.setIconPixmap(pixmap);

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}

void MultiplayerMainWindow::on_actionRule2_triggered()
{
    QMessageBox msgBox(this);

    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle("Правила игры против другого игрока");

    msgBox.setText("1. В игре участвуют два игрока.\n"
                   "2. Каждый игрок получает по две карты, открытые для обоих игроков.\n"
                   "3. Игроки по очереди могут взять карту (Hit) или пропустить ход (Stand).\n"
                   "4. Побеждает тот, кто набрал больше очков, но не перебрал 21.\n"
                   "5. Если у одного из игроков больше 21 очков, он проигрывает.\n"
                   "6. Если у обоих игроков очки равны, это ничья.");

    QPixmap pixmap(":/game/images/resources/rules.png");

    const int iconWidth = 64;
    const int iconHeight = 64;
    pixmap = pixmap.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    msgBox.setIconPixmap(pixmap);

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}

void MultiplayerMainWindow::startBackgroundMusic()
{
    m_player->setSource(QUrl(m_playlist[m_currentTrackIndex]));

    m_player->play();
}

void MultiplayerMainWindow::playNextTrack()
{
    m_currentTrackIndex++;

    if (m_currentTrackIndex >= m_playlist.size()) {
        m_currentTrackIndex = 0;
    }

    m_player->setSource(QUrl(m_playlist[m_currentTrackIndex]));

    m_player->play();
}

void MultiplayerMainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        playNextTrack();
    }
}

void MultiplayerMainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Закрытие приложения",
                                  "Вы уверены, что хотите выйти?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        MenuWindow *menu = new MenuWindow();
        menu->show();
        this->close();
    } else {
        event->ignore();
    }
}

void MultiplayerMainWindow::on_actionMute_triggered()
{
    m_audioOutput->setVolume(0);
    ui->actionMute->setEnabled(false);
    ui->actionUnmute->setEnabled(true);
}

void MultiplayerMainWindow::on_actionUnmute_triggered()
{
    m_audioOutput->setVolume(50);
    ui->actionMute->setEnabled(true);
    ui->actionUnmute->setEnabled(false);
}
