#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

typedef enum {
    TK_OPERATOR,
    TK_NUM,
    TK_EOF,
} TokenType;

typedef struct Token {
    TokenType type;
    struct Token *next;
    int value;
    char *str;    
} Token;

Token *token;

void PrintError(char *format, ...) {
    va_list list;
    va_start(list, format);
    vfprintf(stderr, format, list);
    fprintf(stderr, "\n");
    exit(1);
}

Token * CreateToken(TokenType type, Token *cur, char *str) {
    Token *token = calloc(1, sizeof(Token));
    token->type = type;
    token->str = str;
    cur->next = token;

    return token;
}

Token * Tokenize(char* p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p ++;
        } else if (*p == '+' || *p == '-') {
            cur = CreateToken(TK_OPERATOR, cur, p ++);
        } else if (isdigit(*p)) {
            cur = CreateToken(TK_NUM, cur, p);
            cur->value = strtol(p, &p, 10);
        } else {
            PrintError("Tokenize failed");
        }
    }

    CreateToken(TK_EOF, cur, p);
    return head.next;
}

int ExpectNumber() {
    if (token->type != TK_NUM) {
        PrintError("Not Number");
    }

    int value = token->value;
    token = token->next;

    return value;
}

bool ExpectSymbol(char operator) {
    if (token->type != TK_OPERATOR || token->str[0] != operator) {
        return false;
    }

    token = token->next;
    return true;
}

bool AtEof() {
    return token->type == TK_EOF;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of parameters\n");
        return 1;
    }

    token = Tokenize(argv[1]);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    printf("  mov rax, %d\n", ExpectNumber());

    while (!AtEof()) {
        if (ExpectSymbol('+')) {
            printf("  add rax, %d\n", ExpectNumber());
        } else if (ExpectSymbol('-')) {
            printf("  sub rax, %d\n", ExpectNumber());
        } else {
            PrintError("Unknown symbol");
        }
    }

    printf("  ret\n");

    return 0;
}
