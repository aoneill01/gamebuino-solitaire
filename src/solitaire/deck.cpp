#include "deck.h"

Deck::Deck() {
  _count = 0;
}

void Deck::addCard(Card card) {
  if (_count < DECK_SIZE) _cards[_count++] = card;
}

void Deck::shuffle() {
  for (int i = 0; i < _count; i++) {
    int randomIndex = random(_count - i);
    Card tmp = _cards[randomIndex];
    _cards[randomIndex] = _cards[_count - i - 1];
    _cards[_count - i - 1] = tmp;
  }
}

void Deck::newDeck() {
  _count = 0;
  for (int suit = spade; suit <= diamond; suit++) {
    for (int value = ace; value <= king; value++) {
      addCard(Card(static_cast<Value>(value), static_cast<Suit>(suit), true));
    }
  }
}

byte Deck::getCardCount() const {
  return _count;
}

Card Deck::removeTopCard() {
  if (_count > 0) return _cards[--_count];
  return Card();
}

Card Deck::peekCard(int indexFromTop) const {
  if (indexFromTop < _count) {
    return _cards[_count - indexFromTop - 1];
  }
  return Card();
}

