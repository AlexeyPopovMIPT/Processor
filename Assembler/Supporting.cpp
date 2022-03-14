#include "ASM.h"
#include <ctype.h>  //для isspace

int GetHashCode(const char* command, int len) 
{
    const int HASHFACTOR = -1484991287;
    int ret = 0;
    for (; len > 0; len--, command++) {
        ret = HASHFACTOR * ret + *command;
    }
    return ret;
}

void SkipSpaces(const char** ptr)
{
    while (isspace(**ptr)) (*ptr)++;
}

void ToNextStr(const unsigned char** ptr) 
{
    DBG printf("ToNextStr: %s %d, ", *ptr, **ptr);

    if (**ptr == '\0') return;
    SkipSpaces(ptr);

    int len = -(int)*ptr + 0xBADBABA;
    sscanf_s((const char*)(*ptr), "%*s%n", &len);
    *ptr += len;

    DBG printf("%d, ", len);

    SkipSpaces(ptr);

    DBG printf("%d\n", **ptr);
}

void SkipDouble(const unsigned char** ptr)
{
    DBG printf("SkipDouble: %s %d, ", *ptr, **ptr);
    if (**ptr == '-' || **ptr == '+') ++* ptr;

    //inf
    if (**ptr == 'i' && *(*ptr + 1) == 'n' && *(*ptr + 2) == 'f') {
        *ptr += 3;
    }
    //nan(ind)
    else if (**ptr == 'n' && *(*ptr + 1) == 'a' && *(*ptr + 2) == 'n') {
        *ptr += 3;
        if (**ptr == '(' && *(*ptr + 1) == 'i' && *(*ptr + 2) == 'n' && *(*ptr + 3) == 'd' && *(*ptr + 4) == ')') {
            *ptr += 5;
        }
    }
    //common num
    else {
        for (; **ptr >= '0' && **ptr <= '9'; ++ * ptr);
        if (**ptr == '.') {
            for (++* ptr; **ptr >= '0' && **ptr <= '9'; ++ * ptr);
        }
    }
    SkipSpaces(ptr);
    DBG printf("%d\n", **ptr);
}

void SkipInteger(const unsigned char** ptr)
{
    if (**ptr == '-') ++*ptr;
    for (; **ptr >= '0' && **ptr <= '9'; ++*ptr);
    SkipSpaces(ptr);
}

static void fBufferDump(const unsigned char* buffer, unsigned int bytecnt, FILE* const stream) 
{
    unsigned char c = '\n';
    for (unsigned int i = 0; i < bytecnt; i++) {
        if (!(i & 0b11)) {
            fprintf(stream, "\n[%4d]", i);
        }
        c = buffer[i] / 16;
        if (c < 10) { fputc(c + '0', stream); }
        else { fputc(c - 10 + 'A', stream); }

        c = buffer[i] % 16;
        if (c < 10) { fputc(c + '0', stream); }
        else { fputc(c - 10 + 'A', stream); }

        fputc(' ', stream);
    }

}

void* Reallocate(void* buffer, int* capacity, int elemsize) 
{
    int grow = *capacity;
    void* newbuffer;
    while ((newbuffer = realloc(buffer, (*capacity + grow) * elemsize)) == NULL && grow > 1)
        grow /= 2;
    if (newbuffer == NULL) return NULL;
    *capacity += grow;
    return newbuffer;
}

int Compare(const void* val1, const void* val2) 
{
    return strcmp(((struct Label*)val1)->name, ((struct Label*)val2)->name);
}

int IsReg(const char* ptr) 
{
    if (*(ptr + 1) != 'x') return 0;
    return (*ptr == 'a' || *ptr == 'b' || *ptr == 'c' || *ptr == 'd');
}

int ProcessComplexArg (const char** reader, short* int16arg, double* doublearg,
                       char** param1_writer, char** param2_writer)
{
    assert(reader);
    assert(*reader);
    assert(int16arg);
    assert(doublearg);
    assert(param1_writer);
    assert(param2_writer);

    *param1_writer = NULL;
    *param2_writer = NULL;

    int ret = 0b000;

    if (**reader == '[') {
        while (*++*reader == ' ')
            ;
        ret |= 0b100;
    }

    if (**reader == 'r') {
        ++*reader;
        if (!IsReg(*reader))
            return -1;

        *int16arg = **reader - 'a' + 1;
        *param1_writer = (char*)int16arg;
        ret |= 0b010;

        *reader += 2;
        SkipInteger(reader);

        if (sscanf_s((const char*)*reader, "%lf", doublearg) == 1) {
            if (**reader == '+' || **reader == '-') {
                SkipDouble(reader);
                *param2_writer = (char*)doublearg;
                ret |= 0b001;
            }
            else
                return -1;

        }
    }
    else {
        if (sscanf_s((const char*)*reader, "%lf", doublearg) == 1) {
            SkipDouble(reader);
            *param1_writer = (char*)doublearg;
            ret |= 0b001;
        }
    }

    if ((ret & 0b100) && **reader != ']')
        return -1;
    if (ret & 0b100) {
        if (**reader == ']') {
            ++*reader;
        }
        else
            return -1;
    }
    return ret;
}

size_t MyStrlen(const char* str) {
    int i = 0;
    for (; !isspace(str[i]) && str[i] != '\0'; i++);
    return i;
}

int ProcessLabelLinkArg(char** reader, char** param1_writer, long* param3)
{
    assert(reader);
    assert(*reader);
    assert(param1_writer);
    assert(*param1_writer);
    assert(param3);

    int lnk_len = MyStrlen(*reader);;

    *param3 = *reader;
    *param1_writer = (char*)param3;

    if ((*reader)[lnk_len] == '\0') {
        ToNextStr(reader);
        return 0;
    }

    (*reader)[lnk_len] = '\0';
    *reader += lnk_len + 1;

    SkipSpaces(reader);

    return 0;
}
