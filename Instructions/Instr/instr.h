#pragma once

#include <stdint.h>

void instr_addi(uint8_t *t);
void instr_add(uint8_t *t);
void instr_andi(uint8_t *t);
void instr_and(uint8_t *t);
void instr_empty(uint8_t *t);
void instr_lb(uint8_t *t);
void instr_lw(uint8_t *t);
void instr_ori(uint8_t *t);
void instr_or(uint8_t *t);
void instr_sb(uint8_t *t);
void instr_slli(uint8_t *t);
void instr_sll(uint8_t *t);
void instr_srai(uint8_t *t);
void instr_sra(uint8_t *t);
void instr_srli(uint8_t *t);
void instr_srl(uint8_t *t);
void instr_sub(uint8_t *t);
void instr_sw(uint8_t *t);
void instr_xori(uint8_t *t);
void instr_xor(uint8_t *t);