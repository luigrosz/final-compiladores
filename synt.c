/**
 * @file synt.c
 * @author Prof. Ivairton M. Santos - UFMT - Computacao
 * @brief Codificacao do modulo do analisador sintatico COM GERACAO DE CODIGO INTERMEDIARIO
 * @version 0.3
 * @date 2022-04-25
 *
 */

#include "synt.h"

// Definicao das variaveis globais
type_token *lookahead;
type_tsf *current_func = NULL;  // funcao cujo corpo esta sendo analisado no momento

// Busca um identificador: primeiro na TSL da funcao atual, depois na TSG.
// Encerra com erro semantico se nao encontrado.
static type_ts *busca_id(const char *lexema) {
    if (current_func != NULL && current_func->tsl_stack != NULL) {
        type_ts *found = busca_variavel_local(current_func->tsl_stack->tsl, (char *)lexema);
        if (found != NULL) return found;
    }
    type_ts *found = busca_variavel((char *)lexema);
    if (found != NULL) return found;
    printf("[ERRO SEMANTICO] Identificador '%s' nao declarado!\n", lexema);
    exit(1);
}

// ===================== MATCH =====================

int match(int token_tag) {
    if (lookahead->tag == token_tag) {
        lookahead = getToken();
        return true;
    }
    printf("[ERRO SINTATICO] Esperado tag %d, encontrado tag %d (lexema: '%s')\n",
           token_tag, lookahead->tag, lookahead->lexema);
    return false;
}

int is_tipo(int tag) {
    return (tag == KW_INT || tag == KW_FLOAT || tag == KW_STRING || tag == KW_CHAR);
}

// ===================== EXPRESSOES =====================

// F -> ( E ) | NUM | id
void F(char *temp) {
    if (lookahead->tag == OPEN_PAR) {
        match(OPEN_PAR);
        E(temp);
        match(CLOSE_PAR);
    } else if (lookahead->tag == NUM) {
        strcpy(temp, lookahead->lexema);
        match(NUM);
    } else if (lookahead->tag == ID) {
        strcpy(temp, lookahead->lexema);
        busca_id(lookahead->lexema);   // valida: TSL -> TSG -> erro
        match(ID);
    } else {
        printf("[ERRO SINTATICO] Esperado '(', NUM ou ID, encontrado '%s'\n", lookahead->lexema);
    }
}

// TR -> * F TR | / F TR | epsilon
void TR(char *temp_herdado, char *temp) {
    if (lookahead->tag == MULT || lookahead->tag == DIV) {
        char op = lookahead->lexema[0];
        match(lookahead->tag);

        char f_temp[32];
        F(f_temp);

        char novo_temp[32];
        newTemp(novo_temp);
        genOp(novo_temp, temp_herdado, op, f_temp);

        TR(novo_temp, temp);
    } else {
        // epsilon
        strcpy(temp, temp_herdado);
    }
}

// T -> F TR
void T(char *temp) {
    char f_temp[32];
    F(f_temp);

    TR(f_temp, temp);
}

// ER -> + T ER | - T ER | epsilon
void ER(char *temp_herdado, char *temp) {
    if (lookahead->tag == PLUS || lookahead->tag == MINUS) {
        char op = lookahead->lexema[0];
        match(lookahead->tag);

        char t_temp[32];
        T(t_temp);

        char novo_temp[32];
        newTemp(novo_temp);
        genOp(novo_temp, temp_herdado, op, t_temp);

        ER(novo_temp, temp);
    } else {
        // epsilon
        strcpy(temp, temp_herdado);
    }
}

// E -> T ER
void E(char *temp) {
    char t_temp[32];
    T(t_temp);

    ER(t_temp, temp);
}

// ===================== CONDICAO =====================

// condicao -> E op_rel E
void condicao(char *temp1, char *op, char *temp2) {
    E(temp1);

    // operador relacional
    if (lookahead->tag == LT || lookahead->tag == GT ||
        lookahead->tag == LE || lookahead->tag == GE ||
        lookahead->tag == EQ || lookahead->tag == NEQ) {
        strcpy(op, lookahead->lexema);
        match(lookahead->tag);
    } else {
        printf("[ERRO SINTATICO] Esperado operador relacional, encontrado '%s'\n", lookahead->lexema);
        strcpy(op, "??");
    }

    E(temp2);
}

// ===================== COMANDOS =====================

// atribuicao -> = E ;
void atribuicao(char *id) {
    busca_id(id);   // valida o alvo da atribuicao: TSL -> TSG -> erro
    match(ASSIGN);
    char temp[32];
    E(temp);
    genAssign(id, temp);
    match(SEMICOLON);
}

