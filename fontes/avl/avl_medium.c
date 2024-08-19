#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

struct node {
    int v;
    unsigned int h;
    struct node* l;
    struct node* r;
    struct node* p;
};

struct node* new_node(int v) {
    struct node* n = (struct node*)malloc(sizeof(struct node));
    n->h = 0;
    n->v = v;
    n->l = NULL;
    n->r = NULL;
    n->p = NULL;
    return n;
}

int getHeight(struct node *n) {
    if (n == NULL) 
        return 0;
    return n->h;
}

void update_height(struct node** r) {
    int hl = getHeight((*r)->l);
    int hr = getHeight((*r)->r);
    (*r)->h = hl > hr ? hl + 1: hr + 1;
}

void rotateRight(struct node** n) {
    struct node* aux = (*n);
    (*n) = (*n)->l;
    (*n)->p = aux->p;
    aux->p = (*n);
    aux->l = (*n)->r;
    (*n)->r = aux;
    update_height(&aux);
    
}

void rotateLeft(struct node** n) {
    struct node* aux = (*n);
    (*n) = (*n)->r;
    (*n)->p = aux->p;
    aux->p = (*n);
    aux->r = (*n)->l;
    (*n)->l = aux;
    update_height(&aux);
    update_height(n);
}

void avl(struct node** r) {
    int hl = getHeight((*r)->l);
    int hr = getHeight((*r)->r);
    if (abs(hl - hr) > 1) { // desbalanceado
        if (hl > hr) {
            if (getHeight((*r)->l->l) > getHeight((*r)->l->r)) {
                // CASO 2
                rotateRight(r);
            } else {
                // CASO 4
                rotateLeft(&((*r)->l));
                rotateRight(r);
            }
        } else {
            if (getHeight((*r)->r->r) > getHeight((*r)->r->l)) {
                // CASO 1
                rotateLeft(r);
            } else {
                // CASO 3
                rotateRight(&((*r)->r));
                rotateLeft(r);
            }
        }       
    }
}

void btinsert(struct node** r, struct node* n) {
    if((*r) != NULL) {
        if ((*r)->v > n->v) {
            btinsert(&((*r)->l), n);
            (*r)->l->p = (*r);
        } else {
            btinsert(&((*r)->r), n);
            (*r)->r->p = (*r);
        }
    } else {
        *r = n;
    }
    update_height(r);
    avl(r);
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

void btinorder(struct node* r) {
    if (r != NULL) {
        btinorder(r->l);
        printf("%d ", r->v);
        btinorder(r->r);
    }
}

int main(int argc, char **argv) {
    struct node* r = NULL;
    unsigned int n = atoi(argv[1]);
    srand(time(NULL));

    unsigned int v = (rand() % n);
    for (int i = 0; i < n; i++) {
        btinsert(&r, new_node(i));
    }
    // Inicializando variáveis de tempo
    struct timespec start, end;
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