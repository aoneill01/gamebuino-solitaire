#include <Gamebuino.h>
#include "card.h"

#define MAX_PILE 52

class Pile {
  public:
    Pile();
    void addCard(Card card);
    byte getCardCount() const;
    Card getCard(int indexFromTop) const;
    void removeCards(int count, Pile* destination);
    void empty();
    byte x, y;
    
  private:
    Card _cards[MAX_PILE];
    byte _count;
};

