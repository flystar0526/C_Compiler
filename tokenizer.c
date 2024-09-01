#include "tokenizer.h"

Token *token = NULL;
char *user_input = NULL;

Token *Tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    if (isspace(*p)) {
      p++;
    } else if (*p == '+' || *p == '-' || *p == '*' || *p == '/' || *p == '(' ||
               *p == ')') {
      cur = NewToken(TK_SYMBOL, cur, p++);
    } else if (isdigit(*p)) {
      cur = NewToken(TK_NUM, cur, p);
      cur->value = strtol(p, &p, 10);
    } else {
      ErrorAt(p, "Cannot tokenize");
      exit(1);
    }
  }

  NewToken(TK_EOF, cur, p);
  return head.next;
}

Token *NewToken(TokenType type, Token *cur, char *str) {
  Token *token = calloc(1, sizeof(Token));
  token->type = type;
  token->str = str;
  cur->next = token;
  return token;
}

bool Consume(char op) {
  if (token->type != TK_SYMBOL || token->str[0] != op) {
    return false;
  }
  token = token->next;
  return true;
}

bool AtEof() { return token->type == TK_EOF; }

int ExpectNumber() {
  if (token->type != TK_NUM) {
    ErrorAt(token->str, "Not a number");
  }

  int value = token->value;
  token = token->next;
  return value;
}

void ErrorAt(char *location, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int pos = location - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", pos, "");  // 輸出pos個空白
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}
