#include "apc.h"

Node *mulMag(Node *a, Node *b) {
    int la = listLength(a), lb = listLength(b);
    int n = la + lb;
    int *res = (int *)calloc(n, sizeof(int));
    if (!res) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;
    for (Node *pa = a; pa; pa = pa->next, i++) {
        int carry = 0;
        int j = 0;
        for (Node *pb = b; pb; pb = pb->next, j++) {
            int mul = pa->digit * pb->digit + res[i + j] + carry;
            res[i + j] = mul % 10;
            carry = mul / 10;
        }
        int k = i + j;
        while (carry) {
            int sum = res[k] + carry;
            res[k] = sum % 10;
            carry = sum / 10;
            k++;
        }
    }

    Node *head = NULL, *tail = NULL;
    for (int idx = 0; idx < n; idx++)
        appendDigit(&head, &tail, res[idx]);
    free(res);
    trimLeadingZeroNodes(&tail);
    return head;
}

Node *multiplySigned(Node *magA, int signA, Node *magB, int signB, int *outSign) {
    Node *result = mulMag(magA, magB);
    *outSign = (signA != signB) ? 1 : 0;
    if (isZero(result)) *outSign = 0;
    return result;
}
