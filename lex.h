/**
 * @file lex.h
 * @author Prof. Ivairton M. Santos
 * @brief Modulo do analisador lexico
 * @version 0.3
 * @date 2022-04-25
 *
 */

#ifndef _LEX_H_
#define _LEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"

#define MAX_INPUT 8192

// Variaveis globais
extern int pos;
extern char string[MAX_INPUT];

// Prototipos
void initLex(const char *filename);
type_token *getToken();

#endif //_LEX_H_
