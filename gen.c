/**
 * @file gen.c
 * @author Ivairton M. Santos - UFMT - Computacao
 * @brief Codificacao do modulo gerador de codigo
 * @version 0.3
 * @date 2022-04-25
 *
 */

#include "gen.h"

static int temp_count = 0;
static int label_count = 0;

void newTemp(char *label) {
    sprintf(label, "t%d", temp_count++);
}

void newLabel(char *label) {
    sprintf(label, "L%d", label_count++);
}

void genFuncLabel(const char *name) {
    printf("%s:\n", name);
}

void genLabel(const char *label) {
    printf("%s:\n", label);
}

void genJal(const char *label) {
    printf("  jal %s\n", label);
}

void genJr() {
    printf("  jr $ra\n");
}

void genHalt() {
    printf("  halt\n");
}

void genDecl(const char *tipo, const char *id) {
    printf("  .data %s %s\n", tipo, id);
}

void genAssign(const char *id, const char *temp) {
    printf("  %s = %s\n", id, temp);
}

void genRead(const char *id) {
    printf("  read %s\n", id);
}

void genWrite(const char *expr) {
    printf("  write %s\n", expr);
}

void genIfStart(const char *label_else, const char *temp, const char *op, const char *temp2) {
    printf("  ifFalse (%s %s %s) goto %s\n", temp, op, temp2, label_else);
}

void genIfElse(const char *label_end, const char *label_else) {
    printf("  goto %s\n", label_end);
    printf("%s:\n", label_else);
}

void genIfEnd(const char *label_end) {
    printf("%s:\n", label_end);
}

void genIfEndNoElse(const char *label_else) {
    printf("%s:\n", label_else);
}

void genWhileStart(const char *label_start) {
    printf("%s:\n", label_start);
}

void genWhileCond(const char *label_end, const char *temp, const char *op, const char *temp2) {
    printf("  ifFalse (%s %s %s) goto %s\n", temp, op, temp2, label_end);
}

void genWhileEnd(const char *label_start, const char *label_end) {
    printf("  goto %s\n", label_start);
    printf("%s:\n", label_end);
}

void genOp(const char *dest, const char *src1, char op, const char *src2) {
    printf("  %s = %s %c %s\n", dest, src1, op, src2);
}

void genCopy(const char *dest, const char *src) {
    printf("  %s = %s\n", dest, src);
}
