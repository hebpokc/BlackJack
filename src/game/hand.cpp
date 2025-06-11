#include "hand.h"

void Hand::addCard(const Card &card) {
    m_cards.push_back(card);
}

std::vector<Card> Hand::getCards() const {
    return m_cards;
}

int Hand::getValue() const {
    int total = 0;
    int aceCount = 0;

    for (const auto& card : m_cards) {
        int val = card.getValue();
        total += val;

        if (card.rank() == 1)
            aceCount++;
    }
    //Попытка считать туз за 11 если это не приведет к перебору
    while (aceCount > 0 && total + 10 <= 21) {
        total += 10;
        aceCount--;
    }
    return total;
}

void Hand::clear() {
    m_cards.clear();  // Очищаем все карты в руке
}
