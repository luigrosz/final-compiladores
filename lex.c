/**
 * @file lex.c
 * @author Prof. Ivairton M. Santos - UFMT - Computacao
 * @brief Codificacao do modulo do analisador lexico
 * @version 0.3
 * @date 2022-04-25
 *
 */

#include "lex.h"

int pos;
char string[MAX_INPUT];

// Tabela de keywords
typedef struct {
    const char *palavra;
    int tag;
} keyword_entry;

static keyword_entry keywords[] = {
    {"int",    KW_INT},
    {"float",  KW_FLOAT},
    {"string", KW_STRING},
    {"char",   KW_CHAR},
    {"begin",  KW_BEGIN},
    {"end",    KW_END},
    {"if",     KW_IF},
    {"else",   KW_ELSE},
    {"while",  KW_WHILE},
    {"read",   KW_READ},
    {"write",  KW_WRITE},
    {"return", KW_RETURN},
    {NULL,     0}
};

static int busca_keyword(const char *word) {
    for (int i = 0; keywords[i].palavra != NULL; i++) {
        if (strcmp(keywords[i].palavra, word) == 0)
            return keywords[i].tag;
    }
    return -1;
}

void initLex(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Erro ao abrir arquivo: %s\n", filename);
        exit(1);
    }
    int total = 0;
    int c;
    while ((c = fgetc(fp)) != EOF && total < MAX_INPUT - 1) {
        string[total++] = (char)c;
    }
    string[total] = '\0';
    fclose(fp);
    pos = 0;
}

type_token *getToken() {
    char buffer[MAX_CHAR];
    int pos_buffer;
    type_token *token;

    pos_buffer = 0;
    token = (type_token*) malloc(sizeof(type_token));

    // Consome espacos
    while (isspace((unsigned char)string[pos])) {
        pos++;
    }

    // Verifica se identificador ou keyword
    if (isalpha((unsigned char)string[pos]) || string[pos] == '_') {
        while (isalnum((unsigned char)string[pos]) || string[pos] == '_') {
            buffer[pos_buffer++] = string[pos++];
        }
        buffer[pos_buffer] = '\0';

        int kw = busca_keyword(buffer);
        if (kw != -1) {
            token->tag = kw;
        } else {
            token->tag = ID;
        }
        strcpy(token->lexema, buffer);
        return token;
    }

    // Verifica se NUMERO
    if (isdigit((unsigned char)string[pos])) {
        while (isdigit((unsigned char)string[pos])) {
            buffer[pos_buffer++] = string[pos++];
        }
        // Suporte a ponto decimal
        if (string[pos] == '.') {
            buffer[pos_buffer++] = string[pos++];
            while (isdigit((unsigned char)string[pos])) {
                buffer[pos_buffer++] = string[pos++];
            }
        }
        buffer[pos_buffer] = '\0';
        token->tag = NUM;
        strcpy(token->lexema, buffer);
        return token;
    }

    // String literal entre aspas
    if (string[pos] == '"') {
        pos++; // pula aspa de abertura
        while (string[pos] != '"' && string[pos] != '\0') {
            buffer[pos_buffer++] = string[pos++];
        }
        if (string[pos] == '"') pos++; // pula aspa de fechamento
        buffer[pos_buffer] = '\0';
        token->tag = STRING_LIT;
        strcpy(token->lexema, buffer);
        return token;
    }

    // Operadores relacionais e de dois caracteres
    if (string[pos] == '<') {
        pos++;
        if (string[pos] == '=') {
            pos++;
            token->tag = LE;
            strcpy(token->lexema, "<=");
        } else {
            token->tag = LT;
            strcpy(token->lexema, "<");
        }
        return token;
    }
    if (string[pos] == '>') {
        pos++;
        if (string[pos] == '=') {
            pos++;
            token->tag = GE;
            strcpy(token->lexema, ">=");
        } else {
            token->tag = GT;
            strcpy(token->lexema, ">");
        }
        return token;
    }
    if (string[pos] == '=' && string[pos + 1] == '=') {
        pos += 2;
        token->tag = EQ;
        strcpy(token->lexema, "==");
        return token;
    }
    if (string[pos] == '!' && string[pos + 1] == '=') {
        pos += 2;
        token->tag = NEQ;
        strcpy(token->lexema, "!=");
        return token;
    }

    // Tokens simples de um caractere
    switch (string[pos]) {
        case '+':
            token->tag = PLUS;
            strcpy(token->lexema, "+");
            pos++;
            return token;
        case '-':
            token->tag = MINUS;
            strcpy(token->lexema, "-");
            pos++;
            return token;
        case '*':
            token->tag = MULT;
            strcpy(token->lexema, "*");
            pos++;
            return token;
        case '/':
            token->tag = DIV;
            strcpy(token->lexema, "/");
            pos++;
            return token;
        case '(':
            token->tag = OPEN_PAR;
            strcpy(token->lexema, "(");
            pos++;
            return token;
        case ')':
            token->tag = CLOSE_PAR;
            strcpy(token->lexema, ")");
            pos++;
            return token;
        case '{':
            token->tag = OPEN_BRACE;
            strcpy(token->lexema, "{");
            pos++;
            return token;
        case '}':
            token->tag = CLOSE_BRACE;
            strcpy(token->lexema, "}");
            pos++;
            return token;
        case ';':
            token->tag = SEMICOLON;
            strcpy(token->lexema, ";");
            pos++;
            return token;
        case ',':
            token->tag = COMMA;
            strcpy(token->lexema, ",");
            pos++;
            return token;
        case '=':
            token->tag = ASSIGN;
            strcpy(token->lexema, "=");
            pos++;
            return token;
        case '\0':
            token->tag = ENDTOKEN;
            strcpy(token->lexema, "");
            return token;
        default:
            token->tag = ERROR;
            sprintf(token->lexema, "%c", string[pos]);
            pos++;
            return token;
    }
}
