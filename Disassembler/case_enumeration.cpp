case 0:
DBG printf("hlt\n");
fprintf(out, "hlt "); 
fputc(10, out); break;
case 1:
DBG printf("push\n");
fprintf(out, "push "); 
GETPARAM(double); fputc(10, out); break;
case 2:
DBG printf("push\n");
fprintf(out, "push "); 
fputc('r', out); GETPARAM(short); fputc(10, out); break;
case 3:
DBG printf("push\n");
fprintf(out, "push "); 
fputc('r', out); GETPARAM(short); GETPARAM(double); fputc(10, out); break;
case 5:
DBG printf("push\n");
fprintf(out, "push "); 
fputc('[', out);GETPARAM(double); fputc(']', out);fputc(10, out); break;
case 6:
DBG printf("push\n");
fprintf(out, "push "); 
fputc('[', out);fputc('r', out); GETPARAM(short); fputc(']', out);fputc(10, out); break;
case 7:
DBG printf("push\n");
fprintf(out, "push "); 
fputc('[', out);fputc('r', out); GETPARAM(short); GETPARAM(double); fputc(']', out);fputc(10, out); break;
case 8:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc(10, out); break;
case 10:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc('r', out); GETPARAM(short); fputc(10, out); break;
case 11:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc('r', out); GETPARAM(short); GETPARAM(double); fputc(10, out); break;
case 13:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc('[', out);GETPARAM(double); fputc(']', out);fputc(10, out); break;
case 14:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc('[', out);fputc('r', out); GETPARAM(short); fputc(']', out);fputc(10, out); break;
case 15:
DBG printf("pop\n");
fprintf(out, "pop "); 
fputc('[', out);fputc('r', out); GETPARAM(short); GETPARAM(double); fputc(']', out);fputc(10, out); break;
case 4:
DBG printf("peek\n");
fprintf(out, "peek "); 
fputc('r', out); GETPARAM(short); fputc(10, out); break;
case 9:
DBG printf("add\n");
fprintf(out, "add "); 
fputc(10, out); break;
case 12:
DBG printf("sub\n");
fprintf(out, "sub "); 
fputc(10, out); break;
case 16:
DBG printf("mul\n");
fprintf(out, "mul "); 
fputc(10, out); break;
case 17:
DBG printf("div\n");
fprintf(out, "div "); 
fputc(10, out); break;
case 18:
DBG printf("pow\n");
fprintf(out, "pow "); 
fputc(10, out); break;
case 19:
DBG printf("sqrt\n");
fprintf(out, "sqrt "); 
fputc(10, out); break;
case 20:
DBG printf("sin\n");
fprintf(out, "sin "); 
fputc(10, out); break;
case 21:
DBG printf("cos\n");
fprintf(out, "cos "); 
fputc(10, out); break;
case 22:
DBG printf("neg\n");
fprintf(out, "neg "); 
fputc(10, out); break;
case 23:
DBG printf("in\n");
fprintf(out, "in "); 
fputc(10, out); break;
case 24:
DBG printf("out\n");
fprintf(out, "out "); 
fputc(10, out); break;
case 25:
DBG printf("dump\n");
fprintf(out, "dump "); 
fputc(10, out); break;
case 26:
DBG printf("heck\n");
fprintf(out, "heck "); 
GETPARAM(short); fputc(10, out); break;
case 27:
DBG printf("jmp\n");
fprintf(out, "jmp "); 
GETPARAM(long); fputc(10, out); break;
case 28:
DBG printf("ja\n");
fprintf(out, "ja "); 
GETPARAM(long); fputc(10, out); break;
case 29:
DBG printf("jae\n");
fprintf(out, "jae "); 
GETPARAM(long); fputc(10, out); break;
case 30:
DBG printf("jb\n");
fprintf(out, "jb "); 
GETPARAM(long); fputc(10, out); break;
case 31:
DBG printf("jbe\n");
fprintf(out, "jbe "); 
GETPARAM(long); fputc(10, out); break;
case 32:
DBG printf("je\n");
fprintf(out, "je "); 
GETPARAM(long); fputc(10, out); break;
case 33:
DBG printf("jne\n");
fprintf(out, "jne "); 
GETPARAM(long); fputc(10, out); break;
case 34:
DBG printf("call\n");
fprintf(out, "call "); 
GETPARAM(long); fputc(10, out); break;
case 35:
DBG printf("ret\n");
fprintf(out, "ret "); 
fputc(10, out); break;
