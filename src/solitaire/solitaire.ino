#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>
#include "deck.h"
#include "pile.h"

Gamebuino gb;

enum GameMode { dealing, selecting, drawingCards, movingPile };
GameMode mode = selecting;

enum Location { stock, talon, 
  foundation1, foundation2, foundation3, foundation4,
  tableau1, tableau2, tableau3, tableau4, tableau5, tableau6, tableau7 }; 
// Stack that the cursor is currently pointed at.
Location activeLocation = stock;
// Position of the cursor for animation.
byte cursorX = 11;
byte cursorY = 5;

// Animating moving stack of cards.
Pile moving;
Location returnLocation;
byte remainingDraws;

Deck stockDeck = Deck();
Deck talonDeck = Deck();
Pile foundations[4];
Pile tableau[7];

struct CardAnimation {
  Card card;
  byte tableauIndex;
  byte x, y, destX, destY;
};

CardAnimation cardAnimations[28];
byte cardAnimationCount = 0;

void setup() {
  gb.begin();
  showTitle();
}

void loop() {
  if (gb.update()) {
    if (gb.buttons.pressed(BTN_C)) showTitle();
    
    switch (mode) {
      case selecting: handleSelectingButtons(); break;
      case movingPile: handleMovingPileButtons(); break;
    }
    
    drawBoard();

    switch(mode) {
      case dealing: drawDealing(); break;
      case selecting: drawCursor(); break;
      case drawingCards: drawDrawingCards(); break;
      case movingPile: drawMovingPile(); break;
    }
  }
}

void showTitle() {
  gb.titleScreen(F("Solitaire"));
  gb.pickRandomSeed();
  gb.battery.show = false;
  activeLocation = 0;
  cursorX = 11;
  cursorY = 5;
  setupNewGame();
  mode = dealing;
}

void setupNewGame() {
  talonDeck = Deck();
  stockDeck.newDeck();
  stockDeck.shuffle();
  for (int i = 0; i < 4; i++) {
    foundations[i] = Pile();
    foundations[i].x = 37 + i * 12;
    foundations[i].y = 1;
  }
  for (int i = 0; i < 7; i++) {
    tableau[i] = Pile();
    tableau[i].x = i * 12 + 1;
    tableau[i].y = 17;
  }
  cardAnimationCount = 0;
  for (int i = 0; i < 7; i++) {
    for (int j = i; j < 7; j++) {
      Card card = stockDeck.removeTopCard();
      if (i == j) card.flip();
      cardAnimations[cardAnimationCount] = CardAnimation();
      cardAnimations[cardAnimationCount].x = 1;
      cardAnimations[cardAnimationCount].y = 1;
      cardAnimations[cardAnimationCount].destX = tableau[j].x;
      cardAnimations[cardAnimationCount].destY = tableau[j].y + 2 * i;
      cardAnimations[cardAnimationCount].tableauIndex = j;
      cardAnimations[cardAnimationCount].card = card;
      cardAnimationCount++;
    }
  }
  cardAnimationCount = 0;
}

void handleSelectingButtons() {
  if (gb.buttons.pressed(BTN_RIGHT)) {
    if (activeLocation != foundation4 && activeLocation != tableau7) {
      activeLocation = activeLocation + 1;
    }
  }
  if (gb.buttons.pressed(BTN_LEFT)) {
    if (activeLocation != stock && activeLocation != tableau1) {
      activeLocation = activeLocation - 1;
    }
  }
  if (gb.buttons.pressed(BTN_DOWN)) {
    if (activeLocation < foundation1) activeLocation = activeLocation + 6;
    else if (activeLocation <= foundation4) activeLocation = activeLocation + 7;
  }
  if (gb.buttons.pressed(BTN_UP)) {
    if (activeLocation > tableau2) activeLocation = activeLocation - 7;
    else if (activeLocation >= tableau1) activeLocation = activeLocation - 6;
  }
  if (gb.buttons.pressed(BTN_A)) {
    switch (activeLocation) {
      case stock:
        if (stockDeck.getCardCount() != 0) {
          moving = Pile();
          Card card = stockDeck.removeTopCard();
          card.flip();
          moving.addCard(card);
          moving.x = 1;
          moving.y = 1;
          remainingDraws = min(2, stockDeck.getCardCount());
          mode = drawingCards;
        }
        break;
      case tableau1:
      case tableau2:
      case tableau3:
      case tableau4:
      case tableau5:
      case tableau6:
      case tableau7:
        moving.empty();
        tableau[activeLocation - tableau1].removeCards(1, &moving);
        moving.x = tableau[activeLocation - tableau1].x;
        moving.y = tableau[activeLocation - tableau1].y + 2 * tableau[activeLocation - tableau1].getCardCount() + 2;
        mode = movingPile;
        break;
    }
  }
}

