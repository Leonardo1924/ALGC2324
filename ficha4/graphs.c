#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

#define INF 999999
#define NV 10


typedef struct aresta
{
    int dest;
    int custo;
    struct aresta *prox;
} *LAdj, *GrafoL[NV];

typedef int GrafoM[NV][NV];

typedef struct coord
{
    int l;
    int c;
    int dist;
} coord;

void swap(int v[], int i, int j)
{
    int tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

// Representações

void fromMat(GrafoM in, GrafoL out)
{
    int i, j;
    for (i = 0; i < NV; i++)
    {
        out[i] = NULL;
        for (j = 0; j < NV; j++)
        {
            if (in[i][j] != 0)
            {
                LAdj aux = malloc(sizeof(struct aresta));
                aux->dest = j;
                aux->custo = in[i][j];
                aux->prox = out[i];
            }
        }
    }
}

void inverte2(GrafoL in, GrafoL out)
{
    int i;
    for (i = 0; i < NV; i++)
    {
        out = NULL;
        LAdj aux = in[i];
        while (aux != NULL)
        {
            LAdj novo = malloc(sizeof(struct aresta));
            novo->dest = i;
            novo->custo = aux->custo;
            novo->prox = out;
            out = novo;
            aux = aux->prox;
        }
    }
}

int inDegree(GrafoL g)
{
    int indeg[NV], o, m = 0;
    LAdj aux;

    for (o = 0; o < NV; o++)
    {
        indeg[o] = 0;
    }

    for (o = 0; o < NV; o++)
    {
        for (aux = g[o]; aux != NULL; aux = aux->prox)
        {
            indeg[aux->dest]++;
        }
    }

    for (o = 0; o < NV; o++)
    {
        if (indeg[o] > m)
        {
            m = indeg[o];
        }
    }
}

int colorOk(GrafoL g, int cor[])
{
    int o;
    LAdj aux;

    for (o = 0; o < NV; o++)
    {
        for (aux = g[o]; aux != NULL; aux = aux->prox)
        {
            if (cor[o] == cor[aux->dest])
            {
                return 0;
            }
        }
    }
    return 1;
}

int homomorfism(GrafoL g, GrafoL h, int f[])
{
    int o;
    LAdj aux;

    for (o = 0; o < NV; o++)
    {
        for (aux = g[o]; aux != NULL; aux = aux->prox)
        {
            int a = o;
            int b = aux->dest;
            LAdj aux2;
            for (aux2 = h[f[a]]; aux2 != NULL && aux2->dest != f[b]; aux2 = aux2->prox)
                ;

            if (aux2 == NULL)
            {
                return 0;
            }
        }
    }
    return 1;
}

// TRAVESSIAS
int DF(GrafoL g, int or, int v[], int p[], int l[])
{
    int i;
    for (i = 0; i < NV; i++)
    {
        v[i] = 0;
        p[i] = -1;
        l[i] = -1;
    }
    p[or] = -1;
    l[or] = 0;
    return DFRec(g, or, v, p, l);
}

int DFRec(GrafoL g, int or, int v[], int p[], int l[])
{
    int i;
    LAdj a;
    i = 1;
    v[or] = -1;
    for (a = g[or]; a != NULL; a = a->prox)
        if (!v[a->dest])
        {
            p[a->dest] = or ;
            l[a->dest] = 1 + l[or];
            i += DFRec(g, a->dest, v, p, l);
        }
    v[or] = 1;
    return i;
}

int DFRecWithCicle(GrafoL g, int or, int v[], int p[], int l[], int c[], int *cycleFound)
{
    int i;
    LAdj a;
    v[or] = -1;

    for (a = g[or]; a != NULL; a = a->prox)
    {
        if (!v[a->dest])
        {
            p[a->dest] = or ;
            l[a->dest] = 1 + l[or];
            if (DFRecWithCicle(g, a->dest, v, p, l, c, cycleFound))
            {
                if (*cycleFound == 0)
                {                             // Se ainda não encontramos um ciclo
                    c[*cycleFound] = a->dest; // Adicionamos o vértice ao ciclo
                    (*cycleFound)++;
                }
                if (p[a->dest] != -1 && *cycleFound > 0)
                {                         // Verifica se o ciclo foi fechado
                    c[*cycleFound] = or ; // Adicionamos o vértice de origem ao ciclo
                    (*cycleFound)++;
                    return 1; // Indica que encontramos um ciclo
                }
            }
        }
        else if (v[a->dest] == -1 && p[or] != a->dest && l[a->dest] < l[or])
        {
            // Verifica se encontramos um ciclo
            c[*cycleFound] = a->dest;
            (*cycleFound)++;
            int parent = or ;
            while (parent != a->dest)
            {
                c[*cycleFound] = parent;
                (*cycleFound)++;
                parent = p[parent];
            }
            c[*cycleFound] = a->dest;
            (*cycleFound)++;
            return 1; // Indica que encontramos um ciclo
        }
    }
    v[or] = 1;
    return 0; // Indica que não encontramos um ciclo
}

int BF(GrafoL g, int or, int v[], int p[], int l[])
{
    int i, x;
    LAdj a;
    int q[NV], front, end;
    for (i = 0; i < NV; i++)
    {
        v[i] = 0;
        p[i] = -1;
        l[i] = -1;
    }
    front = end = 0;
    q[end++] = or ; // enqueue
    v[or] = 1;
    l[or] = 0;
    p[or] = -1; // redundante
    i = 1;
    while (front != end)
    {
        x = q[front++]; // dequeue
        for (a = g[x]; a != NULL; a = a->prox)
            if (!v[a->dest])
            {
                i++;
                v[a->dest] = 1;
                p[a->dest] = x;
                l[a->dest] = 1 + l[x];
                q[end++] = a->dest; // enqueue
            }
    }
    return i;
}

int maisLonga(GrafoL g, int or, int p[])
{
    int i, r, max = or ;
    int vis[NV], parent[NV], dist[NV];

    BF(g, or, vis, parent, dist);

    for (i = 0; i < NV; i++)
    {
        if (dist[i] > dist[max])
        {
            max = i;
        }
    }

    r = dist[max];

    while (p[max] != -1)
    {
        p[dist[max]] = max;
        max = parent[max];
    }

    return r;
}

int componentes(GrafoL g, int c[])
{
    int i, j, next = 0;
    int vis[NV], parent[NV], dist[NV];
    for (i = 0; i < NV; i++)
    {
        c[i] = 0;
    }

    for (i = 0; i < NV; i++)
    {
        BF(g, next, vis, parent, dist);

        for (j = 0, next = NV; j < NV; j++)
        {
            if (vis[j])
                c[j] = i + 1;
            else if (c[j] == 0 && j < next)
                next = j;
        }
    }
    return i;
}

void DFOrdTop(GrafoL g, int v, int visitado[], int *indice, int ord[])
{
    LAdj aux;
    visitado[v] = 1;
    for (aux = g[v]; aux != NULL; aux = aux->prox)
    {
        if (!visitado[aux->dest])
        {
            DFOrdTop(g, aux->dest, visitado, indice, ord);
        }
    }
    ord[(*indice)++] = v;
}

int ordTop(GrafoL g, int ord[])
{
    int visitado[NV], i, indice = 0;

    for (i = 0; i < NV; i++)
    {
        visitado[i] = 0;
    }

    for (i = 0; i < NV; i++)
    {
        if (!visitado[i])
        {
            DFOrdTop(g, i, visitado, &indice, ord);
        }
    }
}

int ciclo(GrafoL g, int c[])
{
    int i, cycleFound = 0;
    int vis[NV], parent[NV], dist[NV];
    for (i = 0; i < NV; i++)
    {
        vis[i] = 0;
        parent[i] = -1;
        dist[i] = -1;
    }

    for (i = 0; i < NV; i++)
    {
        if (!vis[i] && DFRecWithCicle(g, i, vis, parent, dist, c, &cycleFound))
        {
            return 1;
        }
    }
    return 0;
}

coord newC(int l, int c)
{
    coord n = {l, c, 0};
    return n;
}

int equals(coord a, coord b)
{
    return a.l == b.l && a.c == b.c;
}

int valid(coord a, int L, int C, char mapa[L][C])
{
    return a.l >= 0 && a.l < L && a.c >= 0 && a.c < C && mapa[a.l][a.c] == ' ';
}

int caminho(int L, int C, char *mapa[L], int ls, int cs, int lf, int cf)
{
    int front, end, vis[L][C];
    int i, j;
    coord queue[2 * L * C], curr, dest;
    front = end = 0;
    dest = newC(lf, cf);
    queue[end++] = newC(ls, cs);

    for (i = 0; i < L; i++)
    {
        for (j = 0; j < C; j++)
        {
            vis[i][j] = 0;
        }
    }

    while (front != end)
    {
        curr = queue[front++];
        if (equals(curr, dest))
        {
            return curr.dist;
        }
        if (valid(newC(curr.l + 1, curr.c), L, C, mapa) && !vis[curr.l + 1][curr.c])
        {
            vis[curr.l + 1][curr.c] = 1;
            queue[end++] = newC(curr.l + 1, curr.c);
        }
        if (valid(newC(curr.l - 1, curr.c), L, C, mapa) && !vis[curr.l - 1][curr.c])
        {
            vis[curr.l - 1][curr.c] = 1;
            queue[end++] = newC(curr.l - 1, curr.c);
        }
        if (valid(newC(curr.l, curr.c + 1), L, C, mapa) && !vis[curr.l][curr.c + 1])
        {
            vis[curr.l][curr.c + 1] = 1;
            queue[end++] = newC(curr.l, curr.c + 1);
        }
        if (valid(newC(curr.l, curr.c - 1), L, C, mapa) && !vis[curr.l][curr.c - 1])
        {
            vis[curr.l][curr.c - 1] = 1;
            queue[end++] = newC(curr.l, curr.c - 1);
        }
    }
    return -1;
}

// Algoritmos sobre Grafos Pesados

int minIndPeso(int v[], int pesos[], int tam)
{
    int i, min = 0;
    for (i = 1; i < tam; i++)
        if (pesos[v[i]] < pesos[v[min]])
            min = i;
    return min;
}

int dijkstraSP(GrafoL g, int or, int pais[], int pesos[])
{
    int r, i, v, cor[NV], orla[NV], tam;
    LAdj x;
    // inicializacoes
    for (i = 0; i < NV; i++)
    {
        pais[i] = -2;
        cor[i] = 0; // nao visitado
    }
    r = 0;
    orla[0] = or ;
    tam = 1;
    pesos[or] = 0;
    pais[or] = -1;
    cor[or] = 1; // na orla
    // ciclo
    while (tam > 0)
    {
        // seleccionar vertice de menor peso
        i = minIndPeso(orla, pesos, tam);
        swap(orla, i, --tam);
        v = orla[tam];
        r++;
        cor[v] = 2; // visitado
        for (x = g[v]; x != NULL; x = x->prox)
        {
            if (cor[x->dest] == 0)
            {
                cor[x->dest] = 1;
                orla[tam++] = x->dest;
                pais[x->dest] = v;
                pesos[x->dest] = pesos[v] + x->custo;
            }
            else if (cor[x->dest] == 1 && pesos[v] + x->custo < pesos[x->dest])
            {
                pais[x->dest] = v;
                pesos[x->dest] = pesos[v] + x->custo;
            }
        }
    }
    return r;
}

int excentricidadeV(GrafoL g, int v)
{
    int i, pais[NV], pesos[NV];
    dijkstraSP(g, v, pais, pesos);
    int max = 0;
    for (i = 0; i < NV; i++)
    {
        if (pesos[i] > max)
        {
            max = pesos[i];
        }
    }
    return max;
}

int dijkstra_for_cars(GrafoL g , int or , int pais[], int pesos[], int autonomia)
{
    int r, i, v, cor[NV], orla[NV], tam;
    LAdj x;
    // inicializacoes
    for (i = 0; i < NV; i++)
    {
        pais[i] = -2;
        cor[i] = 0; // nao visitado
    }
    r = 0;
    orla[0] = or ;
    tam = 1;
    pesos[or] = 0;
    pais[or] = -1;
    cor[or] = 1; // na orla
    // ciclo
    while (tam > 0)
    {
        // seleccionar vertice de menor peso
        i = minIndPeso(orla, pesos, tam);
        swap(orla, i, --tam);
        v = orla[tam];
        r++;
        cor[v] = 2; // visitado
        for (x = g[v]; x != NULL; x = x->prox)
        {
            if (x->custo > autonomia) // se o custo for maior que a autonomia, continue para a próxima iteração
                continue;
            if (cor[x->dest] == 0)
            {
                cor[x->dest] = 1;
                orla[tam++] = x->dest;
                pais[x->dest] = v;
                pesos[x->dest] = pesos[v] + x->custo;
            }
            else if (cor[x->dest] == 1 && pesos[v] + x->custo < pesos[x->dest])
            {
                pais[x->dest] = v;
                pesos[x->dest] = pesos[v] + x->custo;
            }
        }
    }
    return r;
}

int prim(GrafoL g, int or, int pais[], int pesos[]) {
    int i, v, cor[NV], orla[NV], tam;
    LAdj x;
    // inicializacoes
    for (i = 0; i < NV; i++) {
        pais[i] = -2;
        cor[i] = 0; // nao visitado
        pesos[i] = INT_MAX; // set initial weights to maximum
    }
    orla[0] = or;
    tam = 1;
    pesos[or] = 0;
    pais[or] = -1;
    cor[or] = 1; // na orla
    // ciclo
    while (tam > 0) {
        // seleccionar vertice de menor peso
        i = minIndPeso(orla, pesos, tam);
        swap(orla, i, --tam);
        v = orla[tam];
        cor[v] = 2; // visitado
        for (x = g[v]; x != NULL; x = x->prox) {
            if (cor[x->dest] == 0 && x->custo < pesos[x->dest]) {
                cor[x->dest] = 1;
                orla[tam++] = x->dest;
                pais[x->dest] = v;
                pesos[x->dest] = x->custo;
            }
        }
    }
    // calculate the total weight of the minimum spanning tree
    int totalWeight = 0;
    for (i = 0; i < NV; i++) {
        if (pais[i] != -2) {
            totalWeight += pesos[i];
        }
    }
    return totalWeight;
}

void floydWarshall (int graph[NV][NV]){
    int dist[NV][NV], i, j, k;
    for (i = 0; i < NV; i++)
        for(j = 0 ; j < NV; j++)
            dist[i][j] = graph[i][j];

    for (k = 0; k < NV; k++){
        for ( i = 0; i < NV; i++){
            for(j = 0; j < NV; j++){
                if (dist[i][k] + dist[k][j] < dist[i][j])
                    dist[i][j] = dist[i][k] + dist[k][j];
            }
        }
    }
}

int diametro(GrafoL g){
    int max = -INF;
    floydWarshall(g);
    for(int i = 0; i < NV; i++){
        for(int j = 0; j < NV; j++){
            if(i != j && g[i][j].custo > max) // Access the `custo` member of the `g[i][j]` struct.
                max = g[i][j].custo;
        }
    }
    return max;
}