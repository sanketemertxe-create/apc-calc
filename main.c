#include "apc.h"

Node *createNode(int d) {
    Node *n = (Node *)malloc(sizeof(Node));
    if (!n) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    n->digit = d;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

void appendDigit(Node **head, Node **tail, int d) {
    Node *n = createNode(d);
    if (*head == NULL) {
        *head = n;
        *tail = n;
        return;
    }
    n->prev = *tail;
    (*tail)->next = n;
    *tail = n;
}

void freeList(Node *head) {
    while (head) {
        Node *tmp = head;
        head = head->next;
        free(tmp);
    }
}

int listLength(Node *head) {
    int c = 0;
    while (head) { c++; head = head->next; }
    return c;
}

void trimLeadingZeroNodes(Node **tailRef) {
    Node *tail = *tailRef;
    while (tail && tail->prev && tail->digit == 0) {
        Node *p = tail->prev;
        p->next = NULL;
        free(tail);
        tail = p;
    }
    *tailRef = tail;
}

Node *getTail(Node *head) {
    if (!head) return NULL;
    while (head->next) head = head->next;
    return head;
}

int isZero(Node *head) {
    return (head != NULL && head->next == NULL && head->digit == 0);
}

Node *stringMagToList(const char *s) {
    Node *head = NULL, *tail = NULL;
    int len = strlen(s);
    for (int i = len - 1; i >= 0; i--)
        appendDigit(&head, &tail, s[i] - '0');
    trimLeadingZeroNodes(&tail);
    return head;
}

Node *parseNumber(const char *str, int *sign) {
    int start = 0;
    *sign = 0;
    if (str[0] == '-') { *sign = 1; start = 1; }
    else if (str[0] == '+') { start = 1; }

    Node *head = NULL, *tail = NULL;
    int len = strlen(str);
    for (int i = len - 1; i >= start; i--)
        appendDigit(&head, &tail, str[i] - '0');
    trimLeadingZeroNodes(&tail);
    if (isZero(head)) *sign = 0;
    return head;
}

void printNumber(Node *head, int sign) {
    if (sign && !isZero(head)) putchar('-');
    Node *t = getTail(head);
    while (t) {
        putchar('0' + t->digit);
        t = t->prev;
    }
    putchar('\n');
}

int compareMag(Node *a, Node *b) {
    int la = listLength(a), lb = listLength(b);
    if (la != lb) return (la > lb) ? 1 : -1;

    Node *ta = getTail(a), *tb = getTail(b);
    while (ta && tb) {
        if (ta->digit != tb->digit)
            return (ta->digit > tb->digit) ? 1 : -1;
        ta = ta->prev;
        tb = tb->prev;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <num1> <operator> <num2>\n", argv[0]);
        fprintf(stderr, "Example: %s 123456789012345678901234567890 + 987654321\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *num1Str = argv[1];
    const char *opStr   = argv[2];
    const char *num2Str = argv[3];
    char op = opStr[0];

    if (isValidNumber(num1Str) == FAILURE) {
        fprintf(stderr, "Error: '%s' is not a valid number\n", num1Str);
        return EXIT_FAILURE;
    }
    if (isValidNumber(num2Str) == FAILURE) {
        fprintf(stderr, "Error: '%s' is not a valid number\n", num2Str);
        return EXIT_FAILURE;
    }
    if (strlen(opStr) != 1 || isValidOperator(op) == FAILURE) {
        fprintf(stderr, "Error: '%s' is not a valid operator. Use one of + - * /\n", opStr);
        return EXIT_FAILURE;
    }

    int sign1, sign2, resultSign;
    Node *mag1 = parseNumber(num1Str, &sign1);
    Node *mag2 = parseNumber(num2Str, &sign2);
    Node *resultMag = NULL;
    char *remStr = NULL;

    switch (op) {
        case '+':
            resultMag = addSigned(mag1, sign1, mag2, sign2, &resultSign);
            break;

        case '-':
            resultMag = subtractSigned(mag1, sign1, mag2, sign2, &resultSign);
            break;

        case '*':
            resultMag = multiplySigned(mag1, sign1, mag2, sign2, &resultSign);
            break;

        case '/':
            if (isZero(mag2)) {
                fprintf(stderr, "Error: division by zero\n");
                freeList(mag1);
                freeList(mag2);
                return EXIT_FAILURE;
            }
            resultMag = divideSigned(mag1, sign1, mag2, sign2, &resultSign, &remStr);
            break;
    }

    printf("\nResult: ");
    printNumber(resultMag, resultSign);

    if (op == '/') {
        printf("Remainder: %s\n", remStr);
        free(remStr);
    }

    freeList(mag1);
    freeList(mag2);
    freeList(resultMag);

    return 0;
}