void handleMovingPileButtons() {
  if (gb.buttons.pressed(BTN_RIGHT)) {
    if (activeLocation != foundation4 && activeLocation != tableau7) {
      activeLocation = activeLocation + 1;
    }
  }
  if (gb.buttons.pressed(BTN_LEFT)) {
    if (activeLocation != stock && activeLocation != tableau1) {
      activeLocation = activeLocation - 1;
    }
  }
  if (gb.buttons.pressed(BTN_DOWN)) {
    if (activeLocation < foundation1) activeLocation = activeLocation + 6;
    else if (activeLocation <= foundation4) activeLocation = activeLocation + 7;
  }
  if (gb.buttons.pressed(BTN_UP)) {
    if (activeLocation > tableau2) activeLocation = activeLocation - 7;
    else if (activeLocation >= tableau1) activeLocation = activeLocation - 6;
  }
  if (gb.buttons.pressed(BTN_A)) {
    switch (activeLocation) {
      case tableau1:
      case tableau2:
      case tableau3:
      case tableau4:
      case tableau5:
      case tableau6:
      case tableau7:
        tableau[activeLocation - tableau1].addCard(moving.getCard(0));
        mode = selecting;
        break;
    }
  }
}

void drawBoard() {
  // Stock
  if (stockDeck.getCardCount() != 0) {
    drawCard(1, 1, Card(ace, spade, true));
  }
  
  // Talon
  for (int i = 0; i < min(3, talonDeck.getCardCount()); i++) {
    drawCard(13 + i * 2, 1, talonDeck.peekCard(min(3, talonDeck.getCardCount()) - i - 1));
  }
  
  // Foundations
  for (int i = 0; i < 4; i++) {
    if (foundations[i].getCardCount() != 0) {
      drawCard(foundations[i].x, foundations[i].y, foundations[i].getCard(0));
    }
    else {
      gb.display.setColor(GRAY);
      gb.display.drawRect(foundations[i].x, foundations[i].y, 10, 14);
    }
  }
  
  // Tableau
  for (int i = 0; i < 7; i++) {
    drawPile(&tableau[i]);
  }
}

void drawPile(Pile* pile) {
  for (int i = 0; i < pile->getCardCount(); i++) {
    drawCard(pile->x, pile->y + 2 * i, pile->getCard(pile->getCardCount() - i - 1));
  }
}

void drawCard(byte x, byte y, Card card) {
  // Fill 
  byte fill = WHITE;
  if (card.isFaceDown()) fill = GRAY;
  gb.display.setColor(fill);
  gb.display.fillRect(x + 1, y + 1, 8, 12);
  
  // Draw border
  gb.display.setColor(BLACK);
  gb.display.drawFastHLine(x + 1, y, 8);
  gb.display.drawFastHLine(x + 1, y + 13, 8);
  gb.display.drawFastVLine(x, y + 1, 12);
  gb.display.drawFastVLine(x + 9, y + 1, 12);

  if (card.isFaceDown()) return;

  if (card.isRed()) gb.display.setColor(GRAY);
  drawSuit(x + 2, y + 2, card.getSuit());
  drawValue(x + 5, y + 7, card.getValue());
}

void drawSuit(byte x, byte y, Suit suit) {
  switch (suit) {
    case spade: drawSpade(x, y); break;
    case club: drawClub(x, y); break;
    case heart: drawHeart(x, y); break;
    case diamond: drawDiamond(x, y); break;
  }
}

void drawHeart(byte x, byte y) {
  gb.display.drawPixel(x + 1, y);
  gb.display.drawPixel(x + 3, y);
  gb.display.drawFastHLine(x, y + 1, 5);
  gb.display.drawFastHLine(x, y + 2, 5);
  gb.display.drawFastHLine(x + 1, y + 3, 3);
  gb.display.drawPixel(x + 2, y + 4);
}

void drawDiamond(byte x, byte y) {
  gb.display.drawPixel(x + 2, y);
  gb.display.drawFastHLine(x + 1, y + 1, 3);
  gb.display.drawFastHLine(x, y + 2, 5);
  gb.display.drawFastHLine(x + 1, y + 3, 3);
  gb.display.drawPixel(x + 2, y + 4);
}

void drawSpade(byte x, byte y) {
  gb.display.drawPixel(x + 2, y);
  gb.display.drawFastHLine(x + 1, y + 1, 3);
  gb.display.drawFastHLine(x, y + 2, 5);
  gb.display.drawFastHLine(x, y + 3, 5);
  gb.display.drawPixel(x + 2, y + 4);
}

void drawClub(byte x, byte y) {
  gb.display.drawFastHLine(x + 1, y, 3);
  gb.display.drawFastHLine(x + 1, y + 2, 3);
  gb.display.drawFastVLine(x, y + 1, 3);
  gb.display.drawFastVLine(x + 4, y + 1, 3);
  gb.display.drawFastVLine(x + 2, y + 1, 4);
}

