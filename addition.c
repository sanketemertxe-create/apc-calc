#include "apc.h"

Node *addMag(Node *a, Node *b) {
    Node *head = NULL, *tail = NULL;
    int carry = 0;
    while (a || b || carry) {
        int da = a ? a->digit : 0;
        int db = b ? b->digit : 0;
        int sum = da + db + carry;
        carry = sum / 10;
        appendDigit(&head, &tail, sum % 10);
        if (a) a = a->next;
        if (b) b = b->next;
    }
    trimLeadingZeroNodes(&tail);
    return head;
}

Node *addSigned(Node *magA, int signA, Node *magB, int signB, int *outSign) {
    Node *result;
    if (signA == signB) {
        result = addMag(magA, magB);
        *outSign = isZero(result) ? 0 : signA;
    } else {
        int cmp = compareMag(magA, magB);
        if (cmp == 0) {
            result = stringMagToList("0");
            *outSign = 0;
        } else if (cmp > 0) {
            result = subMag(magA, magB);
            *outSign = signA;
        } else {
            result = subMag(magB, magA);
            *outSign = signB;
        }
    }
    return result;
}
