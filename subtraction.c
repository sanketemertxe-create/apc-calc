#include "apc.h"

Node *subMag(Node *a, Node *b) {
    Node *head = NULL, *tail = NULL;
    int borrow = 0;
    while (a) {
        int da = a->digit;
        int db = b ? b->digit : 0;
        int diff = da - db - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else borrow = 0;
        appendDigit(&head, &tail, diff);
        a = a->next;
        if (b) b = b->next;
    }
    trimLeadingZeroNodes(&tail);
    return head;
}

Node *subtractSigned(Node *magA, int signA, Node *magB, int signB, int *outSign) {
    return addSigned(magA, signA, magB, !signB, outSign);
}
