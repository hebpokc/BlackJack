// #include <gtest/gtest.h>
// #include "../src/game/hand.h"
// #include "../src/game/card.h"
// #include "../src/gui/mainwindow.h"

// TEST(BlackjackTest, TestPlayerScore) {
//     Card card1(10);
//     Card card2(5);
//     Card card3(1);

//     // Добавляем карты в руку игрока
//     Hand playerHand;
//     playerHand.addCard(card1);
//     playerHand.addCard(card2);
//     playerHand.addCard(card3);

//     // Проверяем, что сумма карт правильная
//     EXPECT_EQ(playerHand.getValue(), 16);  // 10 + 5 + 1 = 16

//     // Добавляем второй туз
//     Card card4(1);
//     playerHand.addCard(card4);

//     // Теперь сумма должна быть 17
//     EXPECT_EQ(playerHand.getValue(), 17);  // 10 + 5 + 1 + 1 (11) = 17
// }

// TEST(BlackjackTest, TestPlaceBet) {
//     // Создаем объект MainWindow с балансом 100 монет
//     MainWindow window;
//     window.setBalance(100);

//     // Пробуем сделать ставку в 50 монет
//     bool result = window.placeBet(50);

//     // Проверяем, что ставка прошла успешно, и баланс уменьшился на 50 монет
//     EXPECT_TRUE(result);
//     EXPECT_EQ(window.getBalance(), 50);
// }

// TEST(BlackjackTest, TestLeaderboardSaveAndLoad) {
//     MainWindow window;

//     // Создаем несколько игроков
//     Player player1{"TestPlayer1", 100};
//     Player player2{"TestPlayer2", 200};

//     // Сохраняем их в таблице лидеров
//     window.saveLeaderboard({player1, player2});

//     // Загружаем таблицу лидеров
//     QList<Player> leaderboard = window.loadLeaderboard();

//     // Проверяем, что таблица лидеров содержит правильные данные
//     EXPECT_EQ(leaderboard.size(), 2);
//     EXPECT_EQ(leaderboard[0].name, "TestPlayer2");
//     EXPECT_EQ(leaderboard[1].name, "TestPlayer1");
// }

// TEST(BlackjackTest, TestPlayerWin) {
//     // Создаем объект MainWindow с балансом 100 монет
//     MainWindow window;
//     window.setBalance(100);

//     // Игрок ставит 50 монет и выигрывает
//     window.placeBet(50);
//     window.setPlayerScore(21);

//     // Проверяем, что баланс увеличился на 1.5x от ставки (50 * 1.5 = 75)
//     window.updateBalanceAfterWin();
//     EXPECT_EQ(window.getBalance(), 150);  // Баланс должен быть 150 монет
// }
