#include <stdio.h>
#include <stdlib.h>
#include "lib_lista.h"

int inicializa_lista(t_lista *l) {

    t_nodo *first, *last;
    first = (t_nodo *) malloc(sizeof(t_nodo));
    if (first == NULL)
        return 0;
    last = (t_nodo *) malloc(sizeof(t_nodo));
    if (last == NULL) {
        free (first);
        return 0;
    }

    l->ini = first;
    first->prox = last;
    first->prev = NULL;
    l->fim = last;
    last->prox = NULL;
    last->prev = first;
    l->tamanho = 0;
    l->atual = NULL;

    return 1;
}

int tamanho_lista(int *tam, t_lista *l) {

    *tam = l->tamanho;
    return 1;
}

int insere_lista(t_nodo *nodo, t_lista *l) {

    nodo->prox = l->fim;
    nodo->prev = l->fim->prev;
    l->fim->prev->prox = nodo;
    l->fim->prev = nodo;
    l->tamanho++;

    return 1;
}

int inicializa_atual_inicio(t_lista *l) {

    l->atual = l->ini->prox;
    return 1;
}

int inicializa_atual_fim(t_lista *l) {

    l->atual = l->fim->prev;
    return 1;
}


void incrementa_atual(t_lista *l) {

    if (l->atual == l->fim->prev)
        return;

    l->atual = l->atual->prox;
}

void decrementa_atual(t_lista *l) {

    if (l->atual == l->ini->prox)
        return;

    l->atual = l->atual->prev;
}

int remove_item_atual(t_lista *l) {

    t_nodo *aux;
    aux = l->atual;

    l->atual->prev->prox = l->atual->prox;
    l->atual->prox->prev = l->atual->prev;

    if (l->atual->prox == l->fim) 
        l->atual = l->atual->prev;
    else 
        l->atual = l->atual->prox;

    free(aux);
    l->tamanho--;

    return 1;
}