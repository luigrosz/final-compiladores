/**
 * @file symbol.h
 * @brief Modulo de manipulacao das tabelas de simbolos (TS e TSF)
 * @version 0.3
 * @date 2022-04-25
 *
 */

#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct_compiler.h"

// Variaveis globais
extern type_tsf *tsf;
extern type_ts *tsg;   // Tabela de Simbolos Global

// Tabela de Simbolos de Funcoes (TSF)
void cadastra_funcao(char *tipo, char *lexema, type_param *params, int num_params);
type_tsf *busca_funcao(char *lexema);
void imprime_tsf();

// Tabela de Simbolos Global (TSG)
void cadastra_variavel(char *tipo, char *lexema);
type_ts *busca_variavel(char *lexema);
void imprime_tsg();

// Tabela de Simbolos Local (TSL) — por funcao
void cadastra_variavel_local(type_ts **tsl, char *tipo, char *lexema);
type_ts *busca_variavel_local(type_ts *tsl, char *lexema);

#endif //_SYMBOL_H_
