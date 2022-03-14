case -848813528:
DBG printf("hlt\n");
cmdname=0;
goto writing;

case 1758067240:
DBG printf("push\n");
cmdname = ProcessComplexArg(&reader, &param0, &param1, &param1_writer, &param2_writer);
if (cmdname == -1 || cmdname == 0b000 || cmdname == 0b100) return ERR_NO_VALID_PUSH_ARG;
goto writing;

case 1351274247:
DBG printf("pop\n");
cmdname = ProcessComplexArg(&reader, &param0, &param1, &param1_writer, &param2_writer);
if (cmdname == -1 || cmdname == 0b001 || cmdname == 0b100) return ERR_NO_VALID_POP_ARG;
cmdname |= 0b1000;
goto writing;

case 647887389:
DBG printf("peek\n");
if (*reader != 'r' || !IsReg(++reader)) {
    printf("ERROR: peek argument not found on line %d\n",line);
    return ERR_NO_VALID_PEEK_ARG;
}

cmdname = 4;
param0 = *reader - 'a' + 1;
param1_writer = (char*)(&param0);

reader+=2;
SkipInteger(&reader);
goto writing;

case 376099609:
DBG printf("add\n");
cmdname=9;
goto writing;

case 1916296482:
DBG printf("sub\n");
cmdname=12;
goto writing;

case 146225734:
DBG printf("mul\n");
cmdname=16;
goto writing;

case -1868854133:
DBG printf("div\n");
cmdname=17;
goto writing;

case 1351274254:
DBG printf("pow\n");
cmdname=18;
goto writing;

case -682590862:
DBG printf("sqrt\n");
cmdname=19;
goto writing;

case -1738644542:
DBG printf("sin\n");
cmdname=20;
goto writing;

case 1811088237:
DBG printf("cos\n");
cmdname=21;
goto writing;

case -1568705662:
DBG printf("neg\n");
cmdname=22;
goto writing;

case -1305262369:
DBG printf("in\n");
cmdname=23;
goto writing;

case 736249328:
DBG printf("out\n");
cmdname=24;
goto writing;

case 175587406:
DBG printf("dump\n");
cmdname=25;
goto writing;

case -1945425981:
DBG printf("heck\n");
cmdname = 26;
if (sscanf_s((const char*)reader, "%hd", &param0) == 0) {
    printf("ERROR: heck argument not found on line %d\n",line);
    return ERR_NO_VALID_HECK_ARG;
}
param1_writer = (char*)(&param0);
ToNextStr(&reader);
goto writing;

case -1743781233:
DBG printf("jmp\n");
cmdname=27;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 1504713627:
DBG printf("ja\n");
cmdname=28;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case -1103754984:
DBG printf("jae\n");
cmdname=29;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 1504713628:
DBG printf("jb\n");
cmdname=30;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 1706221025:
DBG printf("jbe\n");
cmdname=31;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 1504713631:
DBG printf("je\n");
cmdname=32;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 1066194765:
DBG printf("jne\n");
cmdname=33;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case 970510868:
DBG printf("call\n");
cmdname=34;
if (ProcessLabelLinkArg(&reader, &param1_writer, &param2))
    {
        printf("ERROR: jump argument not found on line %d\n",line);
        return ERR_NO_VALID_JUMP_ARG;
    }
goto writing;

case -388658477:
DBG printf("ret\n");
cmdname=35;
goto writing;

