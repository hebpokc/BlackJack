#ifndef HAND_H
#define HAND_H
#pragma once
#include <vector>
#include "card.h"

class Hand
{
public:
    void addCard(const Card& card);
    int getValue() const;
    std::vector<Card> getCards() const;

private:
    std::vector<Card> m_cards;
};

#endif // HAND_H
