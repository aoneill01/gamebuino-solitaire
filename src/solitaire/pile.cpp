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

void Pile::removeCards(int count, Pile* destination) {
  count = min(count, _count);
  _count -= count;
  for (int i = 0; i < count; i++) destination->addCard(_cards[_count + i]);
}

void Pile::empty() {
  _count = 0;
}

