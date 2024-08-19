#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct item {
    int value;
    struct item* next;
};

struct hashTable {
    unsigned int m; // Tamanho da tabela
    unsigned int n; // Quantidade de Elementos
    struct item** items;
};

unsigned int func_hash(int value, int m) {
    return abs(value % m);
}

void insert(struct hashTable* hashTable, int value);
void reHash(struct hashTable* hashTable);
struct item* search(struct hashTable* hashTable, int value);
void delete_table(struct hashTable* hashTable);

// Função Re-hash - Para aumentar a quantidade de espaços na tabela e evitar que o tempo não seja constante
void reHash(struct hashTable* hashTable) {
    // Pegando a lista anteriormente salva
    unsigned int old_m = hashTable->m;
    struct item** old_items = hashTable->items;

    // Reconfigurar Tabela e Dobrar o Tamanho
    hashTable->m *= 2;  
    hashTable->n = 0;
    hashTable->items = (struct item**)malloc(sizeof(struct item*) * hashTable->m);

    // Nulificando endereços da lista dentro da tabela
    for(int i = 0; i < hashTable->m; i++) {
        hashTable->items[i] = NULL;
    }

    // Inserindo antigos valores na lista
    for(int i = 0;i < old_m; i++) {
        struct item* aux = old_items[i];
        while(aux != NULL) {
            // Inserção dos elementos
            insert(hashTable, aux->value);

            // Liberar memória alocada
            struct item* toFree = aux;
            aux = aux->next;
            free(toFree);
        }
    }
    free(old_items);
}

// Calcula o fator de carga
float loadFactor(struct hashTable* hashTable) {
    return (float)hashTable->n / (float)hashTable->m;
}

// Criando um novo item
struct item* create_item(int value) {
    struct item* item = (struct item*)malloc(sizeof(struct item));
    item->value = value;
    item->next = NULL;
    return item;
}

// Inicialização de Tabela Hash
struct hashTable* create_table(unsigned int m) {
    struct hashTable* hashTable = (struct hashTable*)malloc(sizeof(struct hashTable));
    hashTable->m = m;
    hashTable->n = 0;
    hashTable->items = (struct item**)malloc(sizeof(struct item*) * m);
    for(int i = 0; i < m; i++) {
        hashTable->items[i] = NULL;
    }
    return hashTable;
}

void insert_item(struct item* item, int value) {
    if (item->next != NULL)
        insert_item(item->next, value);
    else
        item->next = create_item(value);
}

void insert(struct hashTable* hashTable, int value) {
    unsigned int key = func_hash(value, hashTable->m);
    
    if (loadFactor(hashTable) < 1) {
        if(hashTable->items[key] == NULL) {
            hashTable->items[key] = create_item(value);
        } else {
            insert_item(hashTable->items[key], value);
        }
        hashTable->n++;
    } else {
        reHash(hashTable);
        insert(hashTable, value);
    }
}

struct item* search_item(struct item* item, int value) {
    if (item != NULL) {
        if (item->value == value) {
            return item;
        }
        search_item(item->next, value);
    }
    return NULL;
}

// Procurando elementos na tabela Hash
struct item* search(struct hashTable* hashTable, int value) {
    unsigned int key = func_hash(value, hashTable->m); 
    return search_item(hashTable->items[key], value);
}

// Excluindo elementos na tabela Hash
void delete_table(struct hashTable* hashTable) {
    for(int i=0; i < hashTable->m; i++) {
        struct item* current_item = hashTable->items[i];

        while(current_item != NULL) {
            struct item* aux_item = current_item;
            current_item = current_item->next;
            free(aux_item);
        }
    }

    free(hashTable->items);
    free(hashTable);
}

// Printar a tabela completa
void printstruct(struct hashTable* hashTable) {
    for(int i = 0; i < hashTable->m; i++) {
        printf("[%d] -> ", i);
        struct item* temp = hashTable->items[i];
        while(temp != NULL) {
            printf("(%d) -> ", temp->value);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main(int argc, char **argv) {
    // Quantidade de elementos na tabela
    unsigned int n = atoi(argv[1]);
    
    struct hashTable* hashTable = create_table(n);

    srand(time(NULL));

    for(int i = 0; i < n; i++) {
        insert(hashTable, i);
    }

    struct timespec start, end;
    int v = (rand() % n) - 1;
    clock_gettime(CLOCK_MONOTONIC, &start);
    struct item* item = search(hashTable, v);
    clock_gettime(CLOCK_MONOTONIC, &end);

    unsigned  time = (end.tv_sec * 1e9 + end.tv_nsec) - (start.tv_sec * 1e9 + start.tv_nsec);

    printf("%u\n", time);

    delete_table(hashTable);
    return 0;
}