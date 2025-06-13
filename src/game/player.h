#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

struct Player {
    QString name;
    int balance;

    // Для удобства сравнения игроков по балансу
    bool operator<(const Player& other) const {
        return balance > other.balance;
    }
};

#endif // PLAYER_H