void drawValue(byte x, byte y, Value value) {
  switch (value) {
    case ace: drawAce(x, y); break;
    case two: drawTwo(x, y); break;
    case three: drawThree(x, y); break;
    case four: drawFour(x, y); break;
    case five: drawFive(x, y); break;
    case six: drawSix(x, y); break;
    case seven: drawSeven(x, y); break;
    case eight: drawEight(x, y); break;
    case nine: drawNine(x, y); break;
    case ten: drawTen(x, y); break;
    case jack: drawJack(x, y); break;
    case queen: drawQueen(x, y); break;
    case king: drawKing(x, y); break;
  }
}

void drawAce(byte x, byte y) {
  gb.display.drawPixel(x + 1, y);
  gb.display.drawFastVLine(x, y + 1, 4);
  gb.display.drawFastVLine(x + 2, y + 1, 4);
  gb.display.drawPixel(x + 1, y + 2);
}

void drawTwo(byte x, byte y) {
  drawSegmentA(x, y);
  drawSegmentB(x, y);
  drawSegmentG(x, y);
  drawSegmentE(x, y);
  drawSegmentD(x, y);
}

void drawThree(byte x, byte y) {
  drawSegmentA(x, y);
  drawSegmentB(x, y);
  drawSegmentG(x, y);
  drawSegmentC(x, y);
  drawSegmentD(x, y);
}

void drawFour(byte x, byte y) {
  drawSegmentF(x, y);
  drawSegmentG(x, y);
  drawSegmentB(x, y);
  drawSegmentC(x, y);
}

void drawFive(byte x, byte y) {
  drawSegmentA(x, y);
  drawSegmentF(x, y);
  drawSegmentG(x, y);
  drawSegmentC(x, y);
  drawSegmentD(x, y);
}

void drawSix(byte x, byte y) {
  drawFive(x, y);
  drawSegmentE(x, y);
}

void drawSeven(byte x, byte y) {
  drawSegmentA(x, y);
  drawSegmentB(x, y);
  drawSegmentC(x, y);
}

void drawEight(byte x, byte y) {
  drawNine(x, y);
  drawSegmentE(x, y);
}

void drawNine(byte x, byte y) {
  drawFive(x, y);
  drawSegmentB(x, y);
}

void drawTen(byte x, byte y) {
  drawSeven(x, y);
  drawSegmentD(x, y);
  drawSegmentE(x, y);
  drawSegmentF(x, y);
  gb.display.drawFastVLine(x - 2, y, 5);
}

void drawJack(byte x, byte y) {
  drawSegmentB(x, y);
  drawSegmentC(x, y);
  drawSegmentD(x, y);
  gb.display.drawPixel(x, y + 3);
}

void drawQueen(byte x, byte y) {
  drawSegmentA(x, y);
  drawSegmentB(x, y);
  drawSegmentF(x, y);
  gb.display.drawFastHLine(x, y + 3, 3);
  gb.display.drawPixel(x + 1, y + 4);
}

void drawKing(byte x, byte y) {
  drawSegmentF(x, y);
  drawSegmentE(x, y);
  gb.display.drawPixel(x + 1, y + 2);
  gb.display.drawFastVLine(x + 2, y, 2);
  gb.display.drawFastVLine(x + 2, y + 3, 2);
}

void drawCursor() {
  bool flipped = false;
  switch (activeLocation) {
    case stock:
      cursorX = updatePosition(cursorX, 11);
      cursorY = updatePosition(cursorY, 5);
      break;
    case talon:
      cursorX = updatePosition(cursorX, 27);
      cursorY = updatePosition(cursorY, 5);
      break;
    case foundation1:
      cursorX = updatePosition(cursorX, 30);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case foundation2:
      cursorX = updatePosition(cursorX, 42);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case foundation3:
      cursorX = updatePosition(cursorX, 54);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case foundation4:
      cursorX = updatePosition(cursorX, 66);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case tableau1:
    case tableau2:
    case tableau3:
      cursorX = updatePosition(cursorX, tableau[activeLocation - tableau1].x + 10);
      cursorY = updatePosition(cursorY, tableau[activeLocation - tableau1].y + 4 + 2 * (tableau[activeLocation - tableau1].getCardCount() - 1));
      break;
    case tableau4:
    case tableau5:
    case tableau6:
    case tableau7:
      cursorX = updatePosition(cursorX, tableau[activeLocation - tableau1].x - 7);
      cursorY = updatePosition(cursorY, tableau[activeLocation - tableau1].y + 4 + 2 * (tableau[activeLocation - tableau1].getCardCount() - 1));
      flipped = true;
      break;
  }
  drawCursor(cursorX, cursorY, flipped);
}

