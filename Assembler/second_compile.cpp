case 0:
DBG printf("[SecondCompile] hlt\n");
break;
case 1:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(double);
break;
case 2:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(short);
break;
case 3:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(short);
iter+=sizeof(double);
break;
case 5:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(double);
break;
case 6:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(short);
break;
case 7:
DBG printf("[SecondCompile] push\n");
iter+=sizeof(short);
iter+=sizeof(double);
break;
case 8:
DBG printf("[SecondCompile] pop\n");
break;
case 10:
DBG printf("[SecondCompile] pop\n");
iter+=sizeof(short);
break;
case 11:
DBG printf("[SecondCompile] pop\n");
iter+=sizeof(short);
iter+=sizeof(double);
break;
case 13:
DBG printf("[SecondCompile] pop\n");
iter+=sizeof(double);
break;
case 14:
DBG printf("[SecondCompile] pop\n");
iter+=sizeof(short);
break;
case 15:
DBG printf("[SecondCompile] pop\n");
iter+=sizeof(short);
iter+=sizeof(double);
break;
case 4:
DBG printf("[SecondCompile] peek\n");
iter+=sizeof(short);
break;
case 9:
DBG printf("[SecondCompile] add\n");
break;
case 12:
DBG printf("[SecondCompile] sub\n");
break;
case 16:
DBG printf("[SecondCompile] mul\n");
break;
case 17:
DBG printf("[SecondCompile] div\n");
break;
case 18:
DBG printf("[SecondCompile] pow\n");
break;
case 19:
DBG printf("[SecondCompile] sqrt\n");
break;
case 20:
DBG printf("[SecondCompile] sin\n");
break;
case 21:
DBG printf("[SecondCompile] cos\n");
break;
case 22:
DBG printf("[SecondCompile] neg\n");
break;
case 23:
DBG printf("[SecondCompile] in\n");
break;
case 24:
DBG printf("[SecondCompile] out\n");
break;
case 25:
DBG printf("[SecondCompile] dump\n");
break;
case 26:
DBG printf("[SecondCompile] heck\n");
iter+=sizeof(short);
break;
case 35:
DBG printf("[SecondCompile] ret\n");
break;
