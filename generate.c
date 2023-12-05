#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "logic.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *logic() {
    return expr();
}

Node *expr() {
    return equality();
}

Node *equality() {
    Node *node = conditional();

    for (;;) {
        if (consume("==")) {
            node = new_node(ND_EQ, node, equality());
        } else {
            return node;
        }
    }
}

Node *conditional() {
    Node *node = disjunction();

    for (;;) {
        if (consume("->")) {
            node = new_node(ND_CON, node, conditional());
        } else {
            return node;
        }
    }
}

Node *disjunction() {
    Node *node = conjunction();

    for (;;) {
        if (consume("||")) {
            node = new_node(ND_DJ, node, disjunction());
        } else {
            return node;
        }
    }
}

Node *conjunction() {
    Node *node = unary();

    for (;;) {
        if (consume("&&")) {
            node = new_node(ND_CJ, node, conjunction());
        } else {
            return node;
        }
    }
}

Node *unary() {
    if (consume("!")) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_NE;
        node->rhs = unary();
        return node;
    } 
    return primary();
}

Node *primary()
{
    if (consume("(")) {
        Node *node = expr();
        expect(")");
        return node;
    }

    Token *tok = consume_ident();
    if (tok) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_PR;
        node->str = tok->str;
        return node;
    }
    error("Error: no proposition");
}


void permutate(int cnt) {
    double len = pow(2, cnt);
    int result[(int)len][cnt];
    for (int i = 0; i < (int)len; i++) {
        int tmp = i;
        for (int j = 0; j < cnt; j++) {
            if (tmp % 2 == 1) {
                graph[i][j] = 1;
            } else {
                graph[i][j] = 0;
            }
            tmp = tmp >> 1;
        }
    }
}

int evaluate(Node *node, int *cond) {
    size_t cnt = strlen(propos);
    switch (node->kind) {
        case ND_PR: // proposition
            for (int i = 0; i < cnt; i++) {
                if (memcmp(&propos[i], node->str, 1) == 0) {
                    node->result = cond[i];
                }
            }
            return node->result;
        case ND_NE: // negation
            evaluate(node->rhs, cond);
            node->result = !node->rhs->result;
            return node->result;
    }
    evaluate(node->lhs, cond);
    evaluate(node->rhs, cond);

    switch (node->kind) {
        case ND_DJ: // disjunction
            node->result = (node->lhs->result || node->rhs->result);
            break;
        case ND_CJ: // conjunction
            node->result = (node->lhs->result && node->rhs->result);
            break;
        case ND_CON: // condition
            if (node->lhs->result == 1 && node->rhs->result == 0) {
                node->result = 0;
            } else {
                node->result = 1;
            }
            break;
        case ND_EQ: // equality
            node->result = (node->lhs->result == node->rhs->result);
            break;
        
    }
    
    return node->result;
    
}