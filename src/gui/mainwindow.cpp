#include "mainwindow.h"
#include "menuwindow.h"
#include "clickerwindow.h"
#include "../game/player.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <QCloseEvent>
#include <QList>
#include <QFile>
#include <QInputDialog>

const QString LEADERBOARD_FILE = "leaderboard.txt";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_playerTurn(true)
    , m_currentTrackIndex(0)
    , m_balance(100)  // Изначальный баланс 100 монет
    , m_bet(0)
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

    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);

    startBackgroundMusic();

    ui->labelBalance->setText("Баланс: \n" + QString::number(m_balance) + " монет 🪙");

    startNewGame();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_player;
    delete m_audioOutput;
}

void MainWindow::startNewGame() {
    m_desk.shuffle();
    m_playerHand = Hand();
    m_bot = BotPlayer();
    m_playerTurn = true;

    // Раздаем по 2 карты игроку и боту
    m_playerHand.addCard(m_desk.draw());
    m_playerHand.addCard(m_desk.draw());

    m_bot.addCard(m_desk.draw());
    m_bot.addCard(m_desk.draw());

    updateUI();

    ui->buttonHit->setEnabled(true);
    ui->buttonStand->setEnabled(true);

    m_bet = 0;

    ui->buttonPlaceBet->setEnabled(true);
}

void MainWindow::updateUI() {
    // Получаем QHBoxLayout для отображения карт
    QLayout* playerLayout = ui->widgetPlayerCards->layout();
    QLayout* botLayout = ui->widgetBotCards->layout();

    // Отображаем карты
    displayCards(m_playerHand.getCards(), playerLayout);
    ui->labelPlayerScore->setText("Очки: " + QString::number(m_playerHand.getValue()));

    displayCards(m_bot.getCards(), botLayout);
    ui->labelBotScore->setText("Очки: ??");  // Пока не показываем очки
}

void MainWindow::displayCards(const std::vector<Card>& cards, QLayout* layout)
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

void MainWindow::displayBotCards(const std::vector<Card>& cards, QLayout* layout, bool revealAll)
{
    // Удаляем старые виджеты из layout
    if (!layout) return;

    while (QLayoutItem* item = layout->takeAt(0)) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater(); // Удаляем старые карты
        }
        delete item;
    }

    // Добавляем карты бота в layout
    const int cardWidth = 60;
    const int cardHeight = 90;

    for (size_t i = 0; i < cards.size(); ++i) {
        QLabel* lbl = new QLabel();
        QPixmap pix;

        if (i == 0 || revealAll) {
            // Показываем карту, если это первая карта или если revealAll=true
            pix.load(cards[i].getImagePath());
        } else {
            pix.load(":/cards/back.png");
        }

        lbl->setPixmap(pix.scaled(cardWidth, cardHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lbl->setFixedSize(cardWidth, cardHeight);
        layout->addWidget(lbl);
    }
}


void MainWindow::on_buttonHit_clicked() {
    if (!m_playerTurn) return;

    m_playerHand.addCard(m_desk.draw());
    updateUI();

    if (m_playerHand.getValue() > 21) {
        endGame("Перебор! Вы проиграли");
    }
}

void MainWindow::on_buttonStand_clicked() {
    if (!m_playerTurn) return;

    m_playerTurn = false;
    ui->buttonHit->setEnabled(false);
    ui->buttonStand->setEnabled(false);

    botTurn();
}

void MainWindow::on_buttonNewGame_clicked() {
    startNewGame();
}

void MainWindow::on_buttonPlaceBet_clicked()
{
    bool ok;
    int bet = ui->lineEditBet->text().toInt(&ok);

    if (!ok || bet <= 0 || bet > m_balance) {
        QMessageBox::warning(this, "Ошибка", "Ставка некорректна или превышает ваш баланс.");
        return;
    }

    m_bet = bet;

    QMessageBox::information(this, "Ставка принята", "Ваша ставка: " + QString::number(m_bet) + " монет 🪙");

    m_balance -= m_bet;
    ui->labelBalance->setText("Баланс: \n" + QString::number(m_balance) + " монет 🪙");

    ui->buttonPlaceBet->setEnabled(false);
}

void MainWindow::botTurn() {
    // Пока бот ходит, показываем только одну карту
    displayBotCards(m_bot.getCards(), ui->widgetBotCards->layout(), false); // Показываем только первую карту
    ui->labelBotScore->setText("Очки: ??");

    // Ход бота — бот берет карты, пока у него меньше 17 очков
    while (m_bot.shouldHit()) {
        Card c = m_desk.draw();
        m_bot.addCard(c);

        // Обновляем отображение карт
        displayBotCards(m_bot.getCards(), ui->widgetBotCards->layout(), false);
        ui->labelBotScore->setText("Очки: ??");
    }

    // После того как бот завершил ход, раскрываем все карты
    displayBotCards(m_bot.getCards(), ui->widgetBotCards->layout(), true); // Показываем все карты
    ui->labelBotScore->setText("Очки: " + QString::number(m_bot.getValue()));

    int playerScore = m_playerHand.getValue();
    int botScore = m_bot.getValue();

    if (botScore > 21 || playerScore > botScore) {
        endGame("Вы выиграли!");
        if (playerScore != 21) {
            m_balance += m_bet * 1.5;

        }
        else {
            m_balance += m_bet * 2;
        }
        ui->labelBalance->setText("Баланс: \n" + QString::number(m_balance) + " монет 🪙");
    } else if (playerScore == botScore) {
        endGame("Ничья!");
        m_balance += m_bet;
        ui->labelBalance->setText("Баланс: \n" + QString::number(m_balance) + " монет 🪙");
    }
    else {
        endGame("Вы проиграли!");
    }
}

void MainWindow::endGame(const QString &result) {
    QMessageBox msgBox(this);

    if (result == "Вы выиграли!") {
        QPixmap pixmap(":/game/images/resources/win.png");
        pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        msgBox.setIconPixmap(pixmap);
    }
    else if (result == "Ничья!") {
        QPixmap pixmap(":/game/images/resources/draw.png");
        pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        msgBox.setIconPixmap(pixmap);
    }
    else {
        QPixmap pixmap(":/game/images/resources/lose.png");
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

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionHit_triggered()
{
    if (!m_playerTurn) return;

    m_playerHand.addCard(m_desk.draw());
    updateUI();

    if (m_playerHand.getValue() > 21) {
        endGame("Перебор! Вы проиграли");
    }
}

void MainWindow::on_actionStand_triggered()
{
    if (!m_playerTurn) return;

    m_playerTurn = false;
    ui->buttonHit->setEnabled(false);
    ui->buttonStand->setEnabled(false);

    botTurn();
}

void MainWindow::on_actionNewGame_triggered()
{
    startNewGame();
}

void MainWindow::on_actionRule1_triggered()
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

void MainWindow::on_actionRule2_triggered()
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

void MainWindow::startBackgroundMusic()
{
    m_player->setSource(QUrl(m_playlist[m_currentTrackIndex]));

    m_player->play();
}

void MainWindow::playNextTrack()
{
    m_currentTrackIndex++;

    if (m_currentTrackIndex >= m_playlist.size()) {
        m_currentTrackIndex = 0;
    }

    m_player->setSource(QUrl(m_playlist[m_currentTrackIndex]));

    m_player->play();
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        playNextTrack();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
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

void MainWindow::on_actionMute_triggered()
{
    m_audioOutput->setVolume(0);
    ui->actionMute->setEnabled(false);
    ui->actionUnmute->setEnabled(true);
}

void MainWindow::on_actionUnmute_triggered()
{
    m_audioOutput->setVolume(50);
    ui->actionMute->setEnabled(true);
    ui->actionUnmute->setEnabled(false);
}

QList<Player> MainWindow::readLeaderboard()
{
    QList<Player> leaderboard;

    QFile file(LEADERBOARD_FILE);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(" | ");
            if (parts.size() == 2) {
                Player player;
                player.name = parts[0];
                player.balance = parts[1].toInt();
                leaderboard.append(player);
            }
        }
        file.close();
    }
    return leaderboard;
}

void MainWindow::writeLeaderboard(const QList<Player>& leaderboard)
{
    QFile file(LEADERBOARD_FILE);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const Player& player : leaderboard) {
            out << player.name << " | " << player.balance << "\n";
        }
        file.close();
    }
}

