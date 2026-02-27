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

// ===================== CONVENCAO DE CHAMADA MIPS =====================

static int is_num_str(const char *s) {
    if (!s || !s[0]) return 0;
    int start = (s[0] == '-') ? 1 : 0;
    if (!s[start]) return 0;
    for (int i = start; s[i]; i++)
        if (!isdigit((unsigned char)s[i]) && s[i] != '.') return 0;
    return 1;
}

void genFuncPreamble(int frame_size) {
    printf("    # Preambulo\n");
    printf("    addi $sp, $sp, -%d\n", frame_size);
    printf("    sw   $ra, 0($sp)\n");
    printf("    sw   $fp, 4($sp)\n");
    printf("    move $fp, $sp\n");
}

void genSaveArg(int i, int offset) {
    printf("    sw   $a%d, %d($sp)    # param %d\n", i, offset, i);
}

void genMoveReturn(const char *temp) {
    printf("    move $v0, %s    # valor de retorno\n", temp);
}

void genFuncEpilogue(int frame_size) {
    printf("    # Epilogo\n");
    printf("    move $sp, $fp\n");
    printf("    lw   $fp, 4($sp)\n");
    printf("    lw   $ra, 0($sp)\n");
    printf("    addi $sp, $sp, %d\n", frame_size);
    printf("    jr   $ra\n");
}

void genCallArg(int i, const char *val) {
    if (is_num_str(val))
        printf("    li   $a%d, %s\n", i, val);
    else if (val[0] == 't' && isdigit((unsigned char)val[1]))
        printf("    move $a%d, %s\n", i, val);   // temp intermediario
    else
        printf("    lw   $a%d, %s\n", i, val);   // variavel em memoria
}

void genCallGetReturn(const char *dest_temp) {
    printf("    move %s, $v0    # pega valor retornado\n", dest_temp);
}
