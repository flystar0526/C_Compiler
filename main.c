#include <stdio.h>

#include "parser.h"
#include "tokenizer.h"

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  // 標記解析
  user_input = argv[1];
  token = Tokenize(user_input);
  Node *node = Expr();

  // 輸出前半部份組合語言指令
  printf(".intel_syntax noprefix\n");
  printf(".global main\n");
  printf("main:\n");

  // 利用抽象語法樹生成指令
  GenCode(node);

  // 將程式的結果放到rax中，作為main函數的返回值
  printf("  pop rax\n");
  printf("  ret\n");
  return 0;
}
