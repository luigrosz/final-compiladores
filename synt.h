/**
 * @file synt.h
 * @author Prof. Ivairton M. Santos - UFMT - Computacao
 * @brief Modulo do analisado sintatico
 * @version 0.3
 * @date 2022-04-25
 *
 */

#ifndef _SYNT_H_
#define _SYNT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"
#include "lex.h"
#include "gen.h"
#include "symbol.h"

// Variaveis globais
extern type_token *lookahead;

// Prototipos - parser
int match(int token_tag);
int is_tipo(int tag);
void program();
void declaracoes();
void declaracao();
void declaracaoF(char *tipo, char *lexema);
void comandos();
void comando();
void func_call_cmd(char *id);
void atribuicao(char *id);
void cmd_if();
void cmd_while();
void cmd_read();
void cmd_write();
void condicao(char *temp1, char *op, char *temp2);
void func_code();
void func_impl();

// Expressoes
void E(char *temp);
void ER(char *temp_herdado, char *temp);
void T(char *temp);
void TR(char *temp_herdado, char *temp);
void F(char *temp);

#endif //_SYNT_H_
