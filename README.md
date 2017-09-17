# gamebuino-solitaire

## About

Solitaire game for [Gamebuino](http://gamebuino.com) or [MAKERbuino](http://makerbuino.com/).

![Screenshot](src/images/sim.gif?raw=true)

[Try it out](http://games.aoneill.com/play.html?hex=https%3A%2F%2Fraw.githubusercontent.com%2Faoneill01%2Fgamebuino-solitaire%2Fmaster%2Fbin%2FSOLITAIR.HEX) in a web browser!

## Installation

Copy `SOLITAIR.HEX` and `SOLITAIR.INF` to your SD card. These can be found in the `bin/` folder or attached to a release. 

## Instructions

See https://en.wikipedia.org/wiki/Klondike_(solitaire) for general Klondike solitaire rules.

### Controls

- **Up, Down, Left, Right** - Move selection cursor, move selected cards.
- **A button** - Pick up cards, draw cards, place cards.
- **B button** - Automaticially moves the card at the cursor to the foundation if there is a valid move.
- **C button** - Pause, quit.

### New game menu

- **New easy game** - Cards will be drawn one at a time from the stock.
- **New hard game** - Three cards will be flipped each time you draw from the stock.
- **Game statistics** - Display the number of games started and won for both easy and hard.

### Pause menu

- **Resume game** - Continue playing
- **Quit game** - Quit the game and return to the title screen. You will not be able to return to this game.
- **Game statistics** - Display the number of games started and won for both easy and hard.
- **Save for later** - *Only available when you are in selection mode. If you are actively moving cards, this won't be available.* Saves the game state and returns to the title screen. You can then turn the system off, turn it back on, and continue from where you were at. Once you continue, the save state is cleared.
- **Undo last move** - *Only available when you are in selection mode and there is move to undo.*

### Gameplay

Move the cursor to the stock and press **A** to draw. Press **A** while the cursor is on a card to pick it up. Use the arrows to move the card around. Press **A** again to place the card at the current location. If it is an illegal move, the card will return to where it came from. To pick multiple cards up from a tableau, move the cursor to the tableau, press up to select multiple cards, and press **A** to pick them up.
