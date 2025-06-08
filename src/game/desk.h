#ifndef DESK_H
#define DESK_H
#pragma once
#include <vector>
#include <random>
#include "card.h"

class Desk
{
public:
    Desk();
    void shuffle();
    Card draw();

private:
    std::vector<Card> m_cards;
    std::mt19937 m_rng;
    size_t m_index;
};

#endif // DESK_H
