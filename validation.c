#include "apc.h"

int isValidNumber(const char *str) {
    if (str == NULL || str[0] == '\0') {
        return FAILURE;
    }

    int start = 0;
    if (str[0] == '+' || str[0] == '-') {
        start = 1;
    }

    if (str[start] == '\0') {
        return FAILURE;
    }

    for (int i = start; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i])) {
            return FAILURE;
        }
    }

    return SUCCESS;
}

int isValidOperator(char op) {
    if (op == '+' || op == '-' || op == '*' || op == '/') {
        return SUCCESS;
    }
    return FAILURE;
}
