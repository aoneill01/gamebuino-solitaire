#include "pile.h"
 
Pile::Pile() {
  _count = 0;
}

void Pile::addCard(Card card) {
  if (_count < MAX_PILE) _cards[_count++] = card;
}

byte Pile::getCardCount() const {
  return _count;
}

Card Pile::getCard(int indexFromTop) const {
  if (indexFromTop < _count) {
    return _cards[_count - indexFromTop - 1];
  }
  return Card(two, heart, false);
}

Pile Pile::removeCards(int count) {
  count = min(count, _count);
  _count -= count;
  Pile result;
  for (int i = 0; i < count; i++) result.addCard(_cards[_count + i]);
  return result;
}

