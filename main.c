#include "synt.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_fonte>\n", argv[0]);
        return 1;
    }

    tsf = NULL;
    tsg = NULL;

    initLex(argv[1]);
    lookahead = getToken();

    printf("=== Codigo Intermediario ===\n");
    program();

    imprime_tsf();
    imprime_tsg();

    if (lookahead->tag == ENDTOKEN) {
        printf("\nCompilacao concluida com sucesso!\n");
    } else {
        printf("\n[ERRO] Tokens nao consumidos. Proximo: '%s' (tag: %d)\n",
               lookahead->lexema, lookahead->tag);
    }

    return 0;
}
