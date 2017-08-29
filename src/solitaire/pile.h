#include <Gamebuino.h>
#include "card.h"

#define MAX_PILE 52

class Pile {
  public:
    Pile();
    void addCard(Card card);
    byte getCardCount() const;
    Card getCard(int indexFromTop) const;
    Card removeTopCard();
    void removeCards(int count, Pile* destination);
    void empty();
    void shuffle();
    void newDeck();
    byte x, y;
    
  private:
    Card _cards[MAX_PILE];
    byte _count;
};

