/**
 * @file struct_compiler.h
 * @author Prof. Ivairton M. Santos - UFMT - Computacao
 * @brief Modulo para definicao de constantes e estruturas de dados do Sistema
 *        Compilador de C Simplificado
 * @version 0.3
 * @date 2022-04-25
 *
 */

#ifndef _STRUCT_COMPILER_
#define _STRUCT_COMPILER_

// Definicao de constantes para todo o compilador
#define MAX_CHAR 256
#define MAX_TOKEN 256
#define MAX_PARAMS 16

#define true 1  //por conveniencia, para facilitar leitura de codigo
#define false 0 //por conveniencia, para facilitar leitura de codigo

// Definicao dos codigos
#define ENDTOKEN '\0'
#define ERROR -1
#define NUM 1
#define PLUS '+'
#define MINUS '-'
#define MULT '*'
#define DIV '/'
#define OPEN_PAR '('
#define CLOSE_PAR ')'
#define SEMICOLON ';'
#define COMMA ','
#define ASSIGN '='
#define OPEN_BRACE '{'
#define CLOSE_BRACE '}'

// Tokens de keywords (valores altos para nao colidir com ASCII)
#define KW_INT 200
#define KW_FLOAT 201
#define KW_STRING 202
#define KW_CHAR 203
#define KW_BEGIN 204
#define KW_END 205
#define KW_IF 206
#define KW_ELSE 207
#define KW_WHILE 208
#define KW_READ 209
#define KW_WRITE 210
#define ID 211

// Operadores relacionais
#define LT 220
#define GT 221
#define LE 222
#define GE 223
#define EQ 224
#define NEQ 225

// String literal
#define STRING_LIT 230

// Definicao da estrutura de dados 'token'
struct st_token {
    int tag;
    char lexema[MAX_CHAR];
};
typedef struct st_token type_token;

// Definicao da estrutura de codigo intermediario
struct st_code {
    char code[2048];
    char temp[32];
};
typedef struct st_code type_code;

// Estrutura de parametros de funcao
struct st_param {
    char tipo[MAX_CHAR];
    char lexema[MAX_CHAR];
};
typedef struct st_param type_param;

// Tabela de Simbolos de Funcoes
struct st_tsf {
    char tipo[MAX_CHAR];
    char lexema[MAX_CHAR];
    char label[MAX_CHAR];
    type_param params[MAX_PARAMS];
    int num_params;
    struct st_tsf *prox;
};
typedef struct st_tsf type_tsf;

// Tabela de Simbolos de Variaveis
struct st_ts {
    char tipo[MAX_CHAR];
    char lexema[MAX_CHAR];
    struct st_ts *prox;
};
typedef struct st_ts type_ts;

#endif //_STRUCT_COMPILER_
