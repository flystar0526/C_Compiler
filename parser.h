#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "tokenizer.h"

// 抽象語法樹結點的種類
typedef enum {
  ND_ADD,  // +
  ND_SUB,  // -
  ND_MUL,  // *
  ND_DIV,  // /
  ND_EQ,   // ==
  ND_NE,   // !=
  ND_LT,   // <
  ND_LE,   // <=
  ND_NUM,  // 整數
} NodeType;

// 抽象語法樹結點的結構
typedef struct Node {
  NodeType type;       // 結點的型態
  struct Node *left;   // 左邊
  struct Node *right;  // 右邊
  int value;           // 只在kind為ND_NUM時使用
} Node;

Node *NewNode(NodeType type, Node *left, Node *right);
Node *NewNodeNum(int value);

// 生成規則
Node *Expr();
Node *Equality();
Node *Relational();
Node *Add();
Node *Mul();
Node *Unary();
Node *Primary();

// 生成組合語言
void GenCode(Node *node);
