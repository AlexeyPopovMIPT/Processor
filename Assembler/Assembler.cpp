#include "ASM.h"
#include "SmartBuffer.h"

extern int Compare (const void* val1, const void* val2);

extern int FirstCompile  (      unsigned char*      code, int   codecpc, struct Buffer* Labels,
                          const struct Buffer*  bytecode);

extern int SecondCompile (const          char*      code, int  codesize, struct Buffer* Labels);


int Assemble(const char* input, const char* output);


int main(int argc, char* argv[])
{
    if (argc == 1) {
        return Assemble("C:/Users/днс/Desktop/code.asm", "../../Compiler/Compiler/disasm.bin");
    }

    if (argc == 3) {
        return Assemble(argv[0], argv[1]);
    }

    printf("ERROR: wrong number of parameters passed\n");
    return ERR_ARGC_DIDNT_MATCH;

}



int Assemble(const char* input, const char* output) 
{
    FILE* in = NULL, *out = NULL;

    int errcode = 0;

    fopen_s(&in, input, "rb");
    if (in == NULL) 
    {
        printf("ERROR: couldn\'t open file %s\n", input);
        errcode = ERR_INPUT_FILE_NOT_FOUND;
        goto close0;
    }
    fopen_s(&out, output, "wb");
    if (out == NULL)
    {
        printf("ERROR: couldn\'t open file %s\n", output);
        errcode = ERR_OUTPUT_FILE_NOT_FOUND;
        goto close1;
    }


    int codecpc = GetFileSize(input) + 1;
    char* code = (char*) calloc(codecpc, sizeof(code[0]));
    if (code == NULL)
    {
        printf("ERROR: code file is too big\n");
        errcode = ERR_FILE_TOO_BIG;
        goto close2;
    }

    code = GetDataAndCountStrings(in, (unsigned char*)code, &codecpc, NULL);


    struct Buffer bytecode;
    struct Buffer Labels;

    if (MakeBuffer(&bytecode, 0) != 0)
    {
        printf("ERROR: haven\'t got enough memory\n");
        errcode = ERR_NO_MEMORY;
        goto close3;
    }
    if (MakeBuffer(&Labels, 0) != 0)
    {
        printf("ERROR: haven\'t got enough memory\n");
        errcode = ERR_NO_MEMORY;
        goto close4;
    }
   
    errcode = FirstCompile( (unsigned char*)code, codecpc, &Labels, &bytecode );
    if (errcode != 0)
    { 
        goto close5;
    }


    ResizeTo(&bytecode, 0);
    ResizeTo(&Labels, 0);


    qsort((void*)(Labels.buffer), Labels.count / sizeof(struct Label), sizeof(struct Label), Compare);   


    errcode = SecondCompile(bytecode.buffer, bytecode.count, &Labels);
    if (errcode != 0)
    { 
        goto close5;
    }


    fwrite(bytecode.buffer, sizeof(char), bytecode.count, out);

    errcode = bytecode.count;


    close5: BufferDistruct(&Labels);
    close4: BufferDistruct(&bytecode);
    close3: free(code);
    close2: fclose(out);
    close1: fclose(in);

    close0: return errcode;
}
