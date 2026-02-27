/**
 * @file gen.h
 * @author Ivairton M. Santos - UFMT - Computacao
 * @brief Modulo do gerador de codigo
 * @version 0.3
 * @date 2022-04-25
 *
 */
#ifndef _GEN_H_
#define _GEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "struct_compiler.h"

// Prototipos
void newTemp(char *);
void newLabel(char *);
void genFuncLabel(const char *name);
void genLabel(const char *label);
void genJal(const char *label);
void genJr();
void genHalt();
void genDecl(const char *tipo, const char *id);
void genAssign(const char *id, const char *temp);
void genRead(const char *id);
void genWrite(const char *expr);
void genIfStart(const char *label_else, const char *temp, const char *op, const char *temp2);
void genIfElse(const char *label_end, const char *label_else);
void genIfEnd(const char *label_end);
void genIfEndNoElse(const char *label_else);
void genWhileStart(const char *label_start);
void genWhileCond(const char *label_end, const char *temp, const char *op, const char *temp2);
void genWhileEnd(const char *label_start, const char *label_end);
void genOp(const char *dest, const char *src1, char op, const char *src2);
void genCopy(const char *dest, const char *src);

#endif //_GEN_H_