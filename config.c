#include <stdio.h>
#include "config.h"

const char *saved_words[] = {"db", "dh", "dw", "entry", "extern", "asciz", NULL};
const BOOL IS_DEBUG = TRUE;

Instruction instruct_array[] = {
        /* opcode and funct are saved as ints and not chars representing binary code (i.e. "00010")
         * as it's easier to make mistakes when using chars, and harder to find and corrects them. */
        {  R_CMD, ADD,"add", 0,    1 },
        {  R_CMD, SUB,"sub", 0,    2 },
        {  R_CMD, AND,"and",0,     3 },
        {  R_CMD, OR, "or",0,      4 },
        {  R_CMD, NOR,"nor",0,     5 },
        { R_CMD , MOVE, "move",1,  1 },
        { R_CMD ,  MVHI,"mvhi",1,  2 },
        {  R_CMD, MVLO,"mvlo",1,   3 },

        { I_CMD,  ADDI,"addi",10 },
        { I_CMD, SUBI,  "subi",11 },
        { I_CMD, ANDI,"andi",12  },
        { I_CMD,  ORI,"ori", 13  },
        { I_CMD,  NORI,"nori",14 },
        { I_CMD, BNE,"bne",15  },
        { I_CMD, BEQ, "beq",16   },
        { I_CMD, BLT, "blt",17   },
        { I_CMD, BGT,"bgt",18    },
        { I_CMD, LB, "lb",19     },
        { I_CMD, SB, "sb",20     },
        { I_CMD, LW, "lw",21     },
        { I_CMD, SW, "sw",22     },
        { I_CMD, LH, "lh",23     },
        { I_CMD, SH, "sh",24     },

        { J_CMD, JMP, "jmp",30   },
        { J_CMD, LA, "la",31    },
        { J_CMD, CALL, "call",32},
        { J_CMD, STOP, "stop",63 },
        { NO_CMD, NOT_DEF,  NULL}
};