// func_call_cmd -> ( Args ) ;
void func_call_cmd(char *id) {
    type_tsf *func = busca_funcao(id);
    if (func == NULL) {
        printf("[ERRO SEMANTICO] Funcao '%s' nao declarada!\n", id);
        exit(1);
    }

    match(OPEN_PAR);

    int arg_count = 0;
    char args[MAX_PARAMS][MAX_CHAR];   // lexema do argumento (para tipo e valor inicial)
    char temps[MAX_PARAMS][32];        // resultado computado de cada argumento

    if (lookahead->tag != CLOSE_PAR) {
        strcpy(args[arg_count], lookahead->lexema);
        E(temps[arg_count]);
        arg_count++;

        while (lookahead->tag == COMMA) {
            match(COMMA);
            strcpy(args[arg_count], lookahead->lexema);
            E(temps[arg_count]);
            arg_count++;
        }
    }

    match(CLOSE_PAR);
    match(SEMICOLON);

    // Verifica contagem de argumentos
    if (arg_count != func->num_params) {
        printf("[ERRO SEMANTICO] Funcao '%s' espera %d parametros, recebeu %d\n",
               id, func->num_params, arg_count);
        exit(1);
    }

    // Verifica tipos e cria nova TSL com parametros inicializados
    type_ts *nova_tsl = NULL;
    for (int i = 0; i < arg_count; i++) {
        // busca_id: TSL do contexto atual -> TSG -> erro
        type_ts *var = busca_id(args[i]);
        if (strcmp(var->tipo, func->params[i].tipo) != 0) {
            printf("[ERRO SEMANTICO] Argumento '%s' eh do tipo '%s', esperado '%s' (param '%s')\n",
                   args[i], var->tipo, func->params[i].tipo, func->params[i].lexema);
            exit(1);
        }
        // Registra parametro na nova TSL inicializado com o argumento passado
        cadastra_variavel_local(&nova_tsl, func->params[i].tipo,
                                func->params[i].lexema, args[i]);
    }

    // Empilha a nova TSL na funcao chamada (suporta recursao)
    tsl_push(func, nova_tsl);

    // Gera instrucoes de passagem de parametros e chamada
    for (int i = 0; i < arg_count; i++) {
        printf("  param %s\n", temps[i]);
    }
    genJal(func->label);
}

// cmd_if -> if ( condicao ) { comandos } [else { comandos }]
void cmd_if() {
    match(KW_IF);
    match(OPEN_PAR);

    char temp1[32], op[8], temp2[32];
    condicao(temp1, op, temp2);

    match(CLOSE_PAR);

    char label_else[32];
    newLabel(label_else);

    genIfStart(label_else, temp1, op, temp2);

    match(OPEN_BRACE);
    comandos();
    match(CLOSE_BRACE);

    if (lookahead->tag == KW_ELSE) {
        char label_end[32];
        newLabel(label_end);

        genIfElse(label_end, label_else);

        match(KW_ELSE);
        match(OPEN_BRACE);
        comandos();
        match(CLOSE_BRACE);

        genIfEnd(label_end);
    } else {
        genIfEndNoElse(label_else);
    }
}

// cmd_while -> while ( condicao ) { comandos }
void cmd_while() {
    char label_start[32], label_end[32];
    newLabel(label_start);

    genWhileStart(label_start);

    match(KW_WHILE);
    match(OPEN_PAR);

    char temp1[32], op[8], temp2[32];
    condicao(temp1, op, temp2);

    match(CLOSE_PAR);

    newLabel(label_end);
    genWhileCond(label_end, temp1, op, temp2);

    match(OPEN_BRACE);
    comandos();
    match(CLOSE_BRACE);

    genWhileEnd(label_start, label_end);
}

// cmd_read -> read ( id ) ;
void cmd_read() {
    match(KW_READ);
    match(OPEN_PAR);

    char id[MAX_CHAR];
    strcpy(id, lookahead->lexema);
    busca_id(id);   // valida: TSL -> TSG -> erro
    match(ID);

    genRead(id);

    match(CLOSE_PAR);
    match(SEMICOLON);
}

// cmd_write -> write ( E ) ;
void cmd_write() {
    match(KW_WRITE);
    match(OPEN_PAR);

    char temp[32];
    E(temp);

    genWrite(temp);

    match(CLOSE_PAR);
    match(SEMICOLON);
}

// comando -> id ComandoR | if ... | while ... | read ... | write ...
void comando() {
    if (lookahead->tag == ID) {
        char id[MAX_CHAR];
        strcpy(id, lookahead->lexema);
        match(ID);

        if (lookahead->tag == ASSIGN) {
            atribuicao(id);
        } else if (lookahead->tag == OPEN_PAR) {
            func_call_cmd(id);
        } else {
            printf("[ERRO SINTATICO] Esperado '=' ou '(' apos '%s'\n", id);
        }
    } else if (lookahead->tag == KW_IF) {
        cmd_if();
    } else if (lookahead->tag == KW_WHILE) {
        cmd_while();
    } else if (lookahead->tag == KW_READ) {
        cmd_read();
    } else if (lookahead->tag == KW_WRITE) {
        cmd_write();
    } else {
        printf("[ERRO SINTATICO] Comando inesperado: '%s'\n", lookahead->lexema);
    }
}

