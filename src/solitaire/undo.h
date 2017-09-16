#include <Gamebuino.h>
#include "pile.h"

#define UNDO_STACK_SIZE 10

struct UndoAction {
    byte special;
    Pile *source;
    Pile *destination;
};

class UndoStack {
    public:
        UndoStack();
        void pushAction(UndoAction action);
        UndoAction popAction();
        bool isEmpty() const;

    private:
        UndoAction actions[UNDO_STACK_SIZE];
        byte bottomIndex, topIndex;
};