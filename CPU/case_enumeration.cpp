case 0:
DBG printf("hlt\n");
iter = thou->codesize; break;

case 1:
DBG printf("push\n");
GETPARAM("push", double, thou, iter, val1);
ASSERTED(StackPush, "push", thou->stack, &val1);
break;

case 2:
DBG printf("push\n");
GETPARAM("push(reg)",short, thou, iter, param);
ASSERTED(StackPush, "push(reg)",  thou->stack, &thou->rax + param - 1);
break;

case 3:
DBG printf("push\n");
GETPARAM("push(reg+num)",  short, thou, iter, param);
GETPARAM("push(reg+num)", double, thou, iter, val1 );
val1 += *(&thou->rax + param - 1);
ASSERTED(StackPush, "push(reg+num)", thou->stack, &val1);
break;

case 5:
DBG printf("push\n");
GETPARAM("push([num])", double, thou, iter, val1);
ASSERTED(Round, "push([num])", val1, RAM_SIZE - 1, &lparam)
ASSERTED(StackPush, "push([num])", thou->stack, thou->RAM + lparam);
break;

case 6:
DBG printf("push\n");
GETPARAM("push([reg])", short , thou, iter, param);
ASSERTED(Round, "push([reg])", *(&thou->rax + param - 1), RAM_SIZE - 1, &lparam);
ASSERTED(StackPush, "push([reg])", thou->stack, thou->RAM + lparam);
break;

case 7:
DBG printf("push\n");
GETPARAM("push([reg+num])", short , thou, iter, param);
GETPARAM("push([reg+num])", double, thou, iter, val1 );
ASSERTED(Round, "push([reg+num])", *(&thou->rax + param - 1), RAM_SIZE - 1, &lparam);
ASSERTED(Round, "push([reg+num])", val1, RAM_SIZE - 1 - lparam, &lparam);
ASSERTED(StackPush, "push([reg+num])", thou->stack, thou->RAM + lparam);
break;

case 8:
DBG printf("pop\n");
ASSERTED(StackPop,"pop", thou->stack, NULL);
break;

case 10:
DBG printf("pop\n");
GETPARAM("pop(reg)", short, thou, iter, param);
ASSERTED(StackPop, "pop(reg)", thou->stack, &val1);
*(&thou->rax + param - 1) = val1;
break;

case 11:
DBG printf("pop\n");
GETPARAM("pop(reg+num)", short , thou, iter, param);
GETPARAM("pop(reg+num)", double, thou, iter, val1 );
ASSERTED(StackPop, "pop(reg+num)", thou->stack, &val2);
*(&thou->rax + param - 1) = val1 + val2;
break;

case 13:
DBG printf("pop\n");
GETPARAM("pop([num])", double, thou, iter, val1 );
ASSERTED(Round, "pop([num])", val1, RAM_SIZE - 1, &lparam);
ASSERTED(StackPop, "pop([num])", thou->stack, &val1);
(thou->RAM)[lparam] = val1;
break;

case 14:
DBG printf("pop\n");
GETPARAM("pop([reg])", short , thou, iter, param);
ASSERTED(StackPop, "pop([reg])", thou->stack, &val1);
ASSERTED(Round, "pop[reg]",  *(&thou->rax + param - 1), RAM_SIZE - 1, &lparam);
(thou->RAM)[lparam] = val1;
break;

case 15:
DBG printf("pop\n");
GETPARAM("pop([reg+num])", short , thou, iter, param);
GETPARAM("pop([reg+num])", double, thou, iter, val1 );
ASSERTED(StackPop, "pop([reg+num])", thou->stack, &val2);
ASSERTED(Round, "pop[reg]",  *(&thou->rax + param - 1), RAM_SIZE - 1, &lparam)
ASSERTED(Round, "push([reg+num])", val1, RAM_SIZE - 1 - lparam, &lparam);
(thou->RAM)[lparam] = val2;
break;

