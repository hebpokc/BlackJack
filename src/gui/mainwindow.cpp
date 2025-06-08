#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_playerTurn(true)
{
    ui->setupUi(this);

    QPixmap bkgnd(":/game/images/resources/game_background.jpg");
    bkgnd = bkgnd.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

    QPalette palette;
    palette.setBrush(QPalette::Window, QBrush(bkgnd));
    this->setPalette(palette);

    startNewGame();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    const int cardWidth = 60;  // ширина карты
    const int cardHeight = 90; // высота карты

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
            // Для остальных карт показываем рубашку карты
            pix.load(":/cards/back.png"); // Убедись, что картинка рубашки существует в ресурсах
        }

        lbl->setPixmap(pix.scaled(cardWidth, cardHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        lbl->setFixedSize(cardWidth, cardHeight);  // Устанавливаем фиксированный размер для карт
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
    } else if (playerScore == botScore) {
        endGame("Ничья!");
    } else {
        endGame("Вы проиграли!");
    }
}

void MainWindow::on_buttonNewGame_clicked() {
    startNewGame();
}

void MainWindow::endGame(const QString &result) {
    QMessageBox::information(this, "Игра окончена", result);
    ui->buttonHit->setEnabled(false);
    ui->buttonStand->setEnabled(false);
}
