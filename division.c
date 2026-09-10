#include "apc.h"

char *magToString(Node *head) {
    int len = listLength(head);
    char *s = (char *)malloc(len + 1);
    Node *t = getTail(head);
    int idx = 0;
    while (t) { s[idx++] = '0' + t->digit; t = t->prev; }
    s[idx] = '\0';
    return s;
}

char *trimLeadingZerosStr(const char *s) {
    int i = 0;
    while (s[i] == '0' && s[i + 1] != '\0') i++;
    return strdup(s + i);
}

int compareStr(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    if (la != lb) return (la > lb) ? 1 : -1;
    int c = strcmp(a, b);
    return (c > 0) ? 1 : (c < 0) ? -1 : 0;
}

char *subtractStr(const char *a, const char *b) {
    int la = strlen(a), lb = strlen(b);
    char *res = (char *)malloc(la + 1);
    res[la] = '\0';
    int borrow = 0;
    int ib = lb - 1;
    for (int ia = la - 1; ia >= 0; ia--) {
        int da = a[ia] - '0';
        int db = (ib >= 0) ? b[ib] - '0' : 0;
        ib--;
        int diff = da - db - borrow;
        if (diff < 0) { diff += 10; borrow = 1; }
        else borrow = 0;
        res[ia] = '0' + diff;
    }
    char *trimmed = trimLeadingZerosStr(res);
    free(res);
    return trimmed;
}

char *mulBySmall(const char *num, int digit) {
    int len = strlen(num);
    char *buf = (char *)malloc(len + 2);
    buf[len + 1] = '\0';
    int carry = 0;
    for (int i = len - 1; i >= 0; i--) {
        int prod = (num[i] - '0') * digit + carry;
        buf[i + 1] = '0' + (prod % 10);
        carry = prod / 10;
    }
    char *result;
    if (carry) {
        buf[0] = '0' + carry;
        result = trimLeadingZerosStr(buf);
    } else {
        result = trimLeadingZerosStr(buf + 1);
    }
    free(buf);
    return result;
}

void divideStr(const char *dividend, const char *divisor,
               char **quotientOut, char **remainderOut) {
    int len = strlen(dividend);
    char *quotient = (char *)malloc(len + 1);
    quotient[0] = '\0';
    int qlen = 0;
    char *remainder = strdup("0");

    for (int i = 0; i < len; i++) {
        int rl = strlen(remainder);
        char *shifted;
        if (strcmp(remainder, "0") == 0) {
            shifted = (char *)malloc(2);
            shifted[0] = dividend[i];
            shifted[1] = '\0';
        } else {
            shifted = (char *)malloc(rl + 2);
            strcpy(shifted, remainder);
            shifted[rl] = dividend[i];
            shifted[rl + 1] = '\0';
        }
        free(remainder);
        remainder = trimLeadingZerosStr(shifted);
        free(shifted);

        int d = 0;
        for (int cand = 9; cand >= 0; cand--) {
            char *prod = mulBySmall(divisor, cand);
            if (compareStr(prod, remainder) <= 0) {
                d = cand;
                free(prod);
                break;
            }
            free(prod);
        }

        quotient[qlen++] = '0' + d;
        quotient[qlen] = '\0';

        char *prodD = mulBySmall(divisor, d);
        char *newRem = subtractStr(remainder, prodD);
        free(prodD);
        free(remainder);
        remainder = newRem;
    }

    *quotientOut = trimLeadingZerosStr(quotient);
    free(quotient);
    *remainderOut = remainder;
}

Node *divideSigned(Node *magA, int signA, Node *magB, int signB, int *outSign, char **remainderStr) {
    char *s1 = magToString(magA);
    char *s2 = magToString(magB);
    char *qStr;

    divideStr(s1, s2, &qStr, remainderStr);

    Node *result = stringMagToList(qStr);
    *outSign = (signA != signB) ? 1 : 0;
    if (isZero(result)) *outSign = 0;

    free(s1);
    free(s2);
    free(qStr);

    return result;
}
