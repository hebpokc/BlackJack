#include "desk.h"
#include <algorithm>
#include <chrono>

Desk::Desk() : m_index(0) {
    m_rng.seed(std::chrono::steady_clock::now().time_since_epoch().count());

    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            m_cards.emplace_back(static_cast<Suit>(s), r);
        }
    }
    shuffle();
}

void Desk::shuffle() {
    std::shuffle(m_cards.begin(), m_cards.end(), m_rng);
    m_index = 0;
}

Card Desk::draw() {
    if (m_index >= m_cards.size()) {
        shuffle();
    }
    return m_cards[m_index++];
}
