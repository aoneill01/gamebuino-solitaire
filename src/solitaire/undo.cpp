#include "undo.h"

UndoStack::UndoStack() {
    bottomIndex = topIndex = 0;
}

void UndoStack::pushAction(UndoAction action) {
    actions[topIndex] = action;
    topIndex = (topIndex + 1) % UNDO_STACK_SIZE;
    if (topIndex == bottomIndex) {
        bottomIndex = (bottomIndex + 1) % UNDO_STACK_SIZE;
    }
}

UndoAction UndoStack::popAction() {
    if (!isEmpty()) {
        if (topIndex == 0) topIndex = UNDO_STACK_SIZE;
        else topIndex--;
        return actions[topIndex];
    }
    // Invalid
    return UndoAction();
}

bool UndoStack::isEmpty() const {
    return bottomIndex == topIndex;
}