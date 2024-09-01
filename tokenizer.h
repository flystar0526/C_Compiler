#pragma once

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 標記種類
typedef enum {
  TK_SYMBOL,  // 符號
  TK_NUM,     // 整數標記
  TK_EOF,     // 代表輸入結束的標記
} TokenType;

typedef struct Token {
  TokenType type;      // 標記的型態
  struct Token *next;  // 下一個輸入標記
  int value;           // type 為 TK_NUM 時的數值
  char *str;           // 標記文字列
} Token;

Token *Tokenize(char *p);
Token *NewToken(TokenType type, Token *cur, char *str);
bool Consume(char op);
bool AtEof();
int ExpectNumber();
void ErrorAt(char *position, char *fmt, ...);

extern Token *token;
extern char *user_input;