// comandos -> comando comandos | epsilon
void comandos() {
    while (lookahead->tag == ID || lookahead->tag == KW_IF ||
           lookahead->tag == KW_WHILE || lookahead->tag == KW_READ ||
           lookahead->tag == KW_WRITE) {
        comando();
    }
}

// ===================== DECLARACOES =====================

// Params -> tipo id ParamsR
// ParamsR -> , tipo id ParamsR | epsilon
void parse_params(type_param *params, int *num_params) {
    *num_params = 0;

    if (is_tipo(lookahead->tag)) {
        strcpy(params[*num_params].tipo, lookahead->lexema);
        match(lookahead->tag);
        strcpy(params[*num_params].lexema, lookahead->lexema);
        match(ID);
        (*num_params)++;

        while (lookahead->tag == COMMA) {
            match(COMMA);
            if (*num_params >= MAX_PARAMS) {
                printf("[ERRO] Numero maximo de parametros excedido\n");
                return;
            }
            strcpy(params[*num_params].tipo, lookahead->lexema);
            match(lookahead->tag);
            strcpy(params[*num_params].lexema, lookahead->lexema);
            match(ID);
            (*num_params)++;
        }
    }
}

// declaracaoF -> ( [Params] ) ;
void declaracaoF(char *tipo, char *lexema) {
    match(OPEN_PAR);

    type_param params[MAX_PARAMS];
    int num_params = 0;

    if (lookahead->tag != CLOSE_PAR) {
        parse_params(params, &num_params);
    }

    match(CLOSE_PAR);
    match(SEMICOLON);

    if (busca_funcao(lexema) != NULL) {
        printf("[ERRO SEMANTICO] Funcao '%s' ja declarada na TSF!\n", lexema);
    } else {
        // Prototipo: cadastra funcao com TSL nula
        cadastra_funcao(tipo, lexema, params, num_params);
    }
}

// declaracao -> tipo id ( DeclaracaoV | DeclaracaoF )
void declaracao() {
    char aux_tipo[MAX_CHAR];
    char aux_lexema[MAX_CHAR];

    strcpy(aux_tipo, lookahead->lexema);
    match(lookahead->tag); // consome tipo

    strcpy(aux_lexema, lookahead->lexema);
    match(ID);

    if (lookahead->tag == SEMICOLON) {
        // DeclaracaoV -> ;
        match(SEMICOLON);
        cadastra_variavel(aux_tipo, aux_lexema);
        genDecl(aux_tipo, aux_lexema);
    } else if (lookahead->tag == OPEN_PAR) {
        // DeclaracaoF -> ( [Params] ) ;
        declaracaoF(aux_tipo, aux_lexema);
    } else {
        printf("[ERRO SINTATICO] Esperado ';' ou '(' apos declaracao de '%s'\n", aux_lexema);
    }
}

// declaracoes -> declaracao declaracoes | epsilon
void declaracoes() {
    while (is_tipo(lookahead->tag)) {
        declaracao();
    }
}

// ===================== FUNC_CODE =====================

// func_impl -> tipo id ( [Params] ) { comandos }
void func_impl() {
    // tipo
    char tipo[MAX_CHAR];
    strcpy(tipo, lookahead->lexema);
    match(lookahead->tag);

    // id
    char nome[MAX_CHAR];
    strcpy(nome, lookahead->lexema);
    match(ID);

    // Busca na TSF para pegar o label
    type_tsf *func = busca_funcao(nome);
    if (func == NULL) {
        printf("[ERRO SEMANTICO] Implementacao de funcao '%s' sem prototipo!\n", nome);
        exit(1);
    }

    // Define o contexto de funcao atual (ativa o uso da TSL nos lookups)
    current_func = func;

    match(OPEN_PAR);

    // Consome os parametros da assinatura (ja registrados na TSL pela chamada)
    if (lookahead->tag != CLOSE_PAR) {
        type_param params_dummy[MAX_PARAMS];
        int n_dummy = 0;
        parse_params(params_dummy, &n_dummy);
    }

    match(CLOSE_PAR);

    // Emite .data para cada parametro (tratados como variaveis locais)
    for (int i = 0; i < func->num_params; i++) {
        genDecl(func->params[i].tipo, func->params[i].lexema);
    }

    genFuncLabel(func->label);

    match(OPEN_BRACE);
    comandos();
    match(CLOSE_BRACE);

    genJr();

    current_func = NULL;
}

// func_code -> func_impl func_code | epsilon
void func_code() {
    while (is_tipo(lookahead->tag)) {
        func_impl();
    }
}

// ===================== PROGRAM =====================

// Program -> Declaracoes BEGIN Comandos END Func_code
void program() {
    declaracoes();

    if (!match(KW_BEGIN)) {
        printf("[ERRO SINTATICO] Esperado 'begin'\n");
        return;
    }

    comandos();

    if (!match(KW_END)) {
        printf("[ERRO SINTATICO] Esperado 'end'\n");
        return;
    }

    genHalt();

    func_code();
}
