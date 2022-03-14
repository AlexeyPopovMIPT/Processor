#include "ASM.h"
#include "SmartBuffer.h"

extern size_t MyStrlen           (const char* str);
extern int    SkipSpaces          (const char** ptr);
extern int    IsReg               (const char* ptr);
extern int    Compare             (void* val1, void* val2);
extern int    GetHashCode         (const char* str, int len);
extern int    ProcessComplexArg   (const char** reader, short* int16arg, double* doublearg,
                                    char** param1_pointer, char** param2_pointer );
extern int    ProcessLabelLinkArg (char** reader, char** param1_writer, long* param3);
extern void   ToNextStr           (unsigned char** pointer);
extern void   SkipDouble          (unsigned char** pointer);
extern void   SkipInteger         (unsigned char** pointer);
extern void*  Reallocate          (void* buffer, int* cpc, int elemsize);



int FirstCompile( unsigned char* code, int codecpc, struct Buffer* Labels,
                        const struct Buffer* bytecode)
{

    assert(code);
    assert(codecpc >= 0);


    int cmd_len = -1;
    int cmd_hash = 0;
    short param0 = 0;
    double param1 = 0;
    long param2 = 0;

    char* param1_writer = NULL;
    char* param2_writer = NULL;
    cmd_t cmdname = NO_COMMAND;

    unsigned char* reader = code;
    int line = 0;

    int namescnt = 0;
    int jumptocnt = 0;

    #define CONDITION (reader - code < codecpc)

    while (CONDITION) 
    {
        ++line;

        for (;;)
        {
            SkipSpaces(&reader);
            if (*reader == ';' || *reader == '\0')
            {
                goto skipping;
            }
            cmd_len = MyStrlen(reader);

            if (reader[cmd_len - 1] != ':') break;

            reader[cmd_len - 1] = '\0';
            --cmd_len;

            struct Label probe = { reader, bytecode->count };

            if (AddToEnd(Labels, sizeof(struct Label), &probe) != 0)
            {
                printf("ERROR: hven\'t got enough memory\n");
                return ERR_NO_MEMORY;
            }

            reader += cmd_len + 1;

        }

        cmd_hash = GetHashCode(reader, cmd_len);
        DBG printf("[%d] %s %d\n", __LINE__, reader, cmd_hash);

        ToNextStr(&reader);

        param1_writer = NULL;
        param2_writer = NULL;
        cmdname = NO_COMMAND;

        switch (cmd_hash) {

        #include "case_enumeration.cpp"
        
        //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        writing:
            DBG printf("[cmdname at writing:] %d\n", cmdname);
            if (cmdname == NO_COMMAND) goto error;


            if (AddToEnd(bytecode, sizeof(cmd_t), &cmdname) != 0) 
            {
                printf("ERROR: haven\'t got enough memory\n");
                return ERR_NO_MEMORY;
            }

            if (param1_writer == NULL) goto skipping;
            if (param1_writer == (char*)&param0) 
            {
                if (AddToEnd(bytecode, sizeof(param0), param1_writer) != 0) 
                {
                    printf("ERROR: haven\'t got enough memory\n");
                    return ERR_NO_MEMORY;
                }
            }
            else if (param1_writer == (char*)&param1) 
            {
                if (AddToEnd(bytecode, sizeof(param1), param1_writer) != 0) 
                {
                    printf("ERROR: haven\'t got enough memory\n");
                    return ERR_NO_MEMORY;
                }
            }
            else if (param1_writer == (char*)&param2) 
            {
                if (AddToEnd(bytecode, sizeof(param2), param1_writer) != 0) 
                {
                    printf("ERROR: haven\'t got enough memory\n");
                    return ERR_NO_MEMORY;
                }
            }
            else 
            {
                printf("Invalid param1_writer pointer\n");
                return ERR_INVALID_PARAM_POINTER;
            }

            if (param2_writer == NULL) goto skipping;
            if (param2_writer == (char*)&param1)
            {
                if (AddToEnd(bytecode, sizeof(param1), param2_writer) != 0) 
                {
                    printf("ERROR: haven\'t got enough memory\n");
                    return ERR_NO_MEMORY;
                }
            }
            else 
            {
                printf("Invalid param2_writer pointer\n");
                return ERR_INVALID_PARAM_POINTER;
            }
            goto skipping;

        //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        skipping:
            DBG printf("[%d]: %d\n", __LINE__, *reader);
            if (*reader == ';') 
            {
                while (*reader != '\0' && CONDITION) ToNextStr(&reader);
            }
            if (*reader == '\0')
            {
                reader++;
                DBG printf("[%d]: %d\n", __LINE__, *reader);
                break;
            }
            if (!CONDITION) 
            {
                break;
            }
            goto error;

        //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        default: error:
            DBG printf("[%d]: %s\n", __LINE__, reader);
            printf("ERROR: unrecognized token at line %d\n", line);
            return ERR_UNRECOGNIZED_TOKEN;
        }

    }

    #undef CONDITION

    printf("Success!\n");

    return 0; 
}





int SecondCompile(const char* code, int codesize, struct Buffer* Labels)
{
    cmd_t command;
    long* writer;

    for (int iter = 0; iter < codesize; ) 
    {
        DBG printf("[SecondCompile] %d \n", iter);

        command = *(cmd_t*)(code + iter);
        iter += sizeof(cmd_t);

        switch (command) 
        {

        #include "second_compile.cpp"

        //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        case 27: case 28: case 29: case 30: case 31: case 32: case 33: case 34:
           //jmp      ja       jae      jb       jbe      je       jne      call
            DBG printf("[SecondCompile] jump\n");

            struct Label probe = { *(char**)(code + iter), 0 };

            struct Label* result = bsearch( (const void*)&probe, (const void*)(Labels->buffer), 
                                              Labels->count / sizeof(struct Label), sizeof(struct Label), Compare );

            if (result == NULL) 
            {
                printf("ERROR: label %s not found\n", probe.name);
                return ERR_LABEL_NOT_FOUND;
            }

            writer = (long*)(code + iter);
            *writer = result->point;
            iter += sizeof(long);
            break;

        //-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

        default:
            printf("ERROR: unexpected command with code %d at byte 0x%X", command, iter);
            return -666;

        }
    }

    return 0;
}
