#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>

Gamebuino gb;

byte activeStack = 0;
byte cursorX = 11;
byte cursorY = 5;

void setup() {
  gb.begin();
  showTitle();
}

void loop() {
  if (gb.update()) {
    if (gb.buttons.pressed(BTN_C)) showTitle();
    if (gb.buttons.pressed(BTN_RIGHT)) {
      if (activeStack != 5 && activeStack != 12) {
        activeStack++;
      }
    }
    if (gb.buttons.pressed(BTN_LEFT)) {
      if (activeStack != 0 && activeStack != 6) {
        activeStack--;
      }
    }
    if (gb.buttons.pressed(BTN_DOWN)) {
      if (activeStack < 2) activeStack += 6;
      else if (activeStack < 6) activeStack += 7;
    }
    if (gb.buttons.pressed(BTN_UP)) {
      if (activeStack > 7) activeStack -= 7;
      else if (activeStack > 5) activeStack -= 6;
    }
    
    // Deck
    drawCard(1, 1, 1 + (1 << 4));
    // Drawn
    for (int i = 0; i < 3; i++) {
      drawCard(13 + i * 2, 1, 2 + (2 << 4));
    }
    // Destination
    for (int i = 0; i < 4; i++) {
      drawCard(37 + i * 12, 1, 3 + i + (i << 4));
    }
    
    // Stacks
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j <= i; j++) {
        byte card = i + 7 + (j << 4);
        if (i != j) card = card | 1 << 7;
        drawCard(i * 12 + 1, 2 * j + 17, card);
      }
    }

    drawCursor();
  }
}

void showTitle() {
  gb.titleScreen(F("Solitaire"));
  gb.battery.show = false;
  activeStack = 0;
  cursorX = 11;
  cursorY = 5;
}

void drawCard(byte x, byte y, byte card) {
  // Fill 
  byte fill = WHITE;
  if (card & 0x80) fill = GRAY;
  gb.display.setColor(fill);
  gb.display.fillRect(x + 1, y + 1, 8, 12);
  
  // Draw border
  gb.display.setColor(BLACK);
  gb.display.drawFastHLine(x + 1, y, 8);
  gb.display.drawFastHLine(x + 1, y + 13, 8);
  gb.display.drawFastVLine(x, y + 1, 12);
  gb.display.drawFastVLine(x + 9, y + 1, 12);

  if (card & 0x80) return;

  if ((1 << 5) & card) gb.display.setColor(GRAY);
  drawSuit(x + 2, y + 2, card);
  drawValue(x + 5, y + 7, card);
}

void drawSuit(byte x, byte y, byte card) {
  if ((card & (3 << 4)) == (0 << 4)) drawSpade(x, y);
  else if ((card & (3 << 4)) == (1 << 4)) drawClub(x, y);
  else if ((card & (3 << 4)) == (2 << 4)) drawHeart(x, y);
  else if ((card & (3 << 4)) == (3 << 4)) drawDiamond(x, y);
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

void drawValue(byte x, byte y, byte card) {
  switch (0x0f & card) {
    case 1:
      drawAce(x, y);
      break;
    case 2:
      drawTwo(x, y);
      break;
    case 3:
      drawThree(x, y);
      break;
    case 4:
      drawFour(x, y);
      break;
    case 5:
      drawFive(x, y);
      break;
    case 6:
      drawSix(x, y);
      break;
    case 7:
      drawSeven(x, y);
      break;
    case 8:
      drawEight(x, y);
      break;
    case 9:
      drawNine(x, y);
      break;
    case 10:
      drawTen(x, y);
      break;
    case 11:
      drawJack(x, y);
      break;
    case 12:
      drawQueen(x, y);
      break;
    case 13:
      drawKing(x, y);
      break;
    default:
      break;
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
  switch (activeStack) {
    case 0:
      cursorX = updatePosition(cursorX, 11);
      cursorY = updatePosition(cursorY, 5);
      break;
    case 1:
      cursorX = updatePosition(cursorX, 27);
      cursorY = updatePosition(cursorY, 5);
      break;
    case 2:
      cursorX = updatePosition(cursorX, 30);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case 3:
      cursorX = updatePosition(cursorX, 42);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case 4:
      cursorX = updatePosition(cursorX, 54);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case 5:
      cursorX = updatePosition(cursorX, 66);
      cursorY = updatePosition(cursorY, 5);
      flipped = true;
      break;
    case 6:
      cursorX = updatePosition(cursorX, 11);
      cursorY = updatePosition(cursorY, 21);
      break;
    case 7:
      cursorX = updatePosition(cursorX, 23);
      cursorY = updatePosition(cursorY, 23);
      break;
    case 8:
      cursorX = updatePosition(cursorX, 35);
      cursorY = updatePosition(cursorY, 25);
      break;
    case 9:
      cursorX = updatePosition(cursorX, 30);
      cursorY = updatePosition(cursorY, 27);
      flipped = true;
      break;
    case 10:
      cursorX = updatePosition(cursorX, 42);
      cursorY = updatePosition(cursorY, 29);
      flipped = true;
      break;
    case 11:
      cursorX = updatePosition(cursorX, 54);
      cursorY = updatePosition(cursorY, 31);
      flipped = true;
      break;
    case 12:
      cursorX = updatePosition(cursorX, 66);
      cursorY = updatePosition(cursorY, 33);
      flipped = true;
      break;
  }
  drawCursor(cursorX, cursorY, flipped);
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