case 4:
DBG printf("peek\n");
GETPARAM("peek", short, thou, iter, param);
ASSERTED(StackPeek,"peek", thou->stack, &val1);
*(&thou->rax + param - 1) = val1;
break;

case 9:
DBG printf("add\n");
ASSERTED(StackPop,"add", thou->stack, &val2);
ASSERTED(StackPop,"add", thou->stack, &val1);
val1 += val2;
StackPush(thou->stack, &val1);
break;

case 12:
DBG printf("sub\n");
ASSERTED(StackPop,"sub", thou->stack, &val2);
ASSERTED(StackPop,"sub", thou->stack, &val1);
val1 -= val2;
StackPush(thou->stack, &val1);
break;

case 16:
DBG printf("mul\n");
ASSERTED(StackPop,"mul", thou->stack, &val2);
ASSERTED(StackPop,"mul", thou->stack, &val1);
val1 *= val2;
StackPush(thou->stack, &val1);
break;

case 17:
DBG printf("div\n");
ASSERTED(StackPop,"div", thou->stack, &val2);
ASSERTED(StackPop,"div", thou->stack, &val1);
ASSERTED(IsZero,"div", val2); 
val1 /= val2;
StackPush(thou->stack, &val1);
break;

case 18:
DBG printf("pow\n");
ASSERTED(StackPop,"pow", thou->stack, &val2);
ASSERTED(StackPop,"pow", thou->stack, &val1);
val1 = pow(val1, val2);
StackPush(thou->stack, &val1);
break;

case 19:
DBG printf("sqrt\n");
ASSERTED(StackPop,"sqrt", thou->stack, &val1);
val1 = sqrt(val1);
StackPush(thou->stack, &val1);
break;

case 20:
DBG printf("sin\n");
ASSERTED(StackPop,"sin", thou->stack, &val1);
val1 = sin(val1);
StackPush(thou->stack, &val1);
break;

case 21:
DBG printf("cos\n");
ASSERTED(StackPop,"cos", thou->stack, &val1);
val1 = cos(val1);
StackPush(thou->stack, &val1);
break;

case 22:
DBG printf("neg\n");
ASSERTED(StackPop,"neg", thou->stack, &val1);
val1 = -val1;
StackPush(thou->stack, &val1);
break;

case 23:
DBG printf("in\n");
if (scanf_s("%lf", &val1) == 0) {
    printf("Error while processing in near 0x%X byte\n", iter);
    return iter; }
ASSERTED(StackPush, "in", thou->stack, &val1);
break;

case 24:
DBG printf("out\n");
ASSERTED(StackPop, "out", thou->stack, &val1);
printf("%lf\n", val1);
break;

case 25:
DBG printf("dump\n");
fStackDump(thou->stack, 0, stdout);
break;

case 26:
DBG printf("heck\n");
GETPARAM("heck", short, thou, iter, param);
break;

case 27:
DBG printf("jmp\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
iter = lparam;
break;

case 28:
DBG printf("ja\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 > val2) iter = lparam;
break;

case 29:
DBG printf("jae\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 >= val2) iter = lparam;
break;

case 30:
DBG printf("jb\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 < val2) iter = lparam;
break;

case 31:
DBG printf("jbe\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 <= val2) iter = lparam;
break;

case 32:
DBG printf("je\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 == val2) iter = lparam;
break;

case 33:
DBG printf("jne\n");
GETPARAM("jmp", long, thou, iter, lparam);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing jmp near 0x%X byte\n", iter); return iter; }
ASSERTED(StackPop, "ja", thou->stack, &val2);
ASSERTED(StackPop, "ja", thou->stack, &val1);
if (val1 != val2) iter = lparam;
break;

case 34:
DBG printf("call\n");
GETPARAM("call", long, thou, iter, lparam);
StackPush(callstk, &iter);
if (lparam < 0 || lparam > thou->codesize) {
    printf("Error while processing call near 0x%X byte\n", iter); return iter; }
iter = lparam;
break;

case 35:
DBG printf("ret\n");
ASSERTED(StackPop, "ret", callstk, &lparam);
iter = lparam;
break;

