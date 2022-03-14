#include <stdio.h>
#include <math.h>
#include "../../Stack/Stack/Stack.h"
#include "../../NewOnegin/NewOnegin/String++.h"

//------------------------------------------------------------------------

#ifdef YA_SHTO_TO_NAZHALA_I_TEPER_VSYO_NE_RABOTAET
#define DBG if(1)
#else
#define DBG if(0)
#endif

//------------------------------------------------------------------------

const size_t RAM_SIZE = 131072; //RAM_SIZE * sizeof(double) == 1 Mb

typedef short cmd_t;

struct CPU 
{
    struct Stack* stack;
    double* RAM;
    unsigned char* code;
    size_t codesize;
    double rax;
    double rbx;
    double rcx;
    double rdx;
};

struct Label
{
    char* name;
    size_t point;
};

/*! Округляет num, целая часть которого >=0 и <= maxval, до ближайшего целого
    и это целое записывает по адресу res, возвращает 1 в случае ошибки или если
    num > maxval, 0 в противном случае.*/
int  Round        (double num, unsigned long maxval, unsigned long* res);


int  MakeCPU      (struct CPU* ret, const char* codefile);

/*! Выполняет все операции, записанные в поле code экземпляра структуры CPU 
    по адресу CPU. Возвращает код ошибки и печатает информацию об ошибке.*/
int  DoEverything (struct CPU* CPU);

/*!
    Аргументы: [inputfile_path][-s]
    inputfile_path - адрес файла с байткодом для процессора,
    -s - печатает содержимое стека и регистров после завершения исполнения кода.
    
    Создаёт экземпляр процессора с байткодом по адресу inputfile_path, исполняет
    байткод, в случае ошибки возвращает -1 и печатает информацию об ошибке*/
int main(int argc, char* argv[])
{
    const char* codefile = "disasm.bin";
    int showstk = 0;

    if (argc == 1) 
        ;
    else if (argc == 2)
    {
        if (argv[1][0] == '-') 
        {
            if (argv[1][1] != 's' || argv[1][2] != '\0') 
            {
                printf("ERROR: unknown argument %s\n", argv[1]);
                return -1;
            }
            showstk = 1;
        }
        else 
        {
            codefile = argv[1];
        }
    }
    else if (argc == 3) 
    {
        if (argv[2][0] != '-' || argv[2][1] != 's' || argv[2][2] != '\0') 
        {
            printf("ERROR: unknown argument %s\n", argv[2]);
            return -1;
        }
        showstk = 1;
        codefile = argv[1];
    }
    else
    {
        printf("ERROR: wrong number of parameters passed\n");
        return -1;
    }


    struct CPU CPU1;
    int errcode = MakeCPU(&CPU1, codefile);
    if (errcode != 0) 
    {
        return errcode;
    }

    errcode = DoEverything(&CPU1);
    DBG fStackDump(CPU1.stack, 0, stdout); putchar('\n');
    if (errcode != 0)
    {
        goto close;
    }

    if (showstk) {
        printf("Inside stack:\n");
        while (GetStackSize(CPU1.stack) > 0)
        {
            double val = 0;
            StackPop(CPU1.stack, &val);
            printf("%lf\n", val);
        }
        printf("Registers:\n[rax]: %lf\n[rbx]: %lf\n[rcx]: %lf\n[rdx]: %lf\n",
            CPU1.rax, CPU1.rbx, CPU1.rcx, CPU1.rdx);
    }

    close:
    StackDistruct(&(CPU1.stack));
    free(CPU1.RAM);
    return errcode;
}

int IsZero(double val) 
{
    return val<(1e-10) && val>-(1e-10);
}


int DoEverything(struct CPU* thou)
{
    size_t iter = 0;
    int errcode = 1;
    
    struct Stack* callstk = NULL;
    NewStack(callstk, int);

    while (iter < thou->codesize) 
    {
        double val1 = 0, val2 = 0;
        short param = 0;
        unsigned long lparam = 0;

        cmd_t command = *(cmd_t*)(thou->code + iter);
        iter += sizeof(cmd_t);
        DBG printf("[%d] %hd\n", __LINE__, command);

        switch (command) 
        {

        #define GETPARAM(oper, type, CPU, iter, param)                                        \
                        if (iter + sizeof(type) > CPU->codesize) {                            \
                            printf("Error while processing " oper " near 0x%X byte\n", iter); \
                            return -1;                                                        \
                        }                                                                     \
                        param = *(type*)(CPU->code + iter);                                   \
                        iter += sizeof(type);

        #define ASSERTED(f,oper,...)                                                          \
                        errcode = f(__VA_ARGS__);                                             \
                        if (errcode != 0) {                                                   \
                            printf("Error while processing " oper " near 0x%X byte\n", iter); \
                            return -1;                                                        \
                        }

        #include "case_enumeration.cpp"
            
        #undef GETPARAM
        #undef ASSERTED

        default:
            return iter;
        }

    }
    printf("Success!\n");
    return 0;
}

int MakeCPU(struct CPU* ret, const char* codefile) 
{
    ret->rax = 0; ret->rbx = 0; ret->rcx = 0; ret->rdx = 0;
    ret->stack = NULL;
    ret->code = NULL;
    ret->codesize = 0;

    int errcode = 0;

    FILE* in;
    fopen_s(&in, codefile, "rb");
    if (in == NULL) 
    {
        printf("ERROR: couldn\'t find file %s or the file is empty\n", codefile);
        return -1;
    }
    DBG printf("MakeCPU: file opened\n");
    ret->codesize = GetFileSize(codefile);

    ret->code = (unsigned char*) calloc(ret->codesize, sizeof(ret->code[0]));
    if (ret->code) 
    {
        fread(ret->code, sizeof(unsigned char), ret->codesize, in);
    }
    else 
    {
        printf("ERROR: couldn\'t make buffer for code due to lack of memory\n");

        errcode = -2;
        goto badclose0;
    }

    DBG printf("MakeCPU: code initialized\n");

    ret->RAM = (double*) calloc(RAM_SIZE, sizeof(*ret->RAM));
    if (!ret->RAM) 
    {
        printf("ERROR: couldn\'t make buffer for RAM due to lack of memory\n");

        errcode = -3;
        goto badclose1;
    }

    DBG printf("MakeCPU: RAM initialized\n");

    if (NewStack(ret->stack, double)) 
    {
        printf("ERROR: couldn\'t make buffer for stack due to lack of memory\n");

        errcode = -4;
        goto badclose2;
    }

    badclose2: free(ret->RAM);
    badclose1: free(ret->code);
    badclose0: close:  fclose(in); return errcode;
}

int Round(double num, unsigned long maxval, unsigned long* res)
{
    if (num - 0.4 > maxval || num + 0.4 < 0) return 1;
    *res += (unsigned int)(num + 0.5);
    return 0;
}

int IsReg(short regnum) 
{
    return regnum >= 1 && regnum <= 4;
}
