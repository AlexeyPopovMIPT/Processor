#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../../NewOnegin/NewOnegin/String++.h"
#include "Errcodes.h"

#ifdef YA_SHTO_TO_NAZHALA_I_TEPER_VSYO_NE_RABOTAET
#define DBG if(1)
#else
#define DBG if(0)
#endif

typedef short cmd_t;

extern int DisassembleItself(char* code, int FileSize, FILE* out);

int main(int argc, const char* argv[])
{

	const char* bytecode = "../../Compiler/Compiler/disasm.bin";
	const char* disasmfile = "C:/Users/днс/Desktop/result.asm";
	if (argc == 3) {
		bytecode = argv[0];
		disasmfile = argv[1];
	}
	else if (argc != 1){
		return ERR_ARGC_DIDNT_MATCH;
	}

	FILE *in, *out;
	if (OpenFile(bytecode, "rb", &in ) != 0 || OpenFile(disasmfile, "wb", &out) != 0) 
	{
		return ERR_FILE_NOT_FOUND;
	}

	int FileSize = GetFileSize(bytecode);

	char* code = (char*) calloc(FileSize + 1, sizeof(code[0]));
	if (code == NULL) 
	{
		printf("ERROR: haven\'t got enough memory\n");
		return ERR_NO_MEMORY;
	}

	fread(code, 1, FileSize, in);

	int errcode = DisassembleItself(code, FileSize, out);

	if (errcode == 0)
	{
		printf("Success!\n");
	}
	else 
	{
		printf("ERROR: unknown command at 0x%X byte\n", errcode);
	}

	free(code);
	return errcode;

}

int DisassembleItself(char* code, int FileSize, FILE* out)
{
	assert(code);
	assert(FileSize >= 0);
	assert(out);

	int bytes_read = 0;
	cmd_t command = -1;
	double number = 0;

	while (bytes_read < FileSize) 
	{
		fprintf(out, "0x%5X %5d ", bytes_read, bytes_read);
		command = *(cmd_t*)(code + bytes_read);
		bytes_read += sizeof(cmd_t);

		switch (command) 
		{

        #include "Mask.h"
        #define GETPARAM(type) \
                       { \
                          type GETPARAM_parameter = *(type*)(code + bytes_read); \
                          bytes_read += sizeof(type); \
                          fprintf(out, type##MASK " ", GETPARAM_parameter); \
			           }
		
        #include "case_enumeration.cpp"

		default:
			return bytes_read - sizeof(cmd_t);

        #undef GETPARAM
        #include "Mask.h"

		}

	}
	return 0;
}
