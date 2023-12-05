#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logic.h"

void error(char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
} 

void error_at(char *loc, char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

bool consume(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        return false;
    }
    token = token->next;
    return true;
}

Token *consume_ident() {
    if (token->kind != TK_IDENT) {
        return NULL;
    }
    Token *current = token;
    token = token->next;
    return current;
}

void expect(char *op) {
    if (token->kind != TK_RESERVED ||
        strlen(op) != token->len ||
        memcmp(token->str, op, token->len)) {
        error_at(token->str, "Error: not '%s'", op);
    }
    token = token->next;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Token *new_token(TokenKind kind, Token *cur, char *str, int len) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    tok->len = len;
    return tok;
}

bool startsWith(char *p, char *q) {
    return memcmp(p, q, strlen(q)) == 0;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (startsWith(p, "->") ||
            startsWith(p, "||") ||
            startsWith(p, "&&") ||
            startsWith(p, "==")) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("!()", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if ('a' <= *p && *p <= 'z') {
            size_t len = strlen(propos);
            bool found = false;
            for (int i = 0; i < len; i++) {
                if (memcmp(&propos[i], p, 1) == 0) {
                    found = true;
                }
            }
            if ( ! found) {
                if (len >= 10) {
                    error("Error: too many proposition");
                }
                strncat(propos, p, 1);
            }
            cur = new_token(TK_IDENT, cur, p++, 1);
            continue;
        }

        if ('A' <= *p && *p <= 'Z') {
            size_t len = strlen(propos);
            bool found = false;
            for (int i = 0; i < len; i++) {
                if (memcmp(&propos[i], p, 1) == 0) {
                    found = true;
                }
            }
            if ( ! found) {
                if (len >= 10) {
                    error("Error: too many proposition");
                }
                strncat(propos, p, 1);
            }
            cur = new_token(TK_IDENT, cur, p++, 1);
            continue;
        }

        error_at(token->str, "Error: failed to tokenize");
    }

    new_token(TK_EOF, cur, p, 0);
    return head.next;
}