void drawDealing() {
  if (cardAnimationCount < 28 && gb.frameCount % 4 == 0) cardAnimationCount++;
  bool doneDealing = cardAnimationCount == 28;
  for (int i = 0; i < cardAnimationCount; i++) {
    if (cardAnimations[i].x != cardAnimations[i].destX || cardAnimations[i].y != cardAnimations[i].destY) {
      doneDealing = false;
      drawCard(cardAnimations[i].x, cardAnimations[i].y, cardAnimations[i].card);
      cardAnimations[i].x = updatePosition(cardAnimations[i].x, cardAnimations[i].destX);
      cardAnimations[i].y = updatePosition(cardAnimations[i].y, cardAnimations[i].destY);
      if (cardAnimations[i].x == cardAnimations[i].destX && cardAnimations[i].y == cardAnimations[i].destY) {
        tableau[cardAnimations[i].tableauIndex].addCard(cardAnimations[i].card);
      }
    }
  }
  if (doneDealing) mode = selecting;
}

void drawDrawingCards() {
  drawPile(&moving);
  moving.x = updatePosition(moving.x, 17);
  moving.y = updatePosition(moving.y, 1);
  if (moving.x == 17 && moving.y == 1) {
    talonDeck.addCard(moving.getCard(0));
    if (remainingDraws) {
      remainingDraws--;
      moving = Pile();
      Card card = stockDeck.removeTopCard();
      card.flip();
      moving.addCard(card);
      moving.x = 1;
      moving.y = 1;
    }
    else {
      mode = selecting;
    }
  }
}

void drawMovingPile() {
  drawPile(&moving);
  switch (activeLocation) {
    case tableau1:
    case tableau2:
    case tableau3:
    case tableau4:
    case tableau5:
    case tableau6:
    case tableau7:
      moving.x = updatePosition(moving.x, tableau[activeLocation - tableau1].x);
      moving.y = updatePosition(moving.y, tableau[activeLocation - tableau1].y + 2 * tableau[activeLocation - tableau1].getCardCount() + 2);
      break;
  }
}

byte updatePosition(byte current, byte destination) {
  if (current == destination) return current;

  byte delta = (destination - current) / 3;
  if (delta == 0 && ((gb.frameCount % 3) == 0)) delta = destination > current ? 1 : -1;
  return current + delta;
}

void drawCursor(byte x, byte y, bool flipped) {
  if (flipped) {
    for (int i = 0; i < 4; i++) {
      gb.display.setColor(BLACK);
      gb.display.drawPixel(x + 3 + i, y + i);
      gb.display.drawPixel(x + 3 + i, y + (6 - i));
      gb.display.setColor(WHITE);
      gb.display.drawFastHLine(x + 3, y + i, i);
      gb.display.drawFastHLine(x + 3, y + (6 - i), i);
    }
    gb.display.setColor(BLACK);
    gb.display.drawFastVLine(x + 2, y, 7);
    gb.display.drawFastHLine(x, y + 2, 2);
    gb.display.drawFastHLine(x, y + 4, 2);
    gb.display.drawPixel(x, y + 3);
    gb.display.setColor(WHITE);
    gb.display.drawFastHLine(x + 1, y + 3, 2);
  }
  else {
    for (int i = 0; i < 4; i++) {
      gb.display.setColor(BLACK);
      gb.display.drawPixel(x + 3 - i, y + i);
      gb.display.drawPixel(x + 3 - i, y + (6 - i));
      gb.display.setColor(WHITE);
      gb.display.drawFastHLine(x + 4 - i, y + i, i);
      gb.display.drawFastHLine(x + 4 - i, y + (6 - i), i);
    }
    gb.display.setColor(BLACK);
    gb.display.drawFastVLine(x + 4, y, 7);
    gb.display.drawFastHLine(x + 5, y + 2, 2);
    gb.display.drawFastHLine(x + 5, y + 4, 2);
    gb.display.drawPixel(x + 6, y + 3);
    gb.display.setColor(WHITE);
    gb.display.drawFastHLine(x + 4, y + 3, 2);
  }
}

void drawSegmentA(byte x, byte y) {
  gb.display.drawFastHLine(x, y, 3);
}

void drawSegmentB(byte x, byte y) {
  gb.display.drawFastVLine(x + 2, y, 3);
}

void drawSegmentC(byte x, byte y) {
  gb.display.drawFastVLine(x + 2, y + 2, 3);
}

void drawSegmentD(byte x, byte y) {
  gb.display.drawFastHLine(x, y + 4, 3);
}

void drawSegmentE(byte x, byte y) {
  gb.display.drawFastVLine(x, y + 2, 3);
}

void drawSegmentF(byte x, byte y) {
  gb.display.drawFastVLine(x, y, 3);
}

void drawSegmentG(byte x, byte y) {
  gb.display.drawFastHLine(x, y + 2, 3);
}

