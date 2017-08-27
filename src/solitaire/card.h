#include <Gamebuino.h>

enum Suit { spade = 0, club, heart, diamond };
enum Value { ace = 1, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king };

class Card {
  public:
    Card() : Card(ace, spade, false) { }
    Card(Value value, Suit suit, bool faceDown);
    bool isFaceDown() const;
    Value getValue() const;
    Suit getSuit() const;
    bool isRed() const;
    void flip();
    
  private:
    byte _value;
};

