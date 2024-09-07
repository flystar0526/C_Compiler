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
    } else if (StartWith(p, "==") || StartWith(p, "!=") || StartWith(p, "<=") ||
               StartWith(p, ">=")) {
      // 多字元運算子
      cur = NewToken(TK_SYMBOL, cur, p, 2);
      p += 2;
    } else if (strchr("+-*/()<>", *p)) {
      cur = NewToken(TK_SYMBOL, cur, p++, 1);
    } else if (isdigit(*p)) {
      cur = NewToken(TK_NUM, cur, p, 0);
      char *q = p;
      cur->value = strtol(p, &p, 10);
      cur->length = p - q;
    } else {
      ErrorAt(p, "Invalid Token");
      exit(1);
    }
  }

  NewToken(TK_EOF, cur, p, 0);
  return head.next;
}

Token *NewToken(TokenType type, Token *cur, char *str, int length) {
  Token *token = calloc(1, sizeof(Token));
  token->type = type;
  token->str = str;
  token->length = length;
  cur->next = token;
  return token;
}

bool Consume(char *op) {
  if (token->type != TK_SYMBOL || strlen(op) != token->length ||
      memcmp(token->str, op, token->length)) {
    return false;
  }
  token = token->next;
  return true;
}

bool AtEof() { return token->type == TK_EOF; }

bool StartWith(char *p, char *q) { return memcmp(p, q, strlen(q)) == 0; }

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
