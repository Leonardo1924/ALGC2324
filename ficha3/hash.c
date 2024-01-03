#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Size 100
#define Free 0
#define Used 1
#define Del 2

// Hash Table Chainning

typedef struct nodo{
    char *key;
    int ocorr;
    struct nodo *prox;
} Nodo, *THash[Size];

unsigned int hash(char *s){
    unsigned hash = 5381;
    int c;
    while(c = *s++)
        hash = ((hash << 5) + hash) + c;
    return hash % Size;
}

void initEmpty(THash t){
    int i;
    for(i = 0; i < Size; i++)
        t[i] = NULL;
}

void add(char *s, THash t) {
    int h = hash(s);
    Nodo *aux;
    for (aux = t[h]; aux != NULL; aux = aux->prox) {
        if (strcmp(aux->key, s) == 0) {
            aux->ocorr++;
            return;
        }
    }
    aux = malloc(sizeof(Nodo));
    aux->key = strdup(s);
    aux->ocorr = 1;
    aux->prox = t[h];
    t[h] = aux;
}

int lookup (char *s, THash t){
    int h = hash(s);
    int found;
    Nodo *aux;
    for(aux = t[h]; aux != NULL && strcmp(aux->key,s) != 0; aux = aux->prox);
    if (aux != NULL){
        found = 1;
        printf("Found %s with %d occurrences\n",aux->key,aux->ocorr);
    }
    else{
        found = 0;
        printf("The key %s was not found\n",s);
    }
    return found;
}

int removeOcorr(char *s, THash t){
    int h = hash(s);
    int removed = 0;
    Nodo *aux, *prev;
    for(aux = t[h], prev = NULL; aux != NULL && strcmp(aux->key,s) != 0; prev = aux, aux = aux->prox);
    if (aux != NULL){
        if (aux->ocorr > 1){
            aux->ocorr--;
            removed = 1;
        }
        else{
            if (prev == NULL)
                t[h] = aux->prox;
            else
                prev->prox = aux->prox;
            free(aux);
            removed = 1;
        }
    }
    return removed;
}

void print(THash t){
    int i;
    Nodo *aux;
    for(i = 0; i < Size; i++){
        for(aux = t[i]; aux != NULL; aux = aux->prox)
            printf("%s %d\n",aux->key,aux->ocorr);
    }
}

// Hash Table Open Addressing

typedef struct bucket{
    int status;
    char *key;
    int ocorr;
} THashOA[Size];

int where (char *s, THashOA t){
    int c, hash = 5381;
    while(c = *s++) 
        hash *= 33 + c;
    
    return hash % Size;
}

void initEmptyOA(THashOA t){
    int i;
    for(i = 0; i < Size; i++){
        t[i].status = Free;
        t[i].key = NULL;
        t[i].ocorr = 0;
    }
}

void add (char *s, THashOA t){
    int h = where(s,t);
    int i;
    for(i = h; t[i].status != Free && strcmp(t[i].key,s) != 0; i = (i+1) % Size);
    if(t[i].status == Free){
        t[i].status = Used;
        t[i].key = strdup(s);
        t[i].ocorr = 1;
    }
}

int lookup (char *s, THashOA t){
    int h = where(s,t);
    int found = 0;
    int i;
    for(i = h; t[i].status != Free && strcmp(t[i].key,s) != 0; i = (i+1) % Size);
    if(t[i].status == Used){
        found = 1;
        printf("Found %s with %d occurrences\n",t[i].key,t[i].ocorr);
    }
    else
        printf("The key %s was not found\n",s);
    return found;
}

int removeOcorr(char *s, THashOA t){
    int h = where(s,t);
    int removed = 0;
    int i;
    for(i = h; t[i].status != Free && strcmp(t[i].key,s) != 0; i = (i+1) % Size);
    if(t[i].status == Used){
        if(t[i].ocorr > 1){
            t[i].ocorr--;
            removed = 1;
        }
        else{
            t[i].status = Del;
            free(t[i].key);
            removed = 1;
        }
    }
}

void copy(char *s, int ocorr, THashOA t){
    int i;
    for(i = where(s,t); t[i].status == Used; i = (i+1) % Size);
    t[i].status = Used;
    strcpy(t[i].key,s);
    t[i].ocorr = ocorr;
}

int garb_collection(THashOA t){
    char *keys[Size];
    int i, c = 0, ocorr[Size];
    for(i = 0; i < Size; i++){
        if(t[i].status = Used){
            strcpy(keys[c],t[i].key);
            ocorr[c] = t[i].ocorr;
            c++;
        } 
    }

    initEmptyOA(t);

    for(i = 0; i < c; i++)
        copy(keys[i],ocorr[i],t);
    
    return 0;
}


typedef struct bucket{
    int status;
    char *key;
    int ocorr;
    int collisions;
} THash5[Size];

void add(char *s, THash5 t){
    int h = where(s,t);
    int i;
    for(i = h; t[i].status != Free && strcmp(t[i].key,s) != 0; i = (i+1) % Size);
    if(t[i].status == Free){
        t[i].status = Used;
        t[i].key = strdup(s);
        t[i].ocorr = 1;
        t[i].collisions = 0;
    }
    else{
        t[i].ocorr++;
        t[i].collisions++;
    }
}



int main(int argc, char const *argv[]){
    THash t;
    initEmpty(t);
    add("ola",t);
    add("ola",t);
    add("ola",t);
    add("ola",t);
    add("teste",t);
    add("teste",t);
    add("ultimate",t);

    print(t);

    printf("\n");
    lookup("ultimate",t);
    printf("\n");
    lookup("algc",t);
    printf("\n");

    printf("\n");
    printf("Removed Code: %d\n",removeOcorr("ultimate",t));
    printf("\n");
    printf("Removed Code: %d\n",removeOcorr("ola",t));

    print(t);
}