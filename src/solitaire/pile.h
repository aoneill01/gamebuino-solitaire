#include <Gamebuino.h>
#include "card.h"

#define MAX_PILE 20

class Pile {
  public:
    Pile();
    void addCard(Card card);
    byte getCardCount() const;
    Card getCard(int indexFromTop) const;
    Pile removeCards(int count);
    byte x, y;
    
  private:
    Card _cards[MAX_PILE];
    byte _count;
};

