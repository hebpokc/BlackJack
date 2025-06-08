#include "card.h"

Card::Card(Suit suit, int rank)
    : m_suit(suit), m_rank(rank){}

int Card::getValue() const {
    if (m_rank > 10) return 10;
    return m_rank;
}

QString Card::toString() const {
    static const char* ranks[] = { "", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    static const char* suits[] = { "♥", "♦", "♣", "♠" };

    return QString("%1%2").arg(ranks[m_rank]).arg(suits[static_cast<int>(m_suit)]);
}

QString Card::getImagePath() const
{
    // Формируем строку вида ":/cards/AS.png"
    // Сопоставляем масть к букве
    char suitChar;
    switch (m_suit) {
    case Suit::Spades:   suitChar = 'S'; break;
    case Suit::Hearts:   suitChar = 'H'; break;
    case Suit::Diamonds: suitChar = 'D'; break;
    case Suit::Clubs:    suitChar = 'C'; break;
    }

    QString rankStr;
    switch (m_rank) {
    case 1:  rankStr = "A"; break;
    case 11: rankStr = "J"; break;
    case 12: rankStr = "Q"; break;
    case 13: rankStr = "K"; break;
    default: rankStr = QString::number(m_rank); break;
    }

    return QString(":/game/cards/resources/cards/%1%2.png").arg(rankStr).arg(suitChar);
}

