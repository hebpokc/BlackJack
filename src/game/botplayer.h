#ifndef BOTPLAYER_H
#define BOTPLAYER_H
#pragma once
#include "hand.h"

class BotPlayer
{
public:
    BotPlayer() = default;

    void addCard(const Card& card);
    int getValue() const;
    bool shouldHit() const;

    std::vector<Card> getCards() const;

private:
    Hand m_hand;
};

#endif // BOTPLAYER_H
