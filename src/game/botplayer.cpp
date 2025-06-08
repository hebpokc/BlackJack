#include "botplayer.h"

void BotPlayer::addCard(const Card &card) {
    m_hand.addCard(card);
}

int BotPlayer::getValue() const {
    return m_hand.getValue();
}

bool BotPlayer::shouldHit() const {
    return getValue() < 17;
}

std::vector<Card> BotPlayer::getCards() const {
    return m_hand.getCards();
}
