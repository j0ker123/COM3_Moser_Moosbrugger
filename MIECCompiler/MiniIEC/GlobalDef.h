///////////////////////////////////////////////////////////////////////////////
// Global definitions
//

#pragma once

typedef unsigned char BYTE;
typedef unsigned short WORD;

// binary helper
#define B(x) ( \
  0##x >>  0 & 0001 | \
  0##x >>  2 & 0002 | \
  0##x >>  4 & 0004 | \
  0##x >>  6 & 0010 | \
  0##x >>  8 & 0020 | \
  0##x >> 10 & 0040 | \
  0##x >> 12 & 0100 | \
  0##x >> 14 & 0200 )

// Opcodes of the PROL16 commands
#define OPCODE_LOADI    B(000010)
#define OPCODE_LOAD     B(000011)
#define OPCODE_STORE    B(000100)
#define OPCODE_MOVE     B(001100)
#define OPCODE_ADD      B(010100)
#define OPCODE_ADDC     B(010101)
#define OPCODE_SUB      B(010110)
#define OPCODE_SUBC     B(010111)
#define OPCODE_COMP     B(011000)
#define OPCODE_INC      B(011010)
#define OPCODE_DEC      B(011011)
#define OPCODE_AND      B(010000)
#define OPCODE_OR       B(010001)
#define OPCODE_XOR      B(010010)
#define OPCODE_NOT      B(010011)
#define OPCODE_SHL      B(011100)
#define OPCODE_SHR      B(011101)
#define OPCODE_SHLC     B(011110)
#define OPCODE_SHRC     B(011111)
#define OPCODE_JUMP     B(001000)
#define OPCODE_JUMPC    B(001010)
#define OPCODE_JUMPZ    B(001011)
#define OPCODE_NOP      B(000000)
#define OPCODE_SLEEP    B(000001)
#define OPCODE_PRINTINT B(111110)
#define OPCODE_PRINTSTR B(111111)
