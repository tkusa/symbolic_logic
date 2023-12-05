#include <stdio.h>
#include <string.h>
#include <math.h>
#include "logic.h"

char *user_input;
Token *token;
char propos[10] = {'\0'};
int graph[1024][10] = {0};

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Error: args\n");
        return 1;
    }

    user_input = argv[1];
    token = tokenize(user_input);
    size_t cnt = strlen(propos);
    permutate(cnt);
    Node *node = logic();
    int row_len = pow(2, cnt);
    printf("%s : %s\n", propos, user_input);
    for (int i = 0; i < row_len; i++) {
        int result = evaluate(node, graph[i]);
        for (int j = 0; j < cnt; j++) {
            printf("%d", graph[i][j]);
        }
        printf(" : %d\n", result);
    } 
}
