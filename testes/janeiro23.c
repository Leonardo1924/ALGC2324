#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N 6


// Exercicio 1

typedef struct avlnode {
    int valor;
    int bal;
    struct avlnode *esq, *dir;
} *AVLTree;

int max(int a, int b) {
    return a > b ? a : b;
}

int altura(AVLTree a) {
    if (a == NULL) return 0;
    return 1 + max(altura(a->esq), altura(a->dir));
}

int balance(AVLTree a) {
    if (a == NULL) return 0;
    return altura(a->esq) - altura(a->dir);
}

AVLTree rotateLeft(AVLTree a) {
    AVLTree b = a->dir;
    a->dir = b->esq;
    b->esq = a;
    return b;
}

AVLTree rotateRight(AVLTree a) {
    AVLTree b = a->esq;
    a->esq = b->dir;
    b->dir = a;
    return b;
}

AVLTree balanceTree(AVLTree a) {
    if (a == NULL) return NULL;
    a->bal = balance(a);
    if (a->bal == 2) {
        if (balance(a->esq) == -1) a->esq = rotateLeft(a->esq);
        a = rotateRight(a);
    } else if (a->bal == -2) {
        if (balance(a->dir) == 1) a->dir = rotateRight(a->dir);
        a = rotateLeft(a);
    }
    return a;
}

AVLTree buildBalancedTree(int a[], int start, int end) {
    if (start > end) return NULL;

    int mid = (start + end) / 2;
    AVLTree root = malloc(sizeof(struct avlnode));

    root->valor = a[mid];
    root->esq = buildBalancedTree(a, start, mid - 1);
    root->dir = buildBalancedTree(a, mid + 1, end);

    root->bal = balance(root);
    return balanceTree(root);
}

AVLTree build (int a[], int N1){
   return buildHelper(a, 0, N1 - 1);
}


// Exercicio 2

typedef struct minheap {
    int size;
    int *values;
} *MinHeap;

void swap(int v[], int i, int j) {
    int t = v[i];
    v[i] = v[j];
    v[j] = t;
}

void bubbleUp(int i, int h[]){
    int p = (i-1)/2;
    while(i > 0 && h[p] > h[i]){
        swap(h,i,p);
        i = p;
        p = (i-1)/2;
    }
}

int decrease(int x, int y, int h[], int N1){
    for(int i = 0 ; i < N; i++){
        if(h[i] == x){
            h[i] = y;
            bubbleUp(i,h);
        }
    }
    return -1;
}

// Exercicio 3
int dx[] = {-1,0,1,0};
int dy[] = {0,1,0,-1};

int isValid(int x, int y, int visited[N][N]){
    return x >= 0 && x < N && y >= 0 && y < N && !visited[x][y];
}

int dfs(int image[N][N], int visited[N][N], int x, int y){
    int size = 1;
    visited[x][y] = 1;
    for(int i = 0; i < 4; i++){
        int nx = x + dx[i];
        int ny = y + dy[i];
        if(isValid(nx,ny,visited) && image[nx][ny]){
            size += dfs(image, visited, nx, ny);
        }
    }
    return size;
}

int maior(int image[N][N]){
    int visited[N][N] = {0};
    int maxSize = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(image[i][j] && !visited[i][j]){
                maxSize = max(maxSize, dfs(image, visited, i, j));
            }
        }
    }
    return maxSize;
}
