#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MAX 100
typedef struct pQueue{
    int valores[MAX];
    int tamanho;
} PriorityQueue;

void swap(int v[], int i, int j){
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}

int parent(int i){
    return (i-1) / 2;
}

int left(int i){
    return 2*i + 1;
}

int right(int i){
    return 2*i + 2;
}

void bubbleUp (int i, int h[]){
    int p = parent(i);
    while(i > 0 && h[p] > h[i]){
        swap(h,i,p);
        i = p;
        p = parent(i);
    }
}

void bubbleDown(int i, int h[], int N){
   int f = left(i);
    while(f < N){
         if(f+1 < N && h[f+1] < h[f]) f = f+1;
         if(h[i] <= h[f]) break;
         swap(h,i,f);
         i = f;
         f = left(i);
    }
}

int empty (PriorityQueue *q){
    return q->tamanho = 0;
}

int isEmpty(PriorityQueue *q){
    return (q->tamanho == 0);
}

int add(int x, PriorityQueue *q){
    if (q -> tamanho == MAX) return 1;

    q->valores[q->tamanho] = x;
    bubbleUp(q->tamanho, q->valores);
    q->tamanho++;

    return 0;
}

int removeElement(PriorityQueue *q, int *rem){
    if (q->tamanho == 0) return 1;

    *rem = q->valores[0];
    q->valores[0] = q->valores[--q->tamanho];
    bubbleDown(0, q->valores, q->tamanho);
    return 0;
}

void heapify_topdown(int v[], int N){
    int p = 0;
    while (p < N) {
        bubbleUp(p, v);
        p++;
    }
}

void heapify_bottomup(int v[], int N){
    int p = N-1;
    while (p >= 0) {
        bubbleDown(p, v, N);
        p--;
    }
}

void ordenaHeap(int h[], int N){
    heapify_bottomup(h, N);
    for(int i = N-1; i > 0; i--){
        swap(h, 0, i);
        bubbleDown(0, h, i);
    }
}    

int decrease(int x, int y, int h[], int N){
    for(int i = 0; i < N; i++){
        if(h[i] == x){
            h[i] = y;
            while(i > 0 && h[parent(i)] > h[i]){
                swap(h,parent(i),i);
                i = parent(i);
            }
            return 0;
        }
    }
    return -1;
}


void print_heap(int h[], int N){
    int i = 0;
    int j = 0;
    int k = 0;
    while(i < N){
        for(j = 0; j < pow(2,k) && i < N; j++){
            printf("%d ", h[i]);
            i++;
        }
        printf("\n");
        k++;
    }
}


// main para testar o código acima
int main(int argc, char const *argv[]){
    PriorityQueue *q = malloc(sizeof(PriorityQueue));
    q->tamanho = 0;
    add(10, q);
    add(5, q);
    add(7, q);
    add(1, q);
    add(3, q);
    add(4, q);
    add(6, q);
    add(2, q);
    add(8, q);
    add(9, q);
    //Fazer print da heap como uma arvore binaria
    print_heap(q->valores, q->tamanho);

    printf("\n");

    //Remover o elemento minimo
    printf("Remove min\n");
    int rem;
    removeElement(q, &rem);
    printf("Nova Heap:\n");
    print_heap(q->valores, q->tamanho);

    printf("\n");

    ordenaHeap(q->valores, q->tamanho);
    printf("Heap ordenada:\n");
    print_heap(q->valores, q->tamanho);
}