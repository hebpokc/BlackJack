#ifndef CARD_H
#define CARD_H
#pragma once
#include <QString>

enum class Suit { Hearts, Diamonds, Clubs, Spades};

class Card
{
public:
    Card(Suit suit, int rank);

    int getValue() const; // Очки карт
    QString toString() const;
    QString getImagePath() const;

    int rank() const { return m_rank; }
    Suit suit() const { return m_suit; }

private:
    Suit m_suit;
    int m_rank; // 1 - Туз, 11 - валет, 12 - дама, 12 - король
};

#endif // CARD_H
