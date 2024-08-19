// Inserção em árvore binária
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
struct node {
    int v;
    struct node* l;
    struct node* r;
};

struct node* new_node(int v) {
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->v = v;
    n->l = NULL;
    n->r = NULL;
    return n;    
}

void btinsert(struct node** r, struct node* n) {
    if (*r != NULL) {
        if ((*r)->v > n->v)
            return btinsert(&(*r)->l, n);
        else
            return btinsert(&(*r)->r, n);
    }
    (*r) = n;
}


// busca binária
struct node* search(struct node* r, int v) {
    if (r != NULL) {
        if (v < r->v)
            return search(r->l, v);
        if (r->v < v)
            return search(r->r, v);
    }
    return r;
}

int main(int argc, char **argv) {
    struct node* r = NULL;
    unsigned int n = atoi(argv[1]);
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        btinsert(&r, new_node(rand() % (n*10)));
    }
    // Inicializando variáveis de tempo
    struct timespec start, end;
    int v = rand() % (n*10);
    // Guardando valores de tempo
    clock_gettime(CLOCK_MONOTONIC, &start);
    struct node* aux = search(r, v);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // Calculando o tempo
    unsigned  time = (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);
    // Mostrando o tempo para iterate
    printf("%u\n", time);
    return 0;
}