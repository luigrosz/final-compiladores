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
type_ts *tsg;   // Tabela de Simbolos Global

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
    novo->tsl_stack = NULL;  // pilha nula enquanto so houver prototipo
    novo->implementada = 0;
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
    printf("\n--- Tabela de Simbolos de Funcoes (TSF) ---\n");
    while (aux != NULL) {
        printf("Tipo: %s | Lexema: %s | Label: %s | Params: %d\n",
               aux->tipo, aux->lexema, aux->label, aux->num_params);
        for (int i = 0; i < aux->num_params; i++) {
            printf("  Param %d: %s %s\n", i, aux->params[i].tipo, aux->params[i].lexema);
        }
        if (aux->tsl_stack == NULL) {
            printf("  TSL: (nula - apenas prototipo)\n");
        } else {
            int frame_num = 0;
            type_tsl_stack *frame = aux->tsl_stack;
            while (frame != NULL) {
                printf("  TSL [frame %d]:\n", frame_num++);
                type_ts *var = frame->tsl;
                while (var != NULL) {
                    if (var->valor[0] != '\0')
                        printf("    Tipo: %s | Lexema: %s | Valor: %s\n",
                               var->tipo, var->lexema, var->valor);
                    else
                        printf("    Tipo: %s | Lexema: %s\n", var->tipo, var->lexema);
                    var = var->prox;
                }
                frame = frame->prev;
            }
        }
        aux = aux->prox;
    }
    printf("-------------------------------------------\n");
}

// ===================== TABELA DE SIMBOLOS GLOBAL (TSG) =====================

void cadastra_variavel(char *tipo, char *lexema) {
    type_ts *novo = (type_ts *)malloc(sizeof(type_ts));
    strcpy(novo->tipo, tipo);
    strcpy(novo->lexema, lexema);
    strcpy(novo->valor, "");
    novo->prox = tsg;
    tsg = novo;
}

type_ts *busca_variavel(char *lexema) {
    type_ts *aux = tsg;
    while (aux != NULL) {
        if (strcmp(aux->lexema, lexema) == 0)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

void imprime_tsg() {
    type_ts *aux = tsg;
    printf("\n--- Tabela de Simbolos Global (TSG) ---\n");
    while (aux != NULL) {
        printf("Tipo: %s | Lexema: %s\n", aux->tipo, aux->lexema);
        aux = aux->prox;
    }
    printf("---------------------------------------\n");
}

// ===================== TABELA DE SIMBOLOS LOCAL (TSL) =====================

void cadastra_variavel_local(type_ts **tsl, char *tipo, char *lexema, char *valor) {
    type_ts *novo = (type_ts *)malloc(sizeof(type_ts));
    strcpy(novo->tipo, tipo);
    strcpy(novo->lexema, lexema);
    strcpy(novo->valor, valor ? valor : "");
    novo->prox = *tsl;
    *tsl = novo;
}

type_ts *busca_variavel_local(type_ts *tsl, char *lexema) {
    type_ts *aux = tsl;
    while (aux != NULL) {
        if (strcmp(aux->lexema, lexema) == 0)
            return aux;
        aux = aux->prox;
    }
    return NULL;
}

void tsl_push(type_tsf *func, type_ts *tsl) {
    type_tsl_stack *frame = (type_tsl_stack *)malloc(sizeof(type_tsl_stack));
    frame->tsl = tsl;
    frame->prev = func->tsl_stack;
    func->tsl_stack = frame;
}

void tsl_pop(type_tsf *func) {
    if (func->tsl_stack == NULL) return;
    type_tsl_stack *old = func->tsl_stack;
    func->tsl_stack = old->prev;
    free(old);
}
