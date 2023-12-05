#include <stdbool.h>

#ifndef MYCC_H
#define MYCC_H

typedef enum {
    TK_RESERVED, // operator
    TK_IDENT, // identifier
    TK_EOF, // end
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

typedef enum {
    ND_DJ, // ||
    ND_CJ, // &&
    ND_CON, // ->
    ND_EQ, // ==
    ND_NE, // !
    ND_PR, // proposition
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    char *str;
    int offset; // offset from rbp
    bool result;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident();
void expect(char *op);
bool at_eof();
Token *new_token(TokenKind kind, Token *cur, char *str, int len);
bool startsWith(char *p, char *q);
Token *tokenize(char *p);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *logic();
Node *expr();
Node *equality();
Node *conditional();
Node *disjunction();
Node *conjunction();
Node *primary();
Node *unary();
void permutate(int cnt);
int evaluate(Node *node, int *cond);

extern Token *token;
extern char *user_input;
extern char propos[10];
extern int graph[1024][10];
#endif