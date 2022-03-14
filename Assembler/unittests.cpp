#include "ASM.h"

void ProcessComplexArg_test() 
{
	extern int ProcessComplexArg(const char** const reader, short* int16arg, double* doublearg,
		char** param1_writer, char** param2_writer);

	short int16arg = 0;
	double doublearg = 0;
	char* param1_writer = NULL;
	char* param2_writer = NULL;

    #define RESET \
            int16arg = 0; \
	        doublearg = 0; \
	        param1_writer = NULL; \
	        param2_writer = NULL;
	{
		const char* buffer = "rax";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 1: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 2, res);
		printf("    param1_writer: %p, %p\n"    , &int16arg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , NULL, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 1, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 0.0, doublearg);
	}
	RESET
	{
		const char* buffer = "5.67";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 2: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 1, res);
		printf("    param1_writer: %p, %p\n"    , &doublearg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , NULL, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 0, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 5.67, doublearg);
	}
		RESET
	{
		const char* buffer = "rax+1";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 3: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 3, res);
		printf("    param1_writer: %p, %p\n"    , &int16arg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , &doublearg, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 1, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 1.0, doublearg);
	}
		RESET
	{
		const char* buffer = "[rax+5.67]";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 4: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 7, res);
		printf("    param1_writer: %p, %p\n"    , &int16arg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , &doublearg, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 1, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 5.67, doublearg);
	}
		RESET
	{
		const char* buffer = "[rbx]";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 4.5: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 6, res);
		printf("    param1_writer: %p, %p\n"    , &int16arg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , NULL, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 2, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 0.0, doublearg);
	}
		RESET
	{
		const char* buffer = "[rax5.67]";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 5: %s\n", buffer);
		printf("              res: %8d, %8d\n", -1, res);
	}
		RESET
	{
		const char* buffer = "[rax+5.67";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 6: %s\n", buffer);
		printf("              res: %8d, %8d\n", -1, res);
	}
		RESET
	{
		const char* buffer = "[]";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 7: %s\n", buffer);
		printf("              res: %8d, %8d\n", 4, res);
	}
		RESET
	{
		const char* buffer = "1";
		char* reader = buffer;
		int res = ProcessComplexArg(&reader, &int16arg, &doublearg, &param1_writer, &param2_writer);
		printf("Test 8: %s\n", buffer);
		printf("              res: %8d, %8d\n"  , 1, res);
		printf("    param1_writer: %p, %p\n"    , &doublearg, param1_writer);
		printf("    param2_writer: %p, %p\n"    , NULL, param2_writer);
		printf("         int16arg: %8hd, %8hd\n", 0, int16arg);
		printf("        doublearg: %8lf, %8lf\n", 1.0, doublearg);
	}

}