void MainWindow::on_actionSaveResult_triggered()
{
    // Запрашиваем имя пользователя через MessageBox
    bool ok;
    QString name = QInputDialog::getText(this, "Введите ник", "Введите ваш ник:", QLineEdit::Normal, "", &ok);

    if (ok && !name.isEmpty()) {
        // Сохраняем результат в таблицу лидеров
        Player newPlayer;
        newPlayer.name = name;
        newPlayer.balance = m_balance;

        // Чтение текущей таблицы лидеров
        QList<Player> leaderboard = readLeaderboard();

        // Добавляем нового игрока в таблицу
        leaderboard.append(newPlayer);

        // Сортируем таблицу по балансу
        std::sort(leaderboard.begin(), leaderboard.end());

        // Ограничиваем таблицу 10 лучшими игроками
        if (leaderboard.size() > 10) {
            leaderboard = leaderboard.mid(0, 10);
        }

        // Записываем обновленную таблицу в файл
        writeLeaderboard(leaderboard);

        QMessageBox::information(this, "Результат сохранен", "Ваш результат был успешно сохранён.");
    }
}

void MainWindow::on_actionShowLeaderboard_triggered()
{
    // Чтение таблицы лидеров
    QList<Player> leaderboard = readLeaderboard();

    // Формируем текст для отображения в MessageBox
    QString leaderboardText;

    if (leaderboard.isEmpty()) {
        leaderboardText = "Таблица лидеров пуста";
    } else {
        for (int i = 0; i < leaderboard.size(); ++i) {
            leaderboardText += QString::number(i + 1) + ". " + leaderboard[i].name + " | " + QString::number(leaderboard[i].balance) + " монет 🪙\n";
        }
    }

    // Показываем таблицу лидеров
    QMessageBox msgBox(this);

    QPixmap pixmap(":/game/images/resources/leaderboard.png");
    const int iconWidth = 64;
    const int iconHeight = 64;
    pixmap = pixmap.scaled(iconWidth, iconHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    msgBox.setIconPixmap(pixmap);

    msgBox.setWindowTitle("Таблица лидеров");
    msgBox.setText(leaderboardText);

    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    msgBox.exec();
}

void MainWindow::on_actionClicker_triggered()
{
    ClickerWindow *clickerWindow = new ClickerWindow(this);
    clickerWindow->show();
}

void MainWindow::updateBalance(int reward)
{
    m_balance += reward;
    ui->labelBalance->setText("Баланс: \n" + QString::number(m_balance) + " монет 🪙");
}
