#include <Gamebuino.h>
#include "card.h"

#define DECK_SIZE 52

class Deck {
  public:
    Deck();
    void addCard(Card card);
    void shuffle();
    void newDeck();
    byte getCardCount() const;
    Card removeTopCard();
    Card peekCard(int indexFromTop) const;
    
  private:
    Card _cards[DECK_SIZE];
    byte _count;
};

