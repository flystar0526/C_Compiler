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

// expr = equality
Node* Expr() { return Equality(); }

// equality = relational ("==" relational | "!=" relational)*
Node* Equality() {
  Node* node = Relational();

  while (true) {
    if (Consume("==")) {
      node = NewNode(ND_EQ, node, Relational());
    } else if (Consume("!=")) {
      node = NewNode(ND_NE, node, Relational());
    } else {
      return node;
    }
  }
}

// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
Node* Relational() {
  Node* node = Add();

  while (true) {
    if (Consume("<")) {
      node = NewNode(ND_LT, node, Add());
    } else if (Consume("<=")) {
      node = NewNode(ND_LE, node, Add());
    } else if (Consume(">")) {
      node = NewNode(ND_LT, Add(), node);
    } else if (Consume(">=")) {
      node = NewNode(ND_LE, Add(), node);
    } else {
      return node;
    }
  }
}

// add = mul ("+" mul | "-" mul)*
Node* Add() {
  Node* node = Mul();

  while (true) {
    if (Consume("+")) {
      node = NewNode(ND_ADD, node, Mul());
    } else if (Consume("-")) {
      node = NewNode(ND_SUB, node, Mul());
    } else {
      return node;
    }
  }
}

// mul = unary ("*" unary | "/" unary)*
Node* Mul() {
  Node* node = Unary();

  while (true) {
    if (Consume("*")) {
      node = NewNode(ND_MUL, node, Unary());
    } else if (Consume("/")) {
      node = NewNode(ND_DIV, node, Unary());
    } else {
      return node;
    }
  }
}

// unary = ("+" | "-")? primary
Node* Unary() {
  if (Consume("+")) {
    return Unary();
  }
  if (Consume("-")) {
    return NewNode(ND_SUB, NewNodeNum(0), Unary());
  }
  return Primary();
}

// primary = "(" expr ")" | num
Node* Primary() {
  if (Consume("(")) {
    Node* node = Expr();
    if (!Consume(")")) {
      ErrorAt(token->str,
              "There is no closing bracket corresponding to the opening "
              "bracket.: %s",
              token->str);
    }
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
    case ND_EQ:
      printf("  cmp rax, rdi\n");
      printf("  sete al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_NE:
      printf("  cmp rax, rdi\n");
      printf("  setne al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LT:
      printf("  cmp rax, rdi\n");
      printf("  setl al\n");
      printf("  movzb rax, al\n");
      break;
    case ND_LE:
      printf("  cmp rax, rdi\n");
      printf("  setle al\n");
      printf("  movzb rax, al\n");
      break;
  }

  printf("  push rax\n");
}
