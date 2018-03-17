
#define DoMath(oper) \
    StackPush( &MyProc->MyStack, StackPop( &MyProc->MyStack) oper StackPop( &MyProc->MyStack)); \
    Pos++;

DEF_CMD(DEV,1, DoMath(/))

DEF_CMD(MUL,2, DoMath(*))

DEF_CMD(SUB,3, DoMath(+))

DEF_CMD(ADD,4, DoMath(-))

#undef DoMath




#define DoMath(oper) \
    StackPush( &MyProc->MyStack, oper(StackPop( &MyProc->MyStack ))); \
    Pos++;

DEF_CMD(SQRT,5, DoMath(sqrt))

DEF_CMD(SIN,6, DoMath(sin))

DEF_CMD(COS,7, DoMath(cos))

DEF_CMD(LOG,8, DoMath(log))

#undef DoMath





DEF_CMD(OUT,9,
    if (MyProc->MyStack.count > 0)
        printf("%lg\n", MyProc->MyStack.Data[MyProc->MyStack.count - 1]);
    Pos++;
)

DEF_CMD(JMP,15,
    Pos = MyProg->MachineCode[++Pos];
)





#define JUMP(oper) \
    if (StackPop( &MyProc->MyStack) oper StackPop( &MyProc->MyStack)) \
    { \
        Pos = MyProg->MachineCode[Pos + 1]; \
    } else \
        Pos += 2;


DEF_CMD(JE,16, JUMP(==))

DEF_CMD(JA,17, JUMP(>))

DEF_CMD(JAE,18, JUMP(>=))

DEF_CMD(JB,19, JUMP(<))

DEF_CMD(JBE,20, JUMP(<=))

#undef JUMP





DEF_CMD(CALL,21,
    StackPush(&MyProc->RetStack, Pos + 2);
    Pos = MyProg->MachineCode[++Pos];
)

DEF_CMD(RET,22,

    Pos = (int) StackPop(&MyProc->RetStack);

    if (Pos == 0 )
        return 0;

)



DEF_CMD(MAU, 33,
    printf("o___/\n"
           " ^ ^\n");
    Pos++;
)

DEF_CMD(HOLD,23,
    return 0;
)

DEF_CMD(IN,24,
    scanf("%f", &input);
    StackPush( &MyProc->MyStack, input );
    Pos++;
)





DEF_CMD(POP,10,
    Pos++;
    if ( MyProg->MachineCode[Pos] == 1 )
        StackPop( &MyProc->MyStack );
    else
        if (MyProg->MachineCode[Pos] == 2 )
            MyProc->RAM[ (int) MyProg->MachineCode[++Pos]] = StackPop( &MyProc->MyStack );
        else
            if (MyProg->MachineCode[Pos] == 3 )
                MyProc->RAM[ (int)MyProc->Reg[(int) MyProg->MachineCode[++Pos]]] = StackPop( &MyProc->MyStack );
            else
                MyProc->Reg[ (int) MyProg->MachineCode[++Pos]] = StackPop( &MyProc->MyStack );
    Pos++;
)

DEF_CMD(PUSH,11,
    Pos++;
    if ( MyProg->MachineCode[Pos] == 1 )
        StackPush( &MyProc->MyStack, MyProg->MachineCode[++Pos] );
    else
        if (MyProg->MachineCode[Pos] == 2 )
            StackPush( &MyProc->MyStack, MyProc->RAM[ (int) MyProg->MachineCode[++Pos]] );
        else
            if (MyProg->MachineCode[Pos] == 2 )
                StackPush( &MyProc->MyStack, MyProc->RAM[ (int)MyProc->Reg[(int) MyProg->MachineCode[++Pos]]] );
            else
                StackPush( &MyProc->MyStack, MyProc->Reg[ (int) MyProg->MachineCode[++Pos]] );
    Pos++;
)