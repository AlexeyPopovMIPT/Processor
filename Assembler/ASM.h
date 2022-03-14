#pragma once
#ifndef INC_APS_ASM

#define INC_APS_ASM

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../../NewOnegin/NewOnegin/String++.h"

//#define YA_SHTO_TO_NAZHALA_I_TEPER_VSYO_NE_RABOTAET
#ifdef YA_SHTO_TO_NAZHALA_I_TEPER_VSYO_NE_RABOTAET
#define DBG if(1)
#else
#define DBG if(0)
#endif

typedef short cmd_t;

static const cmd_t NO_COMMAND = -1;

struct Label
{
    char* name;
    size_t point;
};

enum PREPARING_ERRORS
{
    ERR_ARGC_DIDNT_MATCH = -5, //ERROR: wrong number of parameters passed
    ERR_INPUT_FILE_NOT_FOUND = 2, //ERROR: couldn't open file %s/n", inputpath
    ERR_OUTPUT_FILE_NOT_FOUND = -2, //ERROR: couldn't open file %s/n", outputpath
    ERR_FILE_TOO_BIG = -2147483647, //ERROR: code file is too big
    ERR_NO_MEMORY = -16 //"ERROR: haven't got enough memory
};

enum COMPILATION_ERRORS 
{
    ERR_NO_VALID_PUSH_ARG = -1, //ERROR: push argument not found on line %d\n, line
    ERR_NO_VALID_POP_ARG = -3,
    ERR_NO_VALID_PEEK_ARG = -4,
    ERR_NO_VALID_HECK_ARG = -17,
    ERR_NO_VALID_JUMP_ARG = -27,
    ERR_UNRECOGNIZED_TOKEN = -666, //ERROR: unrecognized token at line %d\n, line
    ERR_INVALID_PARAM_POINTER = -11,
    ERR_LABEL_NOT_FOUND = -8
};

#endif // INC_APS_ASM
