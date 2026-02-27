/**
 * @file symbol.c
 * @brief Codificacao do modulo de manipulacao das tabelas de simbolos (TS e TSF)
 * @version 0.3
 * @date 2022-04-25
 *
 */

#include "symbol.h"

// Definicao das variaveis globais
type_tsf *tsf;
type_ts *ts;

// ===================== TABELA DE SIMBOLOS - FUNCOES (TSF) =====================

void cadastra_funcao(char *tipo, char *lexema, type_param *params, int num_params) {
    type_tsf *novo = (type_tsf *)malloc(sizeof(type_tsf));
    strcpy(novo->tipo, tipo);
    strcpy(novo->lexema, lexema);
    sprintf(novo->label, "func_%s", lexema);
    novo->num_params = num_params;
    for (int i = 0; i < num_params; i++) {
        strcpy(novo->params[i].tipo, params[i].tipo);
        strcpy(novo->params[i].lexema, params[i].lexema);
    }
    novo->prox = tsf;
    tsf = novo;
}

type_tsf *busca_funcao(char *lexema) {
    type_tsf *aux = tsf;
    while (aux != NULL) {
        if (strcmp(aux->lexema, lexema) == 0)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

void imprime_tsf() {
    type_tsf *aux = tsf;
    printf("\n--- Tabela de Simbolos de Funcoes ---\n");
    while (aux != NULL) {
        printf("Tipo: %s | Lexema: %s | Label: %s | Params: %d\n",
               aux->tipo, aux->lexema, aux->label, aux->num_params);
        for (int i = 0; i < aux->num_params; i++) {
            printf("  Param %d: %s %s\n", i, aux->params[i].tipo, aux->params[i].lexema);
        }
        aux = aux->prox;
    }
    printf("-------------------------------------\n");
}

// ===================== TABELA DE SIMBOLOS - VARIAVEIS (TS) =====================

void cadastra_variavel(char *tipo, char *lexema) {
    type_ts *novo = (type_ts *)malloc(sizeof(type_ts));
    strcpy(novo->tipo, tipo);
    strcpy(novo->lexema, lexema);
    novo->prox = ts;
    ts = novo;
}

type_ts *busca_variavel(char *lexema) {
    type_ts *aux = ts;
    while (aux != NULL) {
        if (strcmp(aux->lexema, lexema) == 0)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

void imprime_ts() {
    type_ts *aux = ts;
    printf("\n--- Tabela de Simbolos de Variaveis ---\n");
    while (aux != NULL) {
        printf("Tipo: %s | Lexema: %s\n", aux->tipo, aux->lexema);
        aux = aux->prox;
    }
    printf("---------------------------------------\n");
}
