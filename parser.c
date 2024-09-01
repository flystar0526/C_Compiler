#include "parser.h"

Node* NewNode(NodeType type, Node* left, Node* right) {
  Node* node = calloc(1, sizeof(Node));
  node->type = type;
  node->left = left;
  node->right = right;
  return node;
}

Node* NewNodeNum(int value) {
  Node* node = calloc(1, sizeof(Node));
  node->type = ND_NUM;
  node->value = value;
  return node;
}

Node* Expr() {
  Node* node = Mul();

  while (true) {
    if (Consume('+')) {
      node = NewNode(ND_ADD, node, Mul());
    } else if (Consume('-')) {
      node = NewNode(ND_SUB, node, Mul());
    } else {
      return node;
    }
  }
}

Node* Mul() {
  Node* node = Term();

  while (true) {
    if (Consume('*')) {
      node = NewNode(ND_MUL, node, Term());
    } else if (Consume('/')) {
      node = NewNode(ND_DIV, node, Term());
    } else {
      return node;
    }
  }
}

Node* Term() {
  if (Consume('(')) {
    Node* node = Expr();
    Consume(')');
    return node;
  }

  return NewNodeNum(ExpectNumber());
}

void GenCode(Node* node) {
  if (node->type == ND_NUM) {
    printf("  push %d\n", node->value);
    return;
  }

  GenCode(node->left);
  GenCode(node->right);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->type) {
    case ND_ADD:
      printf("  add rax, rdi\n");
      break;
    case ND_SUB:
      printf("  sub rax, rdi\n");
      break;
    case ND_MUL:
      printf("  imul rax, rdi\n");
      break;
    case ND_DIV:
      printf("  cqo\n");
      printf("  idiv rdi\n");
      break;
  }

  printf("  push rax\n");
}